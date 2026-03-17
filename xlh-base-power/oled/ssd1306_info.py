import datetime
import os
import subprocess
import time
import netifaces as ni
import psutil
import logging
from PIL import ImageFont
from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306

logging.basicConfig(level=logging.INFO)

def td_to_hhmmss(td: datetime.timedelta) -> str:
    total_seconds = int(td.total_seconds())
    hours, rem = divmod(total_seconds, 3600)
    minutes, seconds = divmod(rem, 60)
    return f"{hours:02d}:{minutes:02d}:{seconds:02d}"

def get_bluetooth_mac() -> str:
    try:
        output = subprocess.check_output("bluetoothctl list", shell=True).decode("utf-8").strip()
        return output.split(" ")[1].replace(":", "-")[-8:].lower()
    except:
        return '11-22-33-44-55-66'

bluetoth_mac = get_bluetooth_mac()

serial = i2c(port=1, address=0x3C)
device = ssd1306(serial, width = 128, height = 64, rotate=0)
font14 = ImageFont.truetype('freefont/FreeMonoBold.ttf', 14)

# variables required for auto-restart codesyscontrol
restart = True
restart_time = datetime.timedelta(hours=1, minutes=58, seconds=0)
current_time_old = datetime.datetime.now() # used to detect datetime

logging.info('startup')
while(True):
    try:
        current_time = datetime.datetime.now()
        if current_time - current_time_old > restart_time:
            current_time_old = current_time
            if restart:
                out = subprocess.check_output(['sudo', 'systemctl', 'restart', 'codesyscontrol', '&'])
                logging.info('xLH restart')
        line_1 = f'xLH {td_to_hhmmss(restart_time - (current_time - current_time_old))}'
    except:
        line_1 = 'xLH'

    try:
        ssid = os.popen("nmcli -t -f active,ssid dev wifi | grep '^yes' | cut -d: -f2").read()
        line_2 = ssid

        ni.ifaddresses('wlan0')
        ip = ni.ifaddresses('wlan0')[ni.AF_INET][0]['addr']
        line_3 = ip
    except:
        line_2 = 'NO WiFi'
        line_3 = 'CONNECTION'

    try:
        line_4 = f'C {psutil.cpu_percent(interval=0.0):03.0f}% R {psutil.virtual_memory().percent:03.0f}%'
    except:
        line_4 = ''

    with canvas(device) as draw:
        draw.rectangle((0, 0, 127, 63), outline = 0, fill = 0)
        draw.text((0, 0),  line_1, font = font14, fill=1)
        draw.text((0, 16), line_2, font = font14, fill=1)
        draw.text((0, 32), line_3, font = font14, fill=1)
        draw.text((0, 48), line_4, font = font14, fill=1)

    time.sleep(1.0)


"""
sudo nano /etc/codesyscontrol/CODESYSControl.cfg
[CmpWebServer]
WebServerPortNr=8080

ssid = os.popen("iwconfig wlan0 | grep 'ESSID | awk '{print $4} | awk -F\" '{print $2}'").read()
if len(ssid) >= 1:  # WiFi Client
    line_2 = ssid
    line_3 = ip
elif len(ssid) == 0:  # WiFi AP
    line_1 = 'xLH-AP' + line_1[3:]
    line_2 = f'xlh-{bluetoth_mac}'
    line_3 = f'192.168.31.31'
else:
    line_2 = ''
    line_3 = ''
"""
