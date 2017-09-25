import datetime

tstart = None
tend = None


def start_time():
    global tstart
    tstart = datetime.datetime.now()


def get_dalta():
    global tstart
    tend = datetime.datetime.now()
    return tend-tstart

# fibonacci
def fib(n):
    return n if n == 0 or n == 1 else fib(n - 1) + fib(n - 2)


def fib_seq(n):
    seq = []
    if n > 0:
        seq.extend(fib_seq(n - 1))

    seq.append(fib(n))
    return seq


start_time()
print "About to calculate the fibonacci sequence for the number 30"
delta1 = get_dalta()

start_time()
seq = fib_seq(30)
delta2 = get_dalta()

print "Now we print the number:"
start_time()
for n in seq:
    print n

delta3 = get_dalta()

print "============Profiling results============="
print "Time required to print a simple message: %(delta1)s" % locals()
print "Time required to calculate fibonacci: %(delta2)s" % locals()
print "Time required to iterate and print the numbers: %(delta3)s" % locals()
print "================"
