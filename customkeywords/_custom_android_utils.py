# -*- coding: utf-8 -*-
"""
Created on 2015年7月21日
@author: zhang.xiuhai
"""

from robot.api import logger
import os
import re
import subprocess
import time
from xml.dom import minidom
import zipfile
from CustomLibrary.utils import custom_utils
from CustomLibrary.p11 import *
from keywordgroup import KeywordGroup
from robot.libraries.BuiltIn import BuiltIn
from CustomLibrary.config import config


PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p))


custom_utils.check_dir(config.APPIUM_RUNLOG)



<<<<<<< HEAD
class _CustomAndroidKeywords(KeywordGroup):
    """
    classdocs
    """

    def __init__(self):
        """
        Constructor
        """
        self._p11_utils = P11Tester()
        self._biin = BuiltIn()

    # 清空edit text内容
    def clear_edit_text(self):
        pass

    # public
    def get_local_address(self):    
        """获取本地测试环境的IP地址"""
        tmp_config = os.popen('ipconfig').read()
        ip = re.search(r'192.168.*.*', tmp_config)
        return ip.group(0)
    
    def launch_local_appium(self, ip="127.0.0.1", tport="4723", mode="no-reset"):
        """根据ip，port，mode启动本地appium，例

        """
        launch_cmd = "appium -a " + str(ip) + " -p " + str(tport) + " " + "--" + mode + " " + \
                "--log-timestamp --local-timezone --session-override -g " + config.APPIUM_RUNLOG + "appium-runlog.txt"
        print "launch_cmd: ", launch_cmd

        tmp_pid = self.get_port_pid(config.APPIUMPORT)


        if tmp_pid is None:
            child = subprocess.Popen(launch_cmd, shell=True)
            if child.returncode is None:
                logger.info("Going to Launch the local appium tools, please wait....", also_console=True)
                child.wait()
        else:
            logger.info("Appium tool have been launched, pid: " + tmp_pid, also_console=True)
            return tmp_pid

    def stop_tookit(self, toolniki):
        """停止测试工具,例如本地 Appium
        """

        if "appium" == toolniki:
            pid = self.get_port_pid(config.APPIUMPORT)
            logger.info("Going to stop local appium, pid: ."+str(pid), also_console=True)
            stop_appium_cmd = "tskill " + str(pid)
            child = subprocess.Popen(stop_appium_cmd, shell=True)

            child.wait()
            return child.pid
        else:
            logger.console("No other tools to use.")
            
    def get_port_pid(self, port):
        """根据参数中的端口号查找对应使用该端口号的进程ID，并返回该进程的PID号。
        """

        get_pid_cmd = "netstat -ano | findstr  LISTENING | findstr " + str(port)
        appium_pid_str = os.popen(get_pid_cmd).read()
        
        if appium_pid_str:
            appium_g = appium_pid_str.split(' ')
            logger.console("Process about port " + str(port) + " have existed, is " + appium_g[-1] + ".", True, 'stdout')
            return appium_g[-1]
        else:
            logger.console("No process about port " + str(port) + "!", True, 'stdout')
            return None
        
    def set_androidlog_status(self, mode=True):
        """设置android日志开关
        """

        time_stamp = self._getcurtm()
        # 获取adb.exe的进程ID
        log_pid = self.get_cmd_pids('adb.exe')[1:]
        if log_pid.__len__() is not 0:
            print "log_pid: ", log_pid

        if mode is True:
            log_cmd_0 = "adb devices"
            log_cmd_1 = "adb logcat -s ecm_em ecm_jni ecm_em_okm ecm_ecms ecm_em_util ecm_ui_service ecm " \
                          "ecm_mqttservice libc ecm_ui cryptfunc cryptfunc_p2p cryptfunc_exchange cryptfunc_transfer" \
                          " MainService voltencryptd TelecomFramework > D:\Logs\\android-runlog\logcat_" +\
                        time_stamp + ".log"
            log_cmd_2 = "adb shell logcat -v time > D:\Logs\\android-runlog\logcat_" + time_stamp + ".log &1"
            # print "log_cmd_1: ", log_cmd_1
            child_str = subprocess.Popen(log_cmd_1, shell=True)
            time.sleep(5)
            if os.path.exists("D:\Logs\\android-runlog\logcat_" + time_stamp + ".log"):
                print "Open the android log tool for log successfully."
                return 0
            else:
                return -1
            # re_code = child_str.wait()
            # print "re_code: ", re_code
            # if re_code is 0:
            #     print "Open the android log tool for log successfully."
            #     return 0
            # else:
            #     print "Open the android log tool for log failed."
            #     return -1
        else:
            for i in log_pid:
                logoff_cmd = "taskkill -PID " + i + " /F"
                child = subprocess.Popen(logoff_cmd, shell=True)
                child.wait()

            if self.get_cmd_pids('adb.exe').__len__() == 1:
                print "Have closed all the adb.exe processes."
            else:
                print "Close the adb.exe processes failed!"
                return -1

    def grap_androidlog_after_oper(self, flag, path):
