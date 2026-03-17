import logging
from app.core.config import config
from enum import StrEnum

def setup_logging(level: int = logging.INFO):
    logging.basicConfig(
        level=level,
        # format="%(asctime)s %(levelname)s [%(name)s] %(message)s",
        format='%(asctime)s %(levelname)-8s %(message)s',
    )

def get_log_name(name: str = 'DEV'):
    return f'{config.get_worker_name()}_{name.upper()}'

if __name__ == "__main__":
    setup_logging()
    logging.info('123')
