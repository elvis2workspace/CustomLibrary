# -*- coding: utf-8 -*-

import base64

raw_str = "18912345678cidxx10.1.1.1pubxxx如何取消订单？ sxxxx"
print raw_str
raw_base = base64.urlsafe_b64encode(raw_str)
print raw_base
second_raw_base = base64.urlsafe_b64decode(raw_base)
print second_raw_base