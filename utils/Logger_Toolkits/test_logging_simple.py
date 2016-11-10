# -*- coding: utf-8 -*-

import logging
import logging.config


def logging_simple_scripts():
    # Configure the logging system
    logging.basicConfig(
        filename='app.log',
        level=logging.ERROR,
        format='%(levelname)s:%(asctime)s:%(message)s'
    )

    # Variables (to make the calls that follow work)
    hostname = 'www.python.org'
    item = 'spam'
    filename = 'data.csv'
    mode = 'r'

    # Example logging calls (insert into your program)
    logging.critical('Host %s unknown', hostname)
    logging.error("Couldn't find %r", item)
    logging.warning('Feature is deprecated')
    logging.info('Opening file %r, mode=%r', filename, mode)
    logging.debug('Got here')


def logging_simple_scripts1():
    # configure the logging system
    logging.config.fileConfig('logconfig.ini')

if __name__ == '__main__':
    logging_simple_scripts()
    logging.getLogger().level = logging.DEBUG
    logging_simple_scripts
