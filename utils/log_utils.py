# -*- coding: utf-8 -*-

import custom_exception
from CustomLibrary.config import config
from custom_utils import *


class LogUtils():

    def __init__(self):
        pass

    # capture the log after crash by time list of crash
    # app发生crash，未及时在logcat中抓取到有效log时，可通过该脚本获取到log，日志存放至crash_log目录
    @staticmethod
    def capture_crash_log(self, time_lists=None):
        if time_lists is None:
            pass
        else:
            if check_dir(config.CRASHLOGPATH) is not None:
                raise custom_exception.CustomException("Check %s operator failed." % config.CRASHLOGPATH)

            crash_log_file = os.path.join(config.CRASHLOGPATH, "Crash_log_%s.log" % timestamp())
            f = open(crash_log_file, "w")
            for t in time_lists:
                crash_log = adb_shell("dumpsys dropbox --print %s" % time).stdout.read()
                f.write(crash_log)

            f.close()

    # 获取app发生crash的时间列表
    @property
    def get_crash_time_list(self):
        time_list = []
        result_list = adb_shell("dumpsys dropbox | %s data_app_crash" % find_util).stdout.readlines()
        for t in result_list:
            temp_list = t.split(" ")
            temp_time = [temp_list[0], temp_list[1]]
            time_list.append(" ".join(temp_time))

        return time_list
