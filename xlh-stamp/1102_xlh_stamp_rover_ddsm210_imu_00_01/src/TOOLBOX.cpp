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

void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t *ypr)
{
    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    ypr->yaw *= RAD_TO_DEG;
    ypr->pitch *= RAD_TO_DEG;
    ypr->roll *= RAD_TO_DEG;
}