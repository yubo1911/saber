# -*- coding: utf-8 -*-
import sys
sys.path.append('.')


def test():
    import person
    p = person.Person()
    p.set_name('Qie')
    p.set_items([100, 123.456, 888.8], [0.3, 0.1, 0.5])
    print p.print_info()


if __name__ == "__main__":
    test()
