from dotenv import load_dotenv
from pydantic_settings import BaseSettings, SettingsConfigDict
from pathlib import Path
dotenv_path = Path(__file__).resolve().parents[1] / '.env'
load_dotenv(dotenv_path=dotenv_path)

class Config(BaseSettings):
    app_name: str = ''
    app_version: str = ''
    app_description: str = '''<a href='/' target='_blank'>xLH</a>'''
    debug: bool = False

    

config = Config()

if __name__ == '__main__':
    for item in config.__dict__.items():
        print(f'{item[0]}: {item[1]}')
