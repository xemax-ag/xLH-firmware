import time
from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from starlette.routing import Router
from routers.index import router as index_router
from routers.rgb import router as rgb_router
from routers.wifi import router as wifi_router
from toolbox.templates import templates

# =====================================================================================================================
app = FastAPI(
        title='xLH',
        version='0.9.0',
        description='''Data API <br> <a href="/">Home</a>''',
        swagger_ui_parameters={
            'syntaxHighlight': False
        },
        )

app.include_router(router=index_router, prefix='', tags=['index'], include_in_schema=False)
app.include_router(router=wifi_router, prefix='/wifi', tags=['wifi'], include_in_schema=True)
app.include_router(router=rgb_router, prefix='/rgb', tags=['rgb'], include_in_schema=True)

# =====================================================================================================================

app.mount(f'/static', StaticFiles(directory='static', html=True), name='static')
static_router = Router()
# static_router.mount('/source/static', StaticFiles(directory='static', html=True), name='static')

# =====================================================================================================================

# https://medium.com/@semaphoreci/building-custom-middleware-in-fastapi-a5deb05d87e7
@app.middleware("http")
async def add_process_time_header_http(request: Request, call_next):
    start_time = time.perf_counter()
    response = await call_next(request)
    process_time = time.perf_counter() - start_time
    response.headers["X-Process-Time"] = f'{process_time*1000.0:0.03f} ms'
    return response

@app.middleware("https")
async def add_process_time_header_https(request: Request, call_next):
    start_time = time.perf_counter()
    response = await call_next(request)
    process_time = time.perf_counter() - start_time
    response.headers["X-Process-Time"] = f'{process_time*1000.0:0.03f} ms'
    return response

# =====================================================================================================================

origins = [
    'https://addin.xlwings.org'
]
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=['*'],
    allow_headers=['*'],
)

# =====================================================================================================================
