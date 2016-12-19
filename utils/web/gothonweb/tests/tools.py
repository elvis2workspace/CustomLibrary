from nose.tools import *
import re
from robot.utils.asserts import assert_equal

def assert_response(resp, contains=None, matches=None, headers=None, status="200"):
    assert status in resp.status, "Expected response %r not in %r" % (status, resp.status)
    
    if status == "200":
        assert resp.data, "Response data is empty."
    
    if contains:
        assert contains in resp.data, "Response doesn't contain %r" % contains
        
    if matches:
        reg = re.compile(matches)
        assert reg.matches(resp.data), "Response doesn't match %r " % matches
        
    if headers:
        assert_equal(resp.headers, headers)