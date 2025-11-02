#include <stdint.h>
#include <Arduino.h>
#include "M5AtomS3.h"
#include <EEPROM.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "TOOLBOX.h"

LGFX_Sprite sprite = LGFX_Sprite(&M5.Lcd);

void visu_setup(void)
{
    sprite.createSprite(AtomS3.Display.width(), AtomS3.Display.height());
}

void visu_loop(void)
{
    sprite.fillScreen(TFT_BLACK);
    sprite.setTextColor(WHITE);
    sprite.setTextDatum(top_left);
    // sprite.setFont(&fonts::FreeMonoBold12pt7b);
    sprite.setFont(&fonts::FreeMonoBold9pt7b);
    sprite.setCursor(0, 5);
    sprite.printf("TEMPLATE\r\n");

    sprite.printf("SI " PRINTF_BINARY_PATTERN_INT8 "\r\n", PRINTF_BYTE_TO_BINARY_INT8(can_open.out.byInputSwitch));
    sprite.printf("SO " PRINTF_BINARY_PATTERN_INT8 "\r\n", PRINTF_BYTE_TO_BINARY_INT8(can_open.in.byOutputSwitch));
    sprite.printf("BI " PRINTF_BINARY_PATTERN_INT8 "\r\n", PRINTF_BYTE_TO_BINARY_INT8(can_open.out.byInputButton));
    sprite.printf("BO " PRINTF_BINARY_PATTERN_INT8 "\r\n", PRINTF_BYTE_TO_BINARY_INT8(can_open.in.byOutputButton));

    sprite.printf("ID %02d ", can_open.node_id);
    if (can_open.node_guard_state == NODE_GUARD_STATE_STOPPED)
        sprite.printf("STOP\r\n", can_open.node_id);
    else if (can_open.node_guard_state == NODE_GUARD_STATE_PRE_OPERATIONAL)
        sprite.printf("PREOP\r\n", can_open.node_id);
    else if (can_open.node_guard_state == NODE_GUARD_STATE_OPERATIONAL)
        sprite.printf("OPRT\r\n", can_open.node_id);
    else
        sprite.printf("ERROR\r\n", can_open.node_id);
    sprite.printf("%s\r\n", VERSION);
    sprite.pushSprite(0, 0);
}
