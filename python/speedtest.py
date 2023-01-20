import numpy as np
import time
import functools

def timer(func):
    @functools.wraps(func)
    def wrapper_timer(*args, **kwargs):
        tic = time.perf_counter()
        value = func(*args, **kwargs)
        toc = time.perf_counter()
        elapsed_time = toc - tic
        ns = elapsed_time // 1e-9
        us = elapsed_time // 1e-6
        ms = elapsed_time // 1e-3
        if ms > 0:
            print(f"{func.__name__}: {ms} ms")
        elif us > 0:
            print(f"{func.__name__}: {us} us")
        elif ns > 0:
            print(f"{func.__name__}: {ns} ns")
        return value
    return wrapper_timer

@timer
def add(a, b):
    return a + b

@timer
def np_add(a, b):
    return np.add(a, b)

@timer
def dumn_add(a, b):
    return [a[idx] + b[idx] for idx in range(0, len(a))]

n = int(1e5)
a = np.array([42] * n)
b = np.array([42] * n)

print(n)
c = add(a, b)
c = dumn_add(a, b)

n = int(1e8)
print(n)
a = np.array([42] * n, dtype=int)
b = np.array([42] * n, dtype=int)
c = np_add(a, b)
c = add(a, b)
