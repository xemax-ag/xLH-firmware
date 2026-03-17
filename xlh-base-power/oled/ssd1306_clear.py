import os
import subprocess
import time
import netifaces as ni
from PIL import ImageFont
from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306

serial = i2c(port=1, address=0x3C)
device = ssd1306(serial, width = 128, height = 64, rotate=0)
font14 = ImageFont.truetype('freefont/FreeMonoBold.ttf', 14)

# Variables for oled content
line_1 = ""
line_2 = ""
line_3 = ""
line_4 = ""

with canvas(device) as draw:
    draw.rectangle((0, 0, 127, 63), outline = 0, fill = 0)
    draw.text((0, 0),  line_1, font = font14, fill=1)
    draw.text((0, 16), line_2, font = font14, fill=1)
    draw.text((0, 32), line_3, font = font14, fill=1)
    draw.text((0, 48), line_4, font = font14, fill=1)
