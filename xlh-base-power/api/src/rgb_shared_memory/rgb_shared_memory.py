from multiprocessing import shared_memory
import time
from rpi_ws281x import PixelStrip, Color, ws
from typing import List
from pydantic import BaseModel
import pathlib
import sys

__cwd__ = str(pathlib.Path(__file__).parents[0])
sys.path.append(__cwd__)

# LED strip configuration:
# https://github.com/rpi-ws281x/rpi-ws281x-python
LED_COUNT = 64        # Number of LED pixels.
LED_PIN = 18          # GPIO pin connected to the pixels (18 uses PWM!).
LED_FREQ_HZ = 800000  # LED signal frequency in hertz (usually 800khz)
LED_DMA = 10          # DMA channel to use for generating signal (try 10)
LED_BRIGHTNESS = 255  # Set to 0 for darkest and 255 for brightest
LED_INVERT = False    # True to invert the signal (when using NPN transistor level shift)
LED_CHANNEL = 0       # set to '1' for GPIOs 13, 19, 41, 45 or 53
#LED_STRIP = ws.SK6812_STRIP_GBR
#LED_STRIP = ws.SK6812W_STRIP
LED_STRIP = ws.WS2811_STRIP_RGB


def rgb_cmd_pixel(strip: PixelStrip, values: List):
    if len(values) >= 192:
        for i in range(strip.numPixels()):
            j = int(i % 8)
            idx = (strip.numPixels() - i - 1) * 3
            # idx = i * 3
            strip.setPixelColor(i, Color(values[idx+0], values[idx+1], values[idx+2]))
        strip.show()


def rgb_cmd_wipe(strip: PixelStrip, color: Color):
    for i in range(strip.numPixels()):
        strip.setPixelColor(i, color)
    strip.show()


def rgb_init():
    strip = PixelStrip(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS, LED_CHANNEL, LED_STRIP)
    strip.begin()
    rgb_cmd_wipe(strip, Color(0, 0, 0))
    return strip


# https://superfastpython.com/multiprocessing-sharedmemory/
class RgbSharedMemory:
    COUNTER_RESET_LIMIT: int = 10
    MEM_NAME_CODESYS: str = 'Shared_Memory_Codesys'
    MEM_NAME_API: str = 'Shared_Memory_Api'
    MEM_SIZE: int = 200

    def __init__(self, mem_name: str = '', mem_size: int = 200):
        self._mem_name = mem_name
        self._mem_size = mem_size
        self._sm = None
        self._counter: int = 0
        self._counter_reset: int = 0
        self.active: bool = False

    def create(self):
        try:
            _sm = shared_memory.SharedMemory(name=self._mem_name, create=True, size=self._mem_size)
        except:
            # print('already exist')
            pass

    def read(self):
        try:
            _sm = shared_memory.SharedMemory(name=self._mem_name, create=False, size=self._mem_size)
            values = [int(_sm.buf[i]) for i in range(self._mem_size)]
            _sm.close()
        except Exception as exc:
            print(exc)
            values = [int(0) for _ in range(self._mem_size)]
        return values

    def write(self, values: List):
        try:
            _sm = shared_memory.SharedMemory(name=self._mem_name, create=False, size=self._mem_size)
            # values = [int(_sm.buf[i]) for i in range(self._mem_size)]
            for i in range(len(values)):
                _sm.buf[i] = values[i]
            _sm.close()
        except Exception as exc:
            print(exc)

    def state_update(self, counter: int):
        if self._counter != counter:
            self._counter = counter
            self._counter_reset = 0
            self.active = True
        else:
            if self._counter_reset < RgbSharedMemory.COUNTER_RESET_LIMIT:
                self._counter_reset += 1
            else:
                self.active = False


if __name__ == '__main__':
    print('start rgb_shared_memory')
    _strip = rgb_init()

    _rgb_sm_codesys = RgbSharedMemory(mem_name=RgbSharedMemory.MEM_NAME_CODESYS)
    _rgb_sm_api = RgbSharedMemory(mem_name=RgbSharedMemory.MEM_NAME_API)

    _values_codesys = []
    _values_api = []
    while True:
        time.sleep(0.05)
        try:
            _start_time = time.perf_counter()

            _rgb_sm_codesys.create()
            _values_codesys = _rgb_sm_codesys.read()
            _rgb_sm_codesys.state_update(counter=_values_codesys[199])
            if _rgb_sm_codesys.active:
                rgb_cmd_pixel(strip=_strip, values=_values_codesys)

                _rgb_sm_api.create()
                _values_api = [int(0) for i in range(RgbSharedMemory.MEM_SIZE)]
                _values_api[RgbSharedMemory.MEM_SIZE-1] = 0x55
                _rgb_sm_api.write(values=_values_api)
            else:
                _rgb_sm_api.create()
                _values_api = _rgb_sm_api.read()
                rgb_cmd_pixel(strip=_strip, values=_values_api)

            # print(f'{_rgb_sm_codesys.active} => {_values_codesys[199]:03d} => {((time.perf_counter() - _start_time) * 1000.0):0.3f} ms')
        except Exception as e:
            print(e)
            pass




"""
def sharedMemoryWrite(memName, memString, memSize, memTime):
    # Create new shared memory object
    shm = shared_memory.SharedMemory(name=memName, create=False, size=memSize)
    # Encode string in utf-8
    encoded = memString.encode('utf-8')
    # Convert string to bytearray
    bArr = bytearray(encoded)
    # Write every single element in shared memory buffer
    if len(bArr) < memSize:
        for i in range(len(bArr)):
            shm.buf[i] = bArr[i]
    # Delay to read the memory in other application, before close
    #time.sleep(memTime)
    shm.close()

%%time
s = str(datetime.datetime.now())
sharedMemoryWrite(memName='Shared_Memory_RGB_Codesys', memString=s, memSize=memSize, memTime=0.5)
"""