<<<<<<< HEAD
        """获取操作后日志    
        """
        os.system("adb logcat -v time -d > "+path+"log_" + flag + ".log &1")

    def get_cmd_pids(self, t_cmd):
        """获取执行命令的进程ID"""
        srchAdbCMD = "tasklist | findstr adb.exe"
        r_str = os.popen(srchAdbCMD).read()
        print "r_str: ", r_str
        rg = r_str.split(' ')
        pid_list = []
        num = 0
        for i in rg:
            if re.search(t_cmd, i):
                num += 1
            elif re.match(r'\d{5}|\d{4}', i):
                pid_list.append(i)
            else:
                pass
        return pid_list

    def kill_shell_process(self, pro_alias='ecm'):
        """杀掉指定进程"""

        adb_kill_cmd = "adb shell ps | grep " + str(pro_alias) + " | grep -v ecmapplication:"
        process_details = os.popen(adb_kill_cmd).read()
        # print "proDetails: "+ proDetails
        # logger.info(proDetails, also_console=True)
        is_null = (len(process_details) == 0)

        if is_null:
            logger.error(pro_alias + " is not alive.")
            return -1
        else:
            adb_pid = process_details.split(' ')
            logger.info(self._getcurtm() + ": The ecm application pid is: "+ str(adb_pid[4]), also_console=True)
            ret_val = os.system("adb shell kill " + str(adb_pid[4]))
            if 0 == ret_val:
                logger.info(self._getcurtm() + ": " + pro_alias + " " + str(adb_pid[4]) + " killed successfully.", \
                            also_console=True)
                return None
            else:
                logger.error("Failed to kill ecm pid.")
                return -1

    def reset_android(self, pro_alias=None, remote_url=None ):
        """重置android设备"""

        # 重启android设备
        ret_reb = os.popen("adb shell reboot").read()
        is_null = (len(ret_reb) == 0)
        if is_null:
                logger.info(self._getcurtm()+": Succeed rebooting android device.", also_console=True)
        else:
            logger.error(ret_reb)
            return -1
            
        # 等待android设备重新连接

        ret_val = subprocess.Popen("adb wait-for-device", shell=True)
        ret_val.wait()

        if ret_val.stdout is None:
            ret_dev = os.popen('adb devices').read()
            logger.info(self._getcurtm() + ": Android device named: "+ ret_dev.split('\n')[1].split('\t')[0] +
                        " connected.", also_console=True)
            time.sleep(15)
            logger.info(self._getcurtm() + ": Sleep 10 to wait return.", also_console=True)
            return None
        else:
            # logger.error(pro_alias + " "+ test_str(adbPid[4]) + " process fail to kill!")
            logger.error(self._getcurtm() + ": Android device connected timeout.")
            return -1

    def Cswipe(self, xstart, ystart, xend, yend):
        """自定义滑动屏幕关键字"""
        os.system("adb shell input swipe "+str(xstart)+" "+str(ystart)+" " +str(xend)+" "+str(yend))
        return 0  
    

    def run_all_batf(self, path):
        """接收一个目录的路径，并执行目录下的所有bat文件.例
         | run all bat | file path|
        """
        for root, dirs, files in os.walk(path):
            for f in files:
                if os.path.splitext(f)[1] == '.bat':
                    os.chdir(root)
                    # print root, f
                    os.system(f)

    def decode(self, customer_str):
        return customer_str.decode('utf-8')


    def setup_xml(self, test_name="", user_pin="123456", so_pin="12345678", loop_times="", data_len=""):
        """
        Setup the xml file:data.xml, config param include test_name, user_pin, so_pin, loop_times,
        data_len.
        example:
        | setup xml | test_name | user_pin | so_pin | loop_times | data_len |
        | setup xml | 101 | 123456 | 12345678 | 50 | 32 |
        """

        if os.path.exists(PATH(r"../res/data.xml")):
            os.remove(PATH(r"../res/data.xml"))

        doc = minidom.Document()
        doc.standalone = True
        root_node = doc.createElement("root")
        doc.appendChild(root_node)
        test_node = doc.createElement("Test")
        # book_node.setAttribute("isbn", "34909023")
        root_node.appendChild(test_node)
        test_name_node = doc.createElement("Testname")
        user_pin_node = doc.createElement("Userpin")
        so_pin_node = doc.createElement('Sopin')
        loop_times_node = doc.createElement('Looptimes')
        data_len_node = doc.createElement('Datalen')
        test_node.appendChild(test_name_node)
        test_node.appendChild(user_pin_node)
        test_node.appendChild(so_pin_node)
        test_node.appendChild(loop_times_node)
        test_node.appendChild(data_len_node)
        test_name_text_node = doc.createTextNode(test_name)
        test_name_node.appendChild(test_name_text_node)
        user_pin_text_node = doc.createTextNode(user_pin)
        user_pin_node.appendChild(user_pin_text_node)
        so_pin_text_node = doc.createTextNode(so_pin)
        so_pin_node.appendChild(so_pin_text_node)
        loop_times_text_node = doc.createTextNode(loop_times)
        loop_times_node.appendChild(loop_times_text_node)
        data_len_text_node = doc.createTextNode(data_len)
        data_len_node.appendChild(data_len_text_node)
        # doc.writexml(f, "/t", "/t", "/n", "utf-8")
        f = open(PATH(r"../res/data.xml"), "w")
        test_node.toprettyxml(encoding="utf-8")
        doc.writexml(f, encoding="utf-8")
        f.close()

        if os.path.exists(PATH(r"../res/data.xml")):
            logger.info("Setup xml file successfully.")
        else:
            logger.error("Failed to setup xml file.")
            return -1

    def adb_push(self, local_path=None, dest_path=None):
        ret_str = self._p11_utils.upload_file(local_path, dest_path)
        if ret_str is not None:
            logger.error("Upload file to terminal device failed: %s" % ret_str, html=True)
            return -1
        logger.info("adb push %s to terminal device successfully." % local_path, html=True, also_console=True)

    def run_p11test(self):
        """
        execute p11test.so to verify the p11 interface.
        """
        run_command1 = "adb shell " + "\"chmod 777 /data/p11test\""
        print run_command1
        re_str = os.system(run_command1)
        print re_str
        run_command2 = "adb shell " + "\"cd /data/ && .\/p11test\""
        if re_str < 0:
            logger.error("Setup the permission of p11test failed.")
            return -1
        else:
            output_str = os.system(run_command2)
            if output_str is None:
                return -1
            else:
                return output_str

    def unzip_file(self, zipfilename=None, unzipdir=None):
        """unzip file."""
        if not os.path.exists(unzipdir): os.mkdir(unzipdir, 0777)
        zfobj = zipfile.ZipFile(zipfilename)
        print zfobj

        for name in zfobj.namelist():
            print name
            name = name.replace('\\', '/')
            print name

            if name.endswith('/'):
                if os.path.exists(os.path.join(unzipdir, name)):
                    pass
                else:
                    os.mkdir(os.path.join(unzipdir, name))
            else:
                ext_filename = os.path.join(unzipdir, name)
                ext_dir = os.path.dirname(ext_filename)
                if not os.path.exists(ext_dir):
                    os.mkdir(ext_dir, 0777)
                outfile = open(ext_filename, 'wb')
                outfile.write(zfobj.read(name))
                outfile.close()

    def initial_environment_for_p11(self):
        """Initial the device environment fot the p11test project."""

        if self._p11_utils.initial_env_for_p11() is not None:
            logger.error("Failed to initial environment for p11 verify.")

    def adb_install_app(self, path_of_apk=None, options=None):
        """本地安装apk到终端设备"""

        install_command = "adb install -" + options + " " + path_of_apk
        # logger.info(install_command, html=True)
        print "[adb_install_app]: install_command: ", install_command
        result_adb_install = subprocess.Popen(install_command, shell=True, stdout=subprocess.PIPE,
                                              stderr=subprocess.PIPE)
        result_adb_install.wait()
        # flag = result_adb_install.stdout.readlines()
        print "result_adb_install.stdout: ", result_adb_install.stdout.readlines()
        print "result_adb_install.stderr: ", result_adb_install.stderr.readlines()

    # private
    def _cdebug(self, log_variable=None):
        print "[Debug]%s: %s" % ("log_variable", log_variable)

    def _execute_sql(self, path):
        logger.debug("Executing : %s" % path)
        print path

    def _getcurtm(self):
        return time.strftime("%Y-%m-%d-%H-%M-%S", time.localtime(time.time()))


    def _get_apk_package(self, apk=None):
        """获取指定apk的基本信息，package name, version name, launcher activity, and so on."""
        package_info = {}
        apk_path = os.path.join(config.APKLOCALPATH, apk)
        self._cdebug(apk_path)
        aapt_cmd = os.path.join(config.AAPTToolPATH, "aapt dump badging ")
        self._cdebug(aapt_cmd)
        parse_cmd = aapt_cmd + " " + apk_path
        return_str = os.popen(parse_cmd).readlines()
        self._cdebug(return_str)
        package_name = return_str[0].split(" ")
        print package_name[1]
        package_info['name'] = package_name[1].split("=")[1]
        package_info['version name'] = return_str[0].split("='")[3].split("'")[0]
        print package_info['version name']
        package_info['sdkVersion'] = return_str[1].split(":")[1]
        package_info['targetSdkVersion'] = return_str[2].split(":")[1]
        matching = [line for line in return_str if line.startswith("launchable")]
        package_info['startActivity'] = matching[0].split(" ")[1].split("=")[1]
        return package_info


if __name__ == '__main__':
    tmpObject = _CustomAndroidKeywords()
    # tmpObject.kill_shell_process("ecm")
    # tmpObject.reset_android()

    # adb_pid = tmpObject.get_cmd_pids('adb.exe')
    # print adb_pid
    # path = "E:/Python27/Lib/site-packages/CustomLibrary/res/SCS/SecureCenterService.apk"
    # tmpObject.adb_install_app(path, 'r')
    tmpObject._get_apk_package("EncryptCardManager.apk")
    # tmpObject.set_androidlog_status()
    # time.sleep(10)
    # tmpObject.set_androidlog_status(mode=False)
    # print tmpObject._getcurtm()
    # # print "adb_pid: ", adb_pid[1:]

    # tmppro = tmpObject.launch_local_appium("192.168.20.114", "4723", "no-reset")

#     print "run the testcase."
#     tmpObject.get_port_pid("4723")
#     time.sleep(10)
#     tmpObject.get_local_address()
#     tmpObject.stop_tookit("appium")
#     tmpObject.kill_adb_process('ecm')
#     tmpObject.set_androidlog_status('test729343434322', True)
#     tmpIp = tmpObject.get_local_address()

