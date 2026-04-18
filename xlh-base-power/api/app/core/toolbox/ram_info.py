from functools import lru_cache


def _read_meminfo() -> dict[str, int]:
    values: dict[str, int] = {}
    try:
        with open('/proc/meminfo', 'r') as f:
            for line in f:
                key, _, rest = line.partition(':')
                rest = rest.strip()
                parts = rest.split()
                if not parts:
                    continue
                try:
                    amount = int(parts[0])
                except ValueError:
                    continue
                if len(parts) > 1 and parts[1].lower() == 'kb':
                    amount *= 1024
                values[key.strip()] = amount
    except Exception:
        return {}
    return values


@lru_cache
def _meminfo_static() -> dict[str, int]:
    return _read_meminfo()


def get_ram_total_bytes() -> int:
    return _meminfo_static().get('MemTotal', 0)


def get_ram_available_bytes() -> int:
    return _read_meminfo().get('MemAvailable', 0)


def get_ram_free_bytes() -> int:
    return _read_meminfo().get('MemFree', 0)


def get_ram_used_bytes() -> int:
    info = _read_meminfo()
    total = info.get('MemTotal', 0)
    available = info.get('MemAvailable', info.get('MemFree', 0))
    return max(total - available, 0)


def get_swap_total_bytes() -> int:
    return _meminfo_static().get('SwapTotal', 0)


def get_swap_free_bytes() -> int:
    return _read_meminfo().get('SwapFree', 0)


def get_ram_usage_percent() -> float:
    total = get_ram_total_bytes()
    if total == 0:
        return 0.0
    return round(get_ram_used_bytes() / total * 100, 2)


def bytes_to_human(num: int) -> str:
    value = float(num)
    for unit in ('B', 'KiB', 'MiB', 'GiB', 'TiB'):
        if value < 1024 or unit == 'TiB':
            return f'{value:.2f} {unit}'
        value /= 1024
    return f'{value:.2f} TiB'


def get_ram_info() -> dict:
    total = get_ram_total_bytes()
    available = get_ram_available_bytes()
    used = get_ram_used_bytes()
    return {
        'total_bytes': total,
        'available_bytes': available,
        'used_bytes': used,
        'free_bytes': get_ram_free_bytes(),
        'usage_percent': get_ram_usage_percent(),
        'swap_total_bytes': get_swap_total_bytes(),
        'swap_free_bytes': get_swap_free_bytes(),
    }


if __name__ == '__main__':
    info = get_ram_info()
    for key, value in info.items():
        if key.endswith('_bytes'):
            print(f'{key}: {bytes_to_human(value)}')
        else:
            print(f'{key}: {value}')
