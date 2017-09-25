#!/usr/bin/env python

# -*- coding: utf-8 -*-

import re
from data_utils import search_email


if __name__ == "__main__":
    email_str = "elvis.zhang@hotmail.com, zhangsan@163.net, lishi@cloudminds.net"
    results = search_email(email_str)
    print results
