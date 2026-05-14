# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

Firmware for the **xLH terminal** (model `1210_xlh_terminal_00_02`): an M5Stack AtomS3 (ESP32-S3) acting as a **CANopen slave node** that exposes two daisy-chains of M5Stack Chain peripherals (key-buttons, encoders) to a CODESYS PLC master. The companion CODESYS projects and EDS file live under `codesys/`.

## Build / flash / monitor

PlatformIO project — there is no Make/CMake. Use the PlatformIO CLI or VS Code task runner:

- Build: `pio run`
- Upload: `pio run -t upload` (defaults to `COM3` at 1.5 Mbaud — change `upload_port` in `platformio.ini` if needed)
- Serial monitor: `pio device monitor -b 921600`
- Clean: `pio run -t clean`

Environment is `m5stack-atoms3` (ESP32-S3, Arduino framework). Library deps (`FastLED`, `M5AtomS3`, `M5Unified`) are pulled from the registry; `ESP32-TWAI-CAN` and `M5Chain-main` are vendored under `lib/` and built automatically by PlatformIO's lib LDF.

No `test/` suite is wired up — the `test/` directory only contains PlatformIO's stock README.

## Architecture

### Concurrency model (`src/main.cpp`)

`setup()` brings up display, EEPROM, two chains and the CAN stack, then starts a hardware timer and three statically-allocated FreeRTOS tasks:

- **Arduino `loop()`** (default task, prio inherited): polls the touch button via `can_open.loop()` to cycle node ID, and measures cycle/loop time into the object dictionary. 10 ms tick.
- **`loop_chain1`** (prio 25, 25 KB stack): runs `chain1.cyclic()` every 1 ms — handles UART1 (pins 38/39, header J3).
- **`loop_chain2`** (prio 24, 25 KB stack): runs `chain2.cyclic()` every 1 ms — handles UART2 (pins 5/6, header J4).
- **`loop_display`** (prio 0, 5 KB stack): runs `visu_loop()` every 50 ms.
- **`TimerHandler0`** ISR fires every `ISR_TIMER_0_TIME_US` = **16 ms** (in `CONFIG.h`) and drives the CANopen RX/TX cycle via `can_open.cyclic_isr_rx()` / `cyclic_isr_tx()`. The ISR `ms_counter` increment depends on this constant — keep them aligned.

### CANopen stack (`src/CAN_OPEN_BASE.cpp` + `src/CAN_OPEN.cpp`)

Two-layer design:

- `CAN_OPEN_BASE` is a generic CANopen slave: NMT, node-guard, SDO upload/download for a small object dictionary (`0x1000` device type, `0x1018` vendor ID, `0x1800..0x1803` TPDO event times, `0x5800` status info), and the cyclic RX/TX drivers. Bus speed is hard-coded **1 Mbit/s** on TX=GPIO7 / RX=GPIO8 (see `CAN_RX_PIN`/`CAN_TX_PIN`).
- `CAN_OPEN` (the concrete subclass) overrides `rx_pdo_1..4`, `tx_pdo_1..4` and `reset_output` to map the chain I/O into the four RPDO/TPDO frames. **Always call the `CAN_OPEN_BASE::` version first** in the override (see existing pattern) — the base class drives the ms-counter / send-on-event-time logic and clears outputs on non-operational states.
- COB-IDs follow standard CANopen `node_id + 0x180/0x200/...` offsets (see `ids()`). Node ID is **persisted in EEPROM byte `addr_co_eeprom`** (slot 0) and incremented on each touch-button press (`GPIO_TOUCH_BTN` = GPIO41) — clamped 1..16.

### PDO mapping (project-specific!)

`CAN_OPEN::rx_pdo_1` unpacks one 8-byte RPDO into **two-bit RGB color codes** (`CHAIN_RGB_OFF/RED/GREEN/BLUE` = 0..3) for chain1 LEDs 0..10 and chain2 LEDs 0..12, plus a single brightness byte (`byRxData1[7]`, clamped 0..100) broadcast to all 32 LEDs. `tx_pdo_1` / `tx_pdo_2` pack chain1/chain2 key-button bitmasks (`out.value8Bit[0..1]`) and encoder values (`out.value16Bit[0..2]`) into TPDOs. **TPDOs are change-of-state**: a frame is only sent when a byte differs from `out_old` *and* the node is `OPERATIONAL`.

### M5Chain layer (`src/CHAIN1.cpp`, `src/CHAIN2.cpp`)

Two `CHAIN` instances scan their UART for up to 16 daisy-chained M5Chain devices. On first cyclic call the chain runs `init()` — `malloc`s a `device_list_t`, sets RGB / button mode for each device, then per cycle: pushes brightness/color changes (only on diff vs `inOld`), reads `keyButtonStatus` and `encoderIncValue`, and packs them into `out.value8Bit[]` / `out.value16Bit[]` for the CANopen TPDOs. The two chains differ only in UART (`Serial1`/`Serial2`) and pins.

### Memory & data flow summary

```
PLC (CODESYS master)
   │  CAN @1Mbit/s (GPIO7/8)
   ▼
CAN_OPEN  ──RPDO──►  chain{1,2}.in  ──►  M5Chain UART  ──►  LEDs
CAN_OPEN  ◄─TPDO──  chain{1,2}.out ◄──  M5Chain UART  ◄──  buttons/encoders
```

## Conventions and gotchas

- **Hungarian-style prefixes** are used throughout (`abyTxData1`, `uiValue`, `diValue`, `udi…`) and unions (`u_co_data_*`, `can_msg_data_*_to_byte`) handle little-endian byte packing for CAN frames. Match the existing style when adding fields.
- The `CHAIN` headers and `.cpp` files for chain1/chain2 are near-duplicates — when changing logic in one, mirror it in the other (or refactor both).
- Identifiers `OBJ_DICT_PDO_RX_2/3/4_COM_PARS` in `CAN_OPEN_BASE.h` are all `0x1401` — looks like a copy-paste bug; don't blindly "fix" without checking impact on the EDS.
- `M5Chain` device init `malloc`s but never `free`s; treat the device list as set-once at boot.
- Display task uses LovyanGFX via M5AtomS3 (`LGFX_Sprite`); the AtomS3 USB-CDC quirk is noted in `platformio.ini` comments — re-read those before enabling USB CDC build flags.
- CODESYS-side artifacts live in `codesys/` (`.project` files + EDS) — keep `1210_xlh_terminal_rev1.eds` in sync when object dictionary entries change.
