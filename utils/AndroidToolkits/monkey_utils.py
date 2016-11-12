#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

from utils.AndroidToolkits.custom_utils import *


def monkey_stress(config_file):
    stress_cmd = "monkey -f /data/local/tmp/" + config_file + " --throttle 500 -v -v 1"
    adb_shell(stress_cmd, by_shell=True)


def upload_monkey_config(config_file):
    if os.path.exists(config_file) is not True:
        print "File is not exist, check it please."
    else:
        upload_file(config_file, "/data/local/tmp/")

if __name__ == '__main__':
    upload_monkey_config("cusmonkey")
    monkey_stress("cusmonkey")

