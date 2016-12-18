"""Simple HTTP Server.

This module builds on BaseHTTPServer by implementing the standard GET
and HEAD requests in a fairly straightforward manner.

"""

__version__ = "0.6"

__all__ = ["SimpleHTTPRequestHandler"]

import os
import posixpath
import BaseHTTPServer
import urllib
import cgi
import sys
import shutil
import mimetypes

try:
    from cStringIO import StringIO
except ImportError:
    from StringIO import StringIO
DIR = ''
prune_length = 0


def set_directory(d):
    global DIR, prune_length
    DIR = d
    prune_length = len(DIR)


files = []


class SimpleHTTPRequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    """Simple HTTP request handler with GET and HEAD commands.

    This serves files from the current directory and any of its
    subdirectories.  The MIME type for files is determined by
    calling the .guess_type() method.

    The GET and HEAD requests are identical except that the HEAD
    request omits the actual contents of the file.

    """

    server_version = "SimpleHTTP/" + __version__

    def do_GET(self):
        """Serve a GET request."""
        f = self.send_head()
        if f:
            self.copyfile(f, self.wfile)
            f.close()

    def do_HEAD(self):
        """Serve a HEAD request."""
        f = self.send_head()
        if f:
            f.close()

    def send_head(self):
        """Common code for GET and HEAD commands.

        This sends the response code and MIME headers.

        Return value is either a file object (which has to be copied
        to the outputfile by the caller unless the command was HEAD,
        and must be closed by the caller under all circumstances), or
        None, in which case the caller has nothing further to do.

        """
        # print self.path
        path = self.tanmay(self.path)
        self.tanmay(self.path)
        # print path
        f = None
        if os.path.isdir(path):
            if not self.path.endswith('/'):
                print 'tanmau'
                # redirect browser - doing basically what apache does
                self.send_response(301)
                self.send_header("Location", self.path + "/")
                self.end_headers()
                return None
            for index in "index.html", "index.htm":
                index = os.path.join(path, index)
                if os.path.exists(index):
                    path = index
                    break
            else:
                return self.list_directory(path)
        ctype = self.guess_type(path)
        try:
            # Always read in binary mode. Opening files in text mode may cause
            # newline translations, making the actual size of the content
            # transmitted *less* than the content-length!
            f = open(path, 'rb')
        except IOError:
            self.send_error(404, "File not found")
            return None
        self.send_response(200)
        self.send_header("Content-type", ctype)
        print ctype
        fs = os.fstat(f.fileno())
        self.send_header("Content-Length", str(fs[6]))
        self.send_header("Last-Modified", self.date_time_string(fs.st_mtime))
        self.end_headers()
        return f

    def list_directory(self, path):
        """Helper to produce a directory listing (absent index.html).

        Return value is either a file object, or None (indicating an
        error).  In either case, the headers are sent, making the
        interface the same as for send_head().

        """
        try:
            list = os.listdir(path)
        except os.error:
            self.send_error(404, "No permission to list directory")
            return None
        list.sort(key=lambda a: a.lower())
        f = StringIO()
        displaypath = cgi.escape(urllib.unquote(self.path))
        f.write('<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">')
        f.write("<html>\n<title>Directory listing for %s</title>\n" % displaypath)
        f.write("<body>\n<h2>Directory listing for %s</h2>\n" % displaypath)
        f.write("<hr>\n<ul>\n")
        for name in list:
            fullname = os.path.join(path, name)
            # print '###################'
            # print fullname
            displayname = linkname = name
            # Append / for directories or @ for symbolic links
            if os.path.isdir(fullname):
                displayname = name + "/"
                linkname = name + "/"
            if os.path.islink(fullname):
                displayname = name + "@"
                # Note: a link to a directory displays with @ and links with /
            try:
                f.write('<li><a href="%s">%s</a>\n' % (urllib.quote(linkname.encode("utf-8")), cgi.escape(displayname)))
                # print '<li><a href="%s">%s</a>\n'% (urllib.quote(linkname), cgi.escape(displayname))
            except:
                # f.write('<li><a href="%s">%s</a>\n'% (linkname, displayname))
                # print linkname
                continue
        f.write("</ul>\n<hr>\n</body>\n</html>\n")
        length = f.tell()
        f.seek(0)
        self.send_response(200)
        encoding = sys.getfilesystemencoding()
        self.send_header("Content-type", "text/html; charset=%s" % encoding)
        self.send_header("Content-Length", str(length))
        self.end_headers()
        return f

    def translate_path(self, path):
        """Translate a /-separated PATH to the local filename syntax.

        Components that mean special things to the local file system
        (e.g. drive or directory names) are ignored.  (XXX They should
        probably be diagnosed.)

        """
        # abandon query parameters
        path = path.split('?', 1)[0]
        path = path.split('#', 1)[0]
        # Don't forget explicit trailing slash when normalizing. Issue17324
        trailing_slash = path.rstrip().endswith('/')
        path = posixpath.normpath(urllib.unquote(path))
        words = path.split('/')
        words = filter(None, words)
        path = os.getcwd()
        for word in words:
            drive, word = os.path.splitdrive(word)
            head, word = os.path.split(word)
            if word in (os.curdir, os.pardir): continue
            path = os.path.join(path, word)
        if trailing_slash:
            path += '/'
        return path

    def tanmay(self, p):
        x = url = urllib.unquote(p).decode('utf8')
        if x.endswith('/'):
            y = x[:-1]
            y = y.replace('/', '\\')
            # print '@@@@@@@@@@@@@@@@@@@@@@'
            y += '/'
            # print True
        else:
            y = x
            y = y.replace('/', '\\')
        path = DIR + y
        # print '@@@@@@@@@@@@@@@@@@@@@@'
        return path

    def copyfile(self, source, outputfile):
        """Copy all data between two file objects.

        The SOURCE argument is a file object open for reading
        (or anything with a read() method) and the DESTINATION
        argument is a file object open for writing (or
        anything with a write() method).

        The only reason for overriding this would be to change
        the block size or perhaps to replace newlines by CRLF
        -- note however that this the default server uses this
        to copy binary data as well.

        """
        shutil.copyfileobj(source, outputfile)

    def guess_type(self, path):
        """Guess the type of a file.

        Argument is a PATH (a filename).

        Return value is a string of the form type/subtype,
        usable for a MIME Content-type header.

        The default implementation looks the file's extension
        up in the table self.extensions_map, using application/octet-stream
        as a default; however it would be permissible (if
        slow) to look inside the data to make a better guess.

        """

        base, ext = posixpath.splitext(path)
        if ext in self.extensions_map:
            return self.extensions_map[ext]
        ext = ext.lower()
        if ext in self.extensions_map:
            return self.extensions_map[ext]
        else:
            return self.extensions_map['']

    if not mimetypes.inited:
        mimetypes.init()  # try to read system mime.types
    extensions_map = mimetypes.types_map.copy()
    extensions_map.update({
        '': 'application/octet-stream',
        '.mkv': 'octet-stream',
        '.avi': 'octet-stream',
        '.flv': 'octet-stream',  # Default
        '.py': 'text/plain',
        '.c': 'text/plain',
        '.h': 'text/plain',
    })


