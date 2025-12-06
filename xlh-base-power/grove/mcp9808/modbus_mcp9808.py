import asyncio
import logging
import time

import adafruit_mcp9808
import board
import busio
from pymodbus.datastore import ModbusDeviceContext, ModbusSequentialDataBlock, ModbusServerContext
from mcp9808.server_async import run_async_server, setup_server

_logger = logging.getLogger(__name__)
_logger.setLevel(logging.ERROR)

def int_to_uint(value)->int:
    if value < 0:
        value = max(value, -32768)
        return 65535 + value + 1
    else:
        return min(value, 32767)

def mcp9808_init():
    try:
        i2c = busio.I2C(board.SCL, board.SDA)
        return adafruit_mcp9808.MCP9808(i2c)
    except Exception as e:
        return None

def mcp9808_read(mcp9808):
    try:
        if mcp9808 is None:
            mcp9808 = mcp9808_init()
        values = [
            int_to_uint(1),
            int_to_uint(int(mcp9808.temperature * 100))
        ]
    except Exception as e:
        values = [
            int_to_uint(-1),
            int_to_uint(0)
        ]
    return values


class CallbackDataBlock(ModbusSequentialDataBlock):
    def __init__(self, queue, addr, values):
        self.queue = queue
        self.mcp9808 = mcp9808_init()
        super().__init__(addr, values)

    def setValues(self, address, value):
        super().setValues(address, value)

    def getValues(self, address, count=1):
        if address == 1 and count == 2:
            super().setValues(1, mcp9808_read(self.mcp9808))
        result = super().getValues(address, count=count)
        return result


async def run_mcp9808_server(port: int | None = None):
    """Define datastore callback for server and do setup."""
    queue: asyncio.Queue = asyncio.Queue()
    block = CallbackDataBlock(queue, 0x0000, [0] * 10)
    block.setValues(1, 0)
    store = ModbusDeviceContext(di=block, co=block, hr=block, ir=block)
    context = ModbusServerContext(devices=store, single=True)
    run_args = setup_server(
        description="Run callback server.", port=port, context=context
    )
    await run_async_server(run_args)


if __name__ == '__main__':
    while True:
        try:
            asyncio.run(run_mcp9808_server(port=5020), debug=False)
        except Exception as exc:
            time.sleep(0.5)