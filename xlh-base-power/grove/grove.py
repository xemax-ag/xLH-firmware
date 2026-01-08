import asyncio
import time
from mcp9808.modbus_mcp9808 import run_mcp9808_server

async def run_worker():
    tasks = [
        run_mcp9808_server(port=5020)
    ]
    await asyncio.gather(*tasks)

if __name__ == '__main__':
    asyncio.run(run_worker())
    time.sleep(0.5)