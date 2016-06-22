# -*- coding: utf-8 -*-
import time
import random
def old_fib(n):
	res = [0] * n
	index = 0
	a = 0
	b = 1
	while index < n:
		res[index] = b
		a, b = b, a + b
		index += 1
	return res

def fib(n):
	index = 0
	a = 0
	b = 1
	while index < n:
		yield b
		a, b = b, a + b
		index += 1

def stupid_fib(n):
	index = 0
	a = 0
	b = 1
	while index < n:
		sleep_cnt = yield b
		print('let me think {0} secs'.format(sleep_cnt))
		time.sleep(sleep_cnt)
		a, b = b, a + b
		index += 1

def copy_fib(n):
	print('I am copy from fib')
	yield from fib(n)
	print('Copy end')

def copy_stupid_fib(n):
	print('I am copy from stupid fib')
	yield from stupid_fib(n)
	print('Copy end')

if __name__ == '__main__':
	print('-'*10 + 'test old fib' + '-'*10)
	for fib_res in old_fib(20):
		print(fib_res)
	
	print('-'*10 + 'test yield fib' + '-'*10)
	for fib_res in fib(20):
		print(fib_res)

	print('-'*10 + 'test yield send' + '-'*10)
	N = 20
	sfib = stupid_fib(N)
	fib_res = next(sfib)
	while True:
		print(fib_res)
		try:
			fib_res = sfib.send(random.uniform(0, 0.5))
		except StopIteration:
			break

	print('-'*10 + 'test yield from' + '-'*10)
	for fib_res in copy_fib(20):
		print(fib_res)
	
	print('-'*10 + 'test yield from and send' + '-'*10)
	N = 20
	csfib = copy_stupid_fib(N)
	fib_res = next(csfib)
	while True:
		print(fib_res)
		try:
			fib_res = csfib.send(random.uniform(0, 0.5))
		except StopIteration:
			break

	
