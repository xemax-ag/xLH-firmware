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
    sprite.printf("ADC DAC\r\n");

    sprite.printf("AI  %05d\r\n", can_open.out.iAdc);
    sprite.printf("AO0 %05d\r\n", can_open.in.uiDac0);
    sprite.printf("AO1 %05d\r\n", can_open.in.uiDac1);

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
