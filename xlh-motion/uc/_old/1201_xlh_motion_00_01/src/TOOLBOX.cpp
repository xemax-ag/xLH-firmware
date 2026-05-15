#include <stdint.h>
#include <Arduino.h>
#include "M5AtomS3.h"
//#include "M5CoreS3.h"
#include <EEPROM.h>
#include "CONFIG.h"
#include "CAN_OPEN.h"
#include "TOOLBOX.h"
#include "SPI_IO.h"

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
    sprite.printf("MOTION\r\n");

    sprite.printf("MGN %d\r\n", spi_io.uc.d.uiMagnetDutyCycle);
    sprite.printf("\r\n");
    sprite.printf("\r\n");
    sprite.printf("\r\n");
    
    //sprite.printf("CLK %d\r\n", spi_io.fpga.d.byDigInClk);
    //sprite.printf("CTL %d\r\n", spi_io.uc.d.byControl1);
    //sprite.printf("FRQ %d\r\n", spi_io.uc.d.diFrequency1);
    //sprite.printf("POS %d\r\n", spi_io.fpga.d.diPosition1);
    
    // sprite.printf("D1 %d\r\n", can_open.out.uiUltrasonic[1]);
    //sprite.printf("SPEED %d\r\n", can_open.out.iSpeed);
    //sprite.printf("POS %d\r\n", can_open.out.diPosition);
    //sprite.printf("MIL %d %d\r\n", ddsm210.diDdsm210Mileage, ddsm210.diDeltaPos);
    
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
