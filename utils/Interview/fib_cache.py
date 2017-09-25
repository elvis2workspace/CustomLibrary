import time

# 斐波拉契数列
def decorator(func):
    cache = {}

    def wrap(*args):
        if args not in cache:
            cache[args] = func(*args)
        return cache[args]

    return wrap


@decorator
def fib(n):
    if n == 1 or n == 0:
        return 1
    else:
        return fib(n - 2) + fib(n - 1)


@decorator
def climb(n, steps):
    count = 0
    if n < 1:
        count = 1
    else:
        for step in steps:
            count += climb(n - step, steps)
    return count


if __name__ == '__main__':
    start = time.time()
    print [fib(n) for n in range(40)]
    end = time.time()

    print 'cost:{}'.format(end - start)

    print climb(5, (1, 2))
