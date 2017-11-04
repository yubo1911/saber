# -*- coding: utf-8 -*-
import sys
sys.path.append('.')


def test():
    import hello_boostpy
    return hello_boostpy.greet()


if __name__ == "__main__":
    print test()
