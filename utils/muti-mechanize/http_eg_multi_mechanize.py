import time

import mechanize


class Transaction(object):
    def run(self):
        br = mechanize.Browser()
        br.set_handle_robots(False)
        
        start_timer = time.time()
        resp = br.open('http://www.example.com/')
        resp.read()
        latency = time.time() - start_timer
        
        self.custom_timers['Example_Homepage'] = latency
        
        assert (resp.code == 200)
        assert ('Example Web Page' in resp.get_data())