#!/usr/bin/python
# -*- coding: UTF-8 -*-

# from DBUtils.PooledDB import PooledDB
import DBUtils.PooledDB
import MySQLdb

pool = DBUtils.PooledDB(MySQLdb, 5,host = "localhost", user = "root", passwd = "", db = "ADDRESSBOOKDB") 
"""
PooledDB�ĵ�һ��������creator: either an arbitrary function returning new DB-API 2 
        connection objects or a DB-API 2 compliant database module
Ҳ����˵�����Ǵ���һ�����ݿ�ʵ�����module���ּ��ɣ����Լ���ȥ�ж���ν������ݿ����ӡ�
"""
db_conn = pool.connection() # ����Ǵ����ӳ��л�ȡһ�����ӵ���� 