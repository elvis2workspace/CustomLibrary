import httplib
import time
import urllib


class Transaction(object):
    def __init__(self):
        self.custom_timers = {}

    def run(self):
        post_body=urllib.urlencode({
            'USERNAME': 'corey',
            'PASSWORD': 'secret',})
        headers = {'Content-type': 'application/x-www-form-urlencoded'}

        start_timer = time.time()
        conn = httplib.HTTPConnection('www.example.com')
        conn.request('POST', '/login.cgi', post_body, headers)
        resp = conn.getresponse()
        content = resp.read()
        latency = time.time() - start_timer

        self.custom_timers['LOGIN'] = latency
        assert (resp.status == 200), 'Bad Response: HTTP %s' % resp.status
        assert ('Example Web Page' in content), 'Text Assertion Failed'