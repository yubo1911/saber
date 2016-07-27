# -*- coding: utf-8 -*-

import web
import time
import redis

r = redis.StrictRedis()

urls = (
    '/', 'visit',
    '/online', 'online'
)

app = web.application(urls, globals())


def time_to_key(current_time):
    return 'active.users:' + time.strftime('%M', time.localtime(current_time))


def keys_in_last_10_minutes():
    now = time.time()
    result = []
    for i in range(10):
        result.append(time_to_key(now - i * 60))
    return result


class visit:
    def GET(self):
        user_id = web.ctx.env['HTTP_USER_AGENT']
        current_key = time_to_key(time.time())
        pipe = r.pipeline()
        pipe.sadd(current_key, user_id)
        pipe.expire(current_key, 10 * 60)
        pipe.execute()

        return 'User:\t' + user_id + '\r\nKey:\t' + current_key


class online:
    def GET(self):
        online_users = r.sunion(keys_in_last_10_minutes())
        result = ''
        for user in online_users:
            result += 'User agent:' + user + '\r\n'
        return result


if __name__ == "__main__":
    app.run()
