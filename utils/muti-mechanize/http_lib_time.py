import httplib
import time

class Transaction(object):
    def run(self):
        conn = httplib.HTTPConnection('www.example.com')
        start = time.time()
        conn.request('GET', '/')
        request_time = time.time()
        resp = conn.getresponse()
        response_time = time.time()
        conn.close()
        transfer_time = time.time()

        self.custom_timers['request sent'] = request_time - start
        self.custom_timers['response received'] = response_time - start
        self.custom_timers['content transferred'] = transfer_time - start

        assert (resp.status == 200), 'Bad Response: HTTP %s' % resp.status


if __name__ == '__main__':
    trans = Transaction()
    trans.run()

    for timer in ('request sent', 'response received', 'content transferred'):
        print '%s: %.5f secs' % (timer, trans.custom_timers[timer])