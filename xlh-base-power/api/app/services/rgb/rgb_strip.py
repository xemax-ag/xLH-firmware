from typing import List
from pydantic import BaseModel
from app_rgb.rgb_shared_memory import RgbSharedMemory
from multiprocessing import shared_memory
from enum import IntEnum

LED_COUNT = 64        # Number of LED pixels.
VALUE_LIMIT = 50

class eRgbHw(IntEnum):
    RGB_PCB_BLACK = 1
    RGB_PCB_RED = 2

class eRgbRemoteState(IntEnum):
    CODESYS = 1
    API = 2
    ERROR = 3

class RgbPixel(BaseModel):
    r: int = 0
    g: int = 0
    b: int = 0

    model_config = {
        'json_schema_extra': {
            'examples': [
                {
                    "r": 0, "g": 0, "b": 0
                }
            ]
        }
    }

class RgbMatrixValues(BaseModel):
    values: List[List[RgbPixel]]
    # rgb_hw: int = eRgbHw.RGB_PCB_BLACK.value

    model_config = {
        'json_schema_extra': {
            'examples': [
                {
                    "values":
                        [
                            [{"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0}],
                            [{"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0}],
                            [{"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0}],
                            [{"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0}],
                            [{"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0}],
                            [{"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0}],
                            [{"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0}],
                            [{"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0},
                             {"r": 0, "g": 0, "b": 0}, {"r": 0, "g": 0, "b": 0}],
                        ]
                }
            ]
        }
    }

def value_limit(rgb_pixel: RgbPixel):
    if rgb_pixel.r < 0:
        rgb_pixel.r = 0
    if rgb_pixel.g < 0:
        rgb_pixel.g = 0
    if rgb_pixel.b < 0:
        rgb_pixel.b = 0

    limit = VALUE_LIMIT
    if rgb_pixel.r > limit:
        rgb_pixel.r = limit
    if rgb_pixel.g > limit:
        rgb_pixel.g = limit
    if rgb_pixel.b > limit:
        rgb_pixel.b = limit
    return rgb_pixel

def rgb_cmd_wipe(rgb_pixel: RgbPixel):
    rgb_pixel = value_limit(rgb_pixel)
    rgb_values = []
    for i in range(64):
        rgb_values.append(rgb_pixel.r)
        rgb_values.append(rgb_pixel.g)
        rgb_values.append(rgb_pixel.b)
    for i in range(8):
        rgb_values.append(0)
    # print(len(rgb_values))

    try:
        sm = shared_memory.SharedMemory(name=RgbSharedMemory.MEM_NAME_API, create=False, size=RgbSharedMemory.MEM_SIZE)
        if int(sm.buf[RgbSharedMemory.MEM_SIZE - 1]) == 0:
            rgb_state = eRgbRemoteState.API
        else:
            rgb_state = eRgbRemoteState.CODESYS

        for i in range(len(rgb_values)):
            sm.buf[i] = rgb_values[i]
        sm.close()
        return rgb_state

    except Exception as exc:
        print(exc)
        return eRgbRemoteState.ERROR

def rgb_cmd_pixel(values: List[List[RgbPixel]]):
    rgb_values = []
    for i in range(8):
        for j in range(8):
            rgb_pixel = value_limit(values[i][j])
            rgb_values.append(rgb_pixel.r)
            rgb_values.append(rgb_pixel.g)
            rgb_values.append(rgb_pixel.b)
    for i in range(8):
        rgb_values.append(0)
    # print(len(rgb_values))

    try:
        sm = shared_memory.SharedMemory(name=RgbSharedMemory.MEM_NAME_API, create=False, size=RgbSharedMemory.MEM_SIZE)
        if int(sm.buf[RgbSharedMemory.MEM_SIZE - 1]) == 0:
            rgb_state = eRgbRemoteState.API
        else:
            rgb_state = eRgbRemoteState.CODESYS

        for i in range(len(rgb_values)):
            sm.buf[i] = rgb_values[i]
        sm.close()
        return rgb_state

    except Exception as exc:
        print(exc)
        return eRgbRemoteState.ERROR