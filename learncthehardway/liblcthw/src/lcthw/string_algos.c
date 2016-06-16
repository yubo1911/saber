#include <lcthw/string_algos.h>
#include <limits.h>

static inline void String_setup_skip_chars(
		size_t *skip_chars,
		const unsigned char *needle, ssize_t nlen)
{
	size_t i = 0;
	size_t last = nlen - 1;

	for(i = 0; i < UCHAR_MAX + 1; i++)
	{
		skip_chars[i] = nlen;
	}

	for(i = 0; i < last; i++)
	{
		skip_chars[needle[i]] = last - i;
	}
}


