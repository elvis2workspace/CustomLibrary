# -*- coding: utf-8 -*-
import random
from functools import wraps


def func_cache(size=10):
    def func_wrapper(func):
        cache = {}

        @wraps(func)
        def inner_deco(*args, **kwargs):
            key = (args, frozenset(kwargs.items()))
            if key in cache:
                print('func {} is already cached with arguments {} {}'.format(func.__name__, args, kwargs))
                return cache[key]
            else:
                print('func {} is not cached with arguments {} {}'.format(func.__name__, args, kwargs))
                res = func(*args, **kwargs)
                if len(cache) >= size:
                    lucky_key = random.choice(list(cache.keys()))
                    print 'func {} cache pop {}'.format(func.__name__, lucky_key)
                    cache.pop(lucky_key, None)
                cache[key] = res
            return cache[key]
        return inner_deco
    return func_wrapper


@func_cache(size=3)
def add_two_number(a, b):
    return a + b


@func_cache
def product_two_number(a, b):
    return a*b


if __name__ == "__main__":
    print "##########################"
    print('add_two_number func name is {}'.format(add_two_number.__name__))
    print('1. add_two_number(1, 2)')
    add_two_number(1, 2)
    print(add_two_number.__name__)

    print "##########################"
    print('2. add_two_number(2, 3)')
    add_two_number(2, 3)

    print "##########################"
    print('3. add_two_number(1, b=2)')
    # add_two_number(1, 2)
    add_two_number(1, b=2)

    print "##########################"
    print('4. add_two_number(1, 2)')
    add_two_number(1, 2)

    print "##########################"
    print('5. product_two_number(1, 2)')
    product_two_number(1, 2)

    print "##########################"
    print('6. add_two_number(1, 3)')
    add_two_number(1, 3)

