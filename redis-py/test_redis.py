# -*- coding: utf-8 -*-
import redis

r = redis.StrictRedis(host='127.0.0.1', port=6379, db=0)
r.set('foo', 'bar')
print r.get('foo')

r.hmset('dict', {'name': 'Bob'})
people = r.hgetall('dict')
print people

pipe = r.pipeline()
pipe.set('foo', 'bar')
pipe.get('foo')
result = pipe.execute()
print result
