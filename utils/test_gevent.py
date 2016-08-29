from gevent import monkey
monkey.patch_all()

from gevent.pool import Pool
from time import sleep, time


def fetch_url(url):
    print "Fetching %s" % url
    sleep(10)
    print "Done fetching %s" % url


urls = ["http://test.com", "http://bacon.com", "http://eggs.com"]
p = Pool(10)
start = time()
p.map(fetch_url, urls)
print time() - start