import os, time

f = open('files.txt', 'w')


def get_filepaths(directory):
    """
    This function will generate the file names in a directory 
    tree by walking the tree either top-down or bottom-up. For each 
    directory in the tree rooted at directory top (including top itself), 
    it yields a 3-tuple (dirpath, dirnames, filenames).
    """
    file_paths = []  # List which will store all of the full filepaths.
    global prune_length
    print prune_length
    # Walk the tree.
    for root, directories, files in os.walk(directory):
        for filename in files:
            # Join the two strings in order to form the full filepath.
            filepath = os.path.join(root, filename)
            filepath = filepath.replace('\\', '/')
            filepath = filepath[prune_length:]
            file_paths.append((filename, filepath))  # Add it to the list.
        print 'sleeping'
        time.sleep(2)

    return file_paths  # Self-explanatory.


def test(HandlerClass=SimpleHTTPRequestHandler,
         ServerClass=BaseHTTPServer.HTTPServer):
    BaseHTTPServer.test(HandlerClass, ServerClass)


'''
'''

from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import os, urllib, urllib2
from easygui import *
import socket

'''
print([ip for ip in socket.gethostbyname_ex(socket.gethostname())[2] if not ip.startswith("127.")][:1])

'''
ip = ''
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("Gmail", 80))
    ip = s.getsockname()[0]
    s.close()
except:
    print 'Internet Connectivity issues'
    pass
nick = raw_input('Enter your nick\n')
msgbox("Select the directory which you want to share")
d = diropenbox()


# print d
def hello():
    global nick
    while True:
        if DIR != '':
            x = get_filepaths(DIR)
            print 'done'
            urllib2.urlopen('http://localhost:3000/upload/' + nick + '/', str(x))
            break
        else:
            print 'doing'
            continue


def run():
    global ip, nick
    print('http server is starting...')

    # ip and port of server
    # by default http server port is 80
    server_address = ('', 80)
    set_directory(d)
    httpd = HTTPServer(server_address, SimpleHTTPRequestHandler)
    sa = httpd.socket.getsockname()
    print "Serving HTTP on", sa[0], "port", sa[1], "..."
    try:
        x = urllib2.urlopen('http://localhost:3000/nick/' + nick + '/' + ip)
        x = x.read()
        print x
    except:
        print 'not found'
    httpd.serve_forever()


def main():
    from threading import Thread

    # Second thread will print the hello message. Starting as a daemon means
    # the thread will not prevent the process from exiting.
    t = Thread(target=hello)
    t.daemon = True
    t.start()
    # Main thread will read and process input
    run()


if __name__ == '__main__':
    main()
