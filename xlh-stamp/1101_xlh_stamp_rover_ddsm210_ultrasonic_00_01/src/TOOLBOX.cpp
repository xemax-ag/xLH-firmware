#include <stdint.h>
#include <Arduino.h>
#include "M5AtomS3.h"
#include <EEPROM.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "DDSM210.h"

LGFX_Sprite sprite = LGFX_Sprite(&M5.Lcd);

void visu_setup(void)
{
    sprite.createSprite(AtomS3.Display.width(), AtomS3.Display.height());
}

// FixMe: Long Press für die Performance der Darstellung mittels SDO-Write
void visu_loop(void)
{
    sprite.fillScreen(TFT_BLACK);
    sprite.setTextColor(WHITE);
    sprite.setTextDatum(top_left);
    // sprite.setFont(&fonts::FreeMonoBold12pt7b);
    sprite.setFont(&fonts::FreeMonoBold9pt7b);
    sprite.setCursor(0, 5);
    sprite.printf("DDSM-DIST\r\n");

    /*
    // FixMe
    // if (can_open.in.byMode = 0) sprite.printf("OPEN LOOP\r\n");
    if (can_open.in.byMode = 2)
        sprite.printf("SPEED MODE\r\n");
    // else if (can_open.in.byMode = 3) sprite.printf("POS MODE\r\n");
    else
        sprite.printf("WRONG MODE\r\n");
    */

    // sprite.printf("D0 %d\r\n", can_open.out.uiUltrasonic[0]);
    // sprite.printf("D1 %d\r\n", can_open.out.uiUltrasonic[1]);
    sprite.printf("SPEED %d\r\n", can_open.out.iSpeed);
    //sprite.printf("POS %d\r\n", can_open.out.diPosition);
    //sprite.printf("MIL %d %d\r\n", ddsm210.diDdsm210Mileage, ddsm210.diDeltaPos);
    sprite.printf("CUR %d\r\n", can_open.out.iCurrent);
    sprite.printf("ERROR %04X\r\n", can_open.out.uiErrorCode);

    sprite.printf("CO-ID %02d\r\n", can_open.node_id);
    sprite.printf("STATE ", can_open.node_id);
    if (can_open.node_guard_state == NODE_GUARD_STATE_STOPPED)
        sprite.printf("STOP\r\n", can_open.node_id);
    else if (can_open.node_guard_state == NODE_GUARD_STATE_PRE_OPERATIONAL)
        sprite.printf("PRE OP\r\n", can_open.node_id);
    else if (can_open.node_guard_state == NODE_GUARD_STATE_OPERATIONAL)
        sprite.printf("OPRT\r\n", can_open.node_id);
    else
        sprite.printf("ERROR\r\n", can_open.node_id);
    sprite.printf("%s\r\n", VERSION);
    sprite.pushSprite(0, 0);
}

/*

void visu_loop(void)
{
    static uint8_t node_guard_state_old;

    if (digitalRead(GPIO_TOUCH_MODE) == 0) // ECO-MODE
    {
        if ((digitalRead(GPIO_TOUCH_BTN) == 0) || (can_open.node_guard_state != node_guard_state_old))
        {
            // slow update display => performance
            sprite.fillScreen(TFT_BLACK);
            sprite.setTextColor(WHITE);
            sprite.setTextDatum(top_left);
            // sprite.setFont(&fonts::FreeMonoBold12pt7b);
            sprite.setFont(&fonts::FreeMonoBold9pt7b);
            sprite.setCursor(0, 5);
            sprite.printf("xLH-IO-BASE\r\n");
            sprite.printf("CO-ID %02d\r\n", can_open.node_id);
            sprite.printf("STATE ", can_open.node_id);
            if (can_open.node_guard_state == NODE_GUARD_STATE_STOPPED)
                sprite.printf("STOP\r\n", can_open.node_id);
            else if (can_open.node_guard_state == NODE_GUARD_STATE_PRE_OPERATIONAL)
                sprite.printf("PRE OP\r\n", can_open.node_id);
            else if (can_open.node_guard_state == NODE_GUARD_STATE_OPERATIONAL)
                sprite.printf("OPRT\r\n", can_open.node_id);
            else
                sprite.printf("ERROR\r\n", can_open.node_id);
            sprite.printf("%s\r\n", VERSION);
            sprite.pushSprite(0, 0);
        }
    }
    else // FULL-MODE
    {
        node_guard_state_old = 0xFF;
        // fast update display
        sprite.fillScreen(TFT_BLACK);
        sprite.setTextColor(WHITE);
        sprite.setTextDatum(top_left);
        // sprite.setFont(&fonts::FreeMonoBold12pt7b);
        sprite.setFont(&fonts::FreeMonoBold9pt7b);
        sprite.setCursor(0, 5);
        sprite.printf("xLH-IO-BASE\r\n");
        sprite.printf("DI %02X DO %02X\r\n", can_open.out.byInput, can_open.in.byOutput);
        sprite.printf("AI %03X %03X\r\n", can_open.out.uiAdc1, can_open.out.uiAdc2);
        sprite.printf("AO %03X %03X\r\n", can_open.in.uiDac1, can_open.in.uiDac2);
        sprite.printf("CO-ID %02d\r\n", can_open.node_id);
        sprite.printf("STATE ", can_open.node_id);
        if (can_open.node_guard_state == NODE_GUARD_STATE_STOPPED)
            sprite.printf("STOP\r\n", can_open.node_id);
        else if (can_open.node_guard_state == NODE_GUARD_STATE_PRE_OPERATIONAL)
            sprite.printf("PRE OP\r\n", can_open.node_id);
        else if (can_open.node_guard_state == NODE_GUARD_STATE_OPERATIONAL)
            sprite.printf("OPRT\r\n", can_open.node_id);
        else
            sprite.printf("ERROR\r\n", can_open.node_id);
        sprite.printf("%s\r\n", VERSION);
        sprite.pushSprite(0, 0);
    }

    node_guard_state_old = can_open.node_guard_state;
}

*/