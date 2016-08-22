# -*- coding: utf-8 -*-

import ConfigParser
from custom_utils import *


def test():
    now_ = datetime.now()
    today_ = date.today()


def test_config():
    config = ConfigParser.ConfigParser()
    with open("../cuscfg.cfg", "r+") as cfgfile:
        config.readfp(cfgfile)
        name = config.get("info", "name")
        age = config.get("info", "age")
        print name, age
        config.set("info", "name", "zxh")
        config.set("info", "sex", "female")
        config.set("info", "age", "28")
        name = config.get("info", "name")
        sex = config.get("info", "sex")
        age = config.get("info", "age")
        print name, sex, age


def test_config2():
    config = ConfigParser.ConfigParser()
    config.read("../cuscfg.cfg")

    sections = config.sections()
    print sections
    o = config.options("url")
    print o

    v = config.items("url")
    print v

    access = config.get("url", "GLOBAL_URL")
    print access

    config.add_section("runpath")
    config.set("runpath", "filepath", "D:\\")
    f = open("../cuscfg.cfg", "a+")
    config.write(f)
    f.close()


# 获取当前应用的包名
def get_current_package_name():
    return get_current_surface_package_activity().split("/")[0]


# 获取当前设备的activity
def get_current_activity():
    return get_current_surface_package_activity().split("/")[-1]

test_config()
test_config2()