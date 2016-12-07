import requests

class Transaction(object):
    def run(self):
#         r = requests.get('https://github.com/timeline.json')
#         r = requests.get('https://www.python.org')
        r = requests.get('http://www.baidu.com', timeout=1)
        print r.cookies['BAIDUID']
        
        s = requests.Session()
        r = s.post('http://www.baidu.com')
        r.raw.read()
        if  'Python is a programming language' in r.content:
            print r.content
        else:
            pass
        
        print r.status_code
        print r.headers
        print r.headers['content-type']
        print "run."
        

if __name__ == '__main__':
    newobject = Transaction()
    newobject.run()