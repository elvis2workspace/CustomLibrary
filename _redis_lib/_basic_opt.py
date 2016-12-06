#! /usr/bin/env python
# -*- coding: utf-8 -*-

import redis


class CRedisLib:
    def __init__(self, count=None):
        self.count = count

    # public
    def opt_get(self, keys):
        tmp_r = self._conn_redis()
        ret = tmp_r.get(keys)
        return ret

    # private
    def _baisc_opt(self):
        pass

    def _conn_redis(self):
        p = redis.ConnectionPool(host='172.16.10.72', port=6379, db=0)
        r = redis.Redis(connection_pool=p)
        return r
        # pipe = r.pipeline()
        # pipe.get('zhangxiuhai')
        # return pipe.execute()

if __name__ == '__main__':
    tmp_obj = CRedisLib()
    values = tmp_obj.opt_get('zhangxiuhai')
    print values
