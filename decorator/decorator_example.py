# -*- coding: utf-8 -*-

def func_cache(func):
	cache = {}
	def inner_deco(*args):
		if args in cache:
			print('func {} is already cached with arguments {}'.format(
				func.__name__, args))
			return cache[args]
		else:
			print('func {} is not cached with arguments {}'.format(
				func.__name__, args)) 
			res = func(*args)
			cache[args] = res
			return res
	return inner_deco

@func_cache
def add_two_number(a, b):
	return a + b

if __name__ == "__main__":
	print('1. add_two_number(1, 2)')
	add_two_number(1, 2)
	print('2. add_two_number(2, 3)')
	add_two_number(2, 3)
	print('3. add_two_number(1, 2)')
	add_two_number(1, 2)
