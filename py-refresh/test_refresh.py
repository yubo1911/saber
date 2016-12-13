# -*- coding: utf-8 -*-
from __future__ import print_function

import time
import signal

import refresh_class

rc = refresh_class.RefreshClass()


def handl_refresh(signum, frame):
    reload(refresh_class)
    rc.__class__ = refresh_class.RefreshClass


signal.signal(signal.SIGUSR1, handl_refresh)
while True:
    time.sleep(5)
    rc.print_info()
