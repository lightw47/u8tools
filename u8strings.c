#include "u8tools.h"

#include <ctype.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	size_t min_len = 4;
	if (argc < 2)
	{
		printf("usage: %s [-min-len] filename\n", argv[0]);
		return -1;
	}
	if (argc == 3)
	{
		min_len = -atoi(argv[1]);
		printf("%ld ", min_len);
	}

	FILE *file = fopen(argv[argc-1], "rb");
	if (!file)
	{
		printf("error: file not found\n");
		return -2;
	}

	u8char_t c;
	u8char_t *init;
	int i = 0;
	int j;

	init = (u8char_t *) malloc(min_len * sizeof(u8char_t));

	while (u8_fread(&c, file))
	{
		if (i < min_len)
		{
			if ((c > 0x7f && u8_isvalid(c)) || (c <= 0x7f && isprint(c)))
			{
				init[i] = c;
				++i;
				if (i == min_len)
					for (j = 0; j < min_len; ++j)
						u8_print(init[j]);
			}
			else
				i = 0;
		}
		else
		{
			if ((c > 0x7f & u8_isvalid(c)) || (c <= 0x7f && isprint(c)))
			{
				++i;
				u8_print(c);
			}
			else
			{
				i = 0;
				printf("\n");
			}
		}
	}

	free(init);
	fclose(file);
	return 0;
}
