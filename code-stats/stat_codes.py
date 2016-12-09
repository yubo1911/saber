# -*- coding: utf-8 -*-
import os
import subprocess
import shlex
import time
from os.path import join
from collections import defaultdict

ignore_paths = ['common/cdata', 'common_server/data', 'client/data', 'common_server/bt', 'common_server/Lib', 'common/Lib', 'common_server/lib-dynload-x64', 'common_server/lib-dynload-x86']

cmd_prefix = 'svn blame '

code_stats = defaultdict(int)

def stat_file(fname):
	cmd = cmd_prefix + fname
	args = shlex.split(cmd)
	with open('blame.py', 'w') as f:
		subprocess.call(args, stdout=f)
	
	with open('blame.py', 'r') as f:
		for line in f:
			args = line.split()
			if len(args) >= 2:
				author = args[1]
				if '@' in author:
					author = author.split('@')[0]
				code_stats[author] += 1


for root, dirs, files in os.walk('/home/gzguoyubo/mf/tw2/res/entities'):
	ignore = False
	for ig_path in ignore_paths:
		if ig_path in root:
			ignore = True
	if ignore:
		continue
	for fname in files:
		if not fname.endswith('.py'):
			continue
		abs_file_path = join(root, fname)
		print abs_file_path
		stat_file(abs_file_path)
		time.sleep(0.5)
		print code_stats
	
