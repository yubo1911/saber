# -*- coding: utf-8 -*-
from functools import wraps

def func_cache(func):
	cache = {}
	@wraps(func)
	def inner_deco(*args, **kwargs):
		key = (args, frozenset(kwargs.items()))
		if key not in cache:
			print('func {} is not cached with arguments {} {}'.format(
				func.__name__, args, kwargs)) 
			res = func(*args, **kwargs)
			cache[key] = res
		return cache[key]
	return inner_deco

@func_cache
def add_two_number(a, b):
	return a + b

@func_cache
def product_two_number(a, b):
	return a * b

if __name__ == "__main__":
	print('add_two_number func name is {}'.format(add_two_number.__name__))
	print('1. add_two_number(1, 2)')
	add_two_number(1, 2)
	print(add_two_number.__name__)
	print('2. add_two_number(2, 3)')
	add_two_number(2, 3)
	print('3. add_two_number(1, b=2)')
	add_two_number(1, b=2)
	print('4. add_two_number(1, 2)')
	add_two_number(1, 2)
	print('5. product_two_number(1, 2)')
	product_two_number(1, 2)
