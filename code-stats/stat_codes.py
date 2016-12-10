# -*- coding: utf-8 -*-
import os
import subprocess
import shlex
import random
from os.path import join
from collections import defaultdict
from multiprocessing.dummy import Process, Queue

ignore_paths = ['common/cdata', 'common_server/data', 'client/data', 'common_server/bt', 'common_server/Lib', 'common/Lib', 'common_server/lib-dynload-x64', 'common_server/lib-dynload-x86']

P_NUM = 8

cmd_prefix = 'svn blame '

def stat_file(fnames, res_queue):
	code_stats = defaultdict(int)
	for fname in fnames:
		print 'processing ', fname
		cmd = cmd_prefix + fname
		args = shlex.split(cmd)
		blame_file_name = 'blame_' + os.path.basename(fname) + str(random.randint(0, 100)) + '.py'
		with open(blame_file_name, 'w') as f:
			subprocess.call(args, stdout=f)
		
		with open(blame_file_name, 'r') as f:
			for line in f:
				args = line.split()
				if len(args) >= 2:
					author = args[1]
					if '@' in author:
						author = author.split('@')[0]
					code_stats[author] += 1
		os.remove(blame_file_name)

	res_queue.put(code_stats)
	
def stat_files():
	all_files = []
	for root, dirs, files in os.walk('/home/gzguoyubo/mf/tw2/res/entities/custom_type'):
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
			all_files.append(abs_file_path)
	
	file_sections = []
	file_total_nums = len(all_files)
	for i in xrange(P_NUM):
		start = i * file_total_nums / P_NUM
		stop = start + file_total_nums / P_NUM
		if i == P_NUM - 1:
			stop = -1
		file_sections.append(all_files[start : stop])

	res_queue = Queue()
	processes = []
	for section in file_sections:
		p = Process(target=stat_file, args=(section, res_queue))
		p.start()
		processes.append(p)
	
	for p in processes:
		p.join()
	
	total_stats = defaultdict(int)
	while not res_queue.empty():
		stat = res_queue.get()
		for author, cnt in stat.iteritems():
			total_stats[author] += cnt
	
	print total_stats
	

if __name__ == '__main__':
	stat_files()
	
