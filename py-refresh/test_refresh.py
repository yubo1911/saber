# -*- coding: utf-8 -*-
from __future__ import print_function

import os
import time
import refresh_class


version = 1.0

print(version)

rc = refresh_class.RefreshClass()
while True:
    if os.path.exists('refresh.signal'):
        reload(refresh_class)
        rc.__class__ = refresh_class.RefreshClass
    time.sleep(5)
    rc.print_info()
