# -*- coding: utf-8 -*-
import random
from decorator import decorator


def func_cache(size=10):
    def _cache(func, *args, **kwargs):
        if not hasattr(func, '_cache'):
            func._cache = {}
            func._cache_size = size

        key = (args, frozenset(kwargs.items()))

        if key not in func._cache:
            print('func {} not hit cache'.format(func.__name__))
            res = func(*args, **kwargs)
            if len(func._cache) >= func._cache_size:
                lucky_key = random.choice(list(func._cache.keys()))
                func._cache.pop(lucky_key, None)
                print('func {} pop cache key {}'.format(func.__name__, lucky_key))
                func._cache[key] = res

        return func._cache[key]
    return decorator(_cache)   # 直接将参数列表为（func, *args, **kwargs)的函数转换成一个无参装饰器


@func_cache(size=3)
def add_two_number(a, b):
    return a + b


@func_cache()
def product_two_number(a, b):
    return a * b

if __name__ == "__main__":
    print('add_two_number func name is {}'.format(add_two_number.__name__))
    print('1. add_two_number(1, 2)')
    add_two_number(1, 2)
    print('2. add_two_number(2, 3)')
    add_two_number(2, 3)
    print('3. add_two_number(1, b=2)')
    add_two_number(1, b=2)
    print('4. add_two_number(1, 2)')
    add_two_number(1, 2)
    print('5. product_two_number(1, 2)')
    product_two_number(1, 2)
    print('6. add_two_number(1, 3)')
    add_two_number(1, 3)