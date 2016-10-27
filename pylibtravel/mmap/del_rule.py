#!/usr/bin/python
# -*- coding: utf-8 -*-
import mmap
from docopt import docopt
import logging
import re

logging.basicConfig()
logger = logging.getLogger()
logger.setLevel(logging.INFO)

fmt = '\(CSV_2_DIR\)/(\S+\.csv)'
pattern = re.compile(fmt, re.U)


def del_rule(filename, rule):
    csv_files = []
    with open(filename, 'r+') as f:
        mm = mmap.mmap(f.fileno(), 0)
        start_pos = end_pos = 0
        line = mm.readline()
        force_delete = False
        while line:
            if force_delete:
                start_pos = mm.tell() - len(line)
                end_pos = mm.tell()
                mm[start_pos: end_pos] = ' '*(end_pos-start_pos)
                force_delete = False
                logger.info('force delete...')

            elif rule in line:
                start_pos = mm.tell() - len(line)
                end_pos = mm.tell()
                split_pos = line.find(':')
                start_pos += split_pos + 1
                logger.debug('{} {}'.format(start_pos, end_pos))
                mm[start_pos: end_pos] = ' '*(end_pos-start_pos-1) + '\n'
                force_delete = True
                logger.info('delete {}'.format(rule))

                sm = pattern.search(line)
                if sm:
                    csv_files.append(sm.group(1))
            line = mm.readline()

        mm.close()

    return csv_files


if __name__ == "__main__":
    doc = """Usage:
        del_rule.py <rule> <file>
        del_rule.py (-h | --help)

    Options:
        -h --help        Show this screen
    """
    args = docopt(doc, version="del_rule ver1.0")
    csv_files = del_rule(args['<file>'], args['<rule>'])
    for fn in csv_files:
        logger.info('found csv files: {}'.format(fn))
