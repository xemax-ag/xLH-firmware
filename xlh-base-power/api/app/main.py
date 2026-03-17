from http import HTTPStatus
from app.core.config import config
from fastapi import FastAPI, Request, status
from fastapi.responses import PlainTextResponse, JSONResponse, RedirectResponse
from contextlib import asynccontextmanager
import time
import logging
from fastapi.middleware.cors import CORSMiddleware
from starlette.exceptions import HTTPException as StarletteHTTPException
from app.api.html.html import router as html_router
from app.api.v1.rgb import router as rgb_router
from fastapi.staticfiles import StaticFiles
from app.core.toolbox.logging_app import setup_logging
from app.core.templates.base import template_base

setup_logging(level=logging.WARNING)

# =====================================================================================================================

@asynccontextmanager
async def lifespan(app: FastAPI):
    # print('startup')
    pass
    yield
    # print('shutdown')
    pass

app = FastAPI(
        lifespan=lifespan,
        title=config.app_name,
        version=config.app_version,
        description=config.app_description,
        swagger_ui_parameters={
            'syntaxHighlight': False
        },
        # docs_url = None,
        redirect_slashes=True,
    )

# =====================================================================================================================

app.include_router(router=html_router, prefix='', tags=['html'], include_in_schema=False)
app.include_router(router=rgb_router, prefix='/v1/rgb', tags=['v1 rgb'], include_in_schema=True)

# =====================================================================================================================

app.mount(f'/static', StaticFiles(directory='app/static', html=True), name='static')

# =====================================================================================================================

# https://medium.com/@semaphoreci/building-custom-middleware-in-fastapi-a5deb05d87e7
@app.middleware('http')
async def add_process_time_header_http(request: Request, call_next):
    start_time = time.perf_counter()
    # if request.url.path == '/playground/1/':
    #     return RedirectResponse(url='/login/')
    # zentrale Kontrolle ob valider session key vorhanden ist
    # print(f'cookies: {request.cookies}')
    response = await call_next(request)
    process_time = time.perf_counter() - start_time
    response.headers['x-process-time'] = f'{process_time*1000.0:0.03f} ms'
    return response

# @app.middleware('https')
# async def add_process_time_header_https(request: Request, call_next):
#     start_time = time.perf_counter()
#     response = await call_next(request)
#     process_time = time.perf_counter() - start_time
#     response.headers['x-process-time'] = f'{process_time*1000.0:0.03f} ms'
#     return response

# =====================================================================================================================

@app.exception_handler(StarletteHTTPException)
async def http_exception_handler(request: Request, exc):
    if 'v1' in request.url.path:
        return JSONResponse(content={'status': exc.status_code,
                                     'detail': HTTPStatus(exc.status_code).phrase})
    else:
        if exc.status_code == status.HTTP_403_FORBIDDEN:
            return template_base.TemplateResponse(request=request,
                                                  name='failure/403.html',
                                                  context={})
        elif exc.status_code == status.HTTP_404_NOT_FOUND:
            return template_base.TemplateResponse(request=request,
                                                  name='failure/404.html',
                                                  context={})
        elif exc.status_code == status.HTTP_500_INTERNAL_SERVER_ERROR:
            return template_base.TemplateResponse(request=request,
                                                  name='failure/500.html',
                                                  context={})
        else:
            return template_base.TemplateResponse(request=request,
                                                  name='failure/failure.html',
                                                  context={'status': exc.status_code,
                                                           'detail': HTTPStatus(exc.status_code).phrase,
                                                           'exc': str(exc)})

# =====================================================================================================================

# https://fastapi.tiangolo.com/es/tutorial/cors/#use-corsmiddleware
# https://medium.com/stackademic/securing-apis-with-fastapi-489c3d4d1ea0
origins = [
    '*',
]
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=['*'],
    allow_headers=['*'],
)

# =====================================================================================================================
