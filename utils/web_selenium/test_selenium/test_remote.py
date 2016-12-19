# -*- coding: utf-8 -*-

from test_selenium.webdriver.common.desired_capabilities import DesiredCapabilities

from elvis.Web_selenium.test_selenium import webdriver


def process_remote_task(url):
    # Remote get dynamic page on selenium-standalone
    try:
        driver.set_page_load_timeout(5)
        driver.get(url)
    except:
        pass

    # Parse
    for tag in driver.find_elements_by_xpath(xpath):
        # ...process
        pass


if __name__ == '__main__':
    # Init selenium replace the ip port with your own selenium-standalone
    driver = webdriver.Remote(
        command_executor='http://127.0.0.1:4444/wd/hub',
        desired_capabilities=DesiredCapabilities.CHROME
    )

    process_remote_task()

    # Cleanup selenium
    driver.close()
