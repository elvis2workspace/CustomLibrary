import requests

class Transacation(object):
    def run(self):
        r = requests.get('https://github.com/timeline.json')
        r.raw.read()