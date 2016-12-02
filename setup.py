#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from os.path import join, dirname
from ez_setup import use_setuptools
from setuptools import setup

sys.path.append(join(dirname(__file__), 'src'))
use_setuptools()

execfile(join(dirname(__file__), 'src', 'CustomLibrary', 'version.py'))

setup(name='robotframework-customlibrary',
      version=VERSION,
      description='app testing library for Robot Framework',
      long_description=open(join(dirname(__file__), 'README.rst')).read(),
      author='Elvis Zhang',
      author_email='<xiuhai5052@hotmail.com>',
      url='https://github.com/elvis2workspace/CustomLibrary',
      license='Apache License 2.0',
      keywords='robotframework testing testautomation mobile appium webdriver app',
      platforms='any',
      classifiers=[
                        "Development Status :: 5 - Production/Stable",
                        "License :: OSI Approved :: Apache Software License",
                        "Operating System :: OS Independent",
                        "Programming Language :: Python",
                        "Topic :: Software Development :: Testing"
                     ],
      install_requires=[
                            'decorator >= 3.3.2',
                            'robotframework >= 2.6.0',
                            'docutils >= 0.8.1',
                            'Appium-Python-Client >= 0.5',
                            'mock >= 1.0.1',
                            'sauceclient >= 0.1.0',
                            'pytest-cov >= 1.8.1',
                            'pytest-xdist >= 1.11',
                            'pytest-pythonpath >= 0.4', 'redis', 'redis',
                         ],
      py_modules=['ez_setup'],
      package_dir={'': 'src'},
      packages=['CustomLibrary', 'CustomLibrary.keywords', 'CustomLibrary.locators', 'CustomLibrary.utils'],
      include_package_data=True,
      )
