from functools import lru_cache
import platform
import os
from enum import StrEnum


@lru_cache
def _read_cpuinfo() -> str:
    try:
        with open('/proc/cpuinfo', 'r') as f:
            return f.read()
    except Exception:
        return ''


def _first_field(field: str) -> str | None:
    for line in _read_cpuinfo().splitlines():
        if line.startswith(field):
            _, _, value = line.partition(':')
            return value.strip()
    return None


@lru_cache
def get_cpu_name() -> str:
    name = _first_field('model name') or _first_field('Hardware') or _first_field('Model')
    if name:
        return name
    return platform.processor() or platform.machine() or 'unknown'


@lru_cache
def get_cpu_arch() -> str:
    return platform.machine()


@lru_cache
def get_cpu_vendor() -> str | None:
    return _first_field('vendor_id') or _first_field('CPU implementer')


@lru_cache
def get_cpu_count_logical() -> int:
    return os.cpu_count() or 0


@lru_cache
def get_cpu_count_physical() -> int:
    ids = set()
    current_physical = None
    current_core = None
    for line in _read_cpuinfo().splitlines():
        if line.startswith('physical id'):
            current_physical = line.split(':', 1)[1].strip()
        elif line.startswith('core id'):
            current_core = line.split(':', 1)[1].strip()
        elif line.strip() == '':
            if current_physical is not None and current_core is not None:
                ids.add((current_physical, current_core))
            current_physical = None
            current_core = None
    if current_physical is not None and current_core is not None:
        ids.add((current_physical, current_core))
    return len(ids) if ids else get_cpu_count_logical()


@lru_cache
def get_cpu_frequency_mhz() -> float | None:
    value = _first_field('cpu MHz')
    if value:
        try:
            return float(value)
        except ValueError:
            return None
    return None


@lru_cache
def get_cpu_flags() -> tuple[str, ...]:
    value = _first_field('flags') or _first_field('Features')
    if not value:
        return ()
    return tuple(value.split())


@lru_cache
def get_cpu_info() -> dict:
    return {
        'name': get_cpu_name(),
        'arch': get_cpu_arch(),
        'vendor': get_cpu_vendor(),
        'logical_cores': get_cpu_count_logical(),
        'physical_cores': get_cpu_count_physical(),
        'frequency_mhz': get_cpu_frequency_mhz(),
        'flags': get_cpu_flags(),
    }


class XlhType(StrEnum):
    NONE = 'none'
    BASE = 'base'
    POWER = 'power'


@lru_cache
def get_xlh_type() -> XlhType:
    if 'Pi Zero' in get_cpu_name().lower():
        return XlhType.BASE
    elif 'Pi 4' in get_cpu_name():
        return XlhType.BASE
    elif 'Pi 5' in get_cpu_name():
        return XlhType.POWER
    return XlhType.NONE


if __name__ == '__main__':
    for key, value in get_cpu_info().items():
        if key == 'flags':
            print(f'{key}: {len(value)} flags')
        else:
            print(f'{key}: {value}')
