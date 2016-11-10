# -*- coding: utf-8 -*-


def get_each_prime(m):
    for n in range(2, m):
        for x in range(2, n):
            if n % x == 0:
                print n, " equals ", x, '*', n//x
                break
        else:  # this else is matching the inside for loop.
            # loop fell through without finding a factor.
            print n, ' is a prime number.'


def found_even_number(m):
    for num in range(2, m):
        if num % 2 == 0:
            print "Found an even number", num
            continue  # 循环继续执行下一次迭代
        print "Found a number", num

# def test_parameter(*args, sep = "/"):
#     return sep.join(args)


if __name__ == '__main__':
    get_each_prime(20)
    found_even_number(20)
    # test_parameter("earth","mars","venus")
