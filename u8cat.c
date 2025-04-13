#include "u8tools.h"

#include <ctype.h>

int main(int argc, char **argv)
{
	size_t min_len = 4;
	if (argc != 2)
	{
		printf("usage: %s filename\n", argv[0]);
		return -1;
	}

	FILE *file = fopen(argv[argc-1], "rb");
	if (!file)
	{
		printf("error: file not found\n");
		return -2;
	}

	u8char_t c;
	while(u8_fread(&c, file))
		u8_print(c);

	fclose(file);
	return 0;
}
