#include "u8tools.h"

uint8_t u8_length(char *s)
{
	switch ((s[0] & 0xFF) >> 4)
	{
		case 0x0: return 1;
		case 0x1: return 1;
		case 0x2: return 1;
		case 0x3: return 1;
		case 0x4: return 1;
		case 0x5: return 1;
		case 0x6: return 1;
		case 0x7: return 1;
		case 0x8: return 0;
		case 0x9: return 0;
		case 0xA: return 0;
		case 0xB: return 0;
		case 0xC: return 2;
		case 0xD: return 2;
		case 0xE: return 3;
		case 0xF: return 4;
	}
	return 0xFF;
}

u8char_t u8_load(char *p_char)
{
	u8char_t c = 0;
	uint8_t len = u8_length(p_char);
	uint8_t i;

	for (i = 0; i < len && p_char[i] != '\0'; ++i)
		c = (c << 8) | (p_char[i] & 0xff);

	return c;
}

int u8_isvalid(u8char_t c)
{
	// ascii characters
	if (c <= 0x7f)
		return 1;

	// U+0xyz => 110xxxyy 10yyzzzz
	// U+0080 => 11000010 10000000 => C2 80
	// U+07FF => 11011111 10111111 => DF BF
	if (0xC280 <= c && c <= 0xDFBF)
		return ((c & 0xE0C0) == 0xC080);

	// exclude surrogates
	// U+wxyz => 1110wwww 10xxxxyy 10yyzzzz
	// U+D800 => 11101101 10100000 10000000 => ED A0 80
	// U+DFFF => 11101101 10111111 10111111 => ED BF BF
	if (0xEDA080 <= c && c <= 0xEDBFBF)
		return 0;

	// same rule as above
	// U+0800 => 11100000 10100000 10000000 => E0 A0 80
	// U+FFFF => 11101111 10111111 10111111 => EF BF BF
	if (0xE0A080 <= c && c <= 0xEFBFBF)
		return ((c & 0xF0C0C0) == 0xE08080);

	// U+uvwxyz => 11110uvv 10vvwwww 10xxxxyy 10yyzzzz
	// U+010000 => 11110000 10010000 10000000 10000000 => F0 90 80 80
	// U+10FFFF => 11110100 10001111 10111111 10111111 => F4 8F BF BF
	if (0xF0908080 <= c && c <= 0xF48FBFBF)
		return ((c & 0xF8C0C0C0) == 0xF0808080);

	return 0;
}

int u8_fread(u8char_t *c, FILE *file)
{
	char ch;
	if (!fread(&ch, sizeof(ch), 1, file))
		return 0;

	*c = ch & 0xff;

	uint8_t len = u8_length(&ch) - 1;
	int i;
	for (i = 0; i < len; ++i)
	{
		if (!fread(&ch, sizeof(ch), 1, file))
			return 0;

		*c = ((*c) << 8) | (ch & 0xff);
	}

	return 1;
}

void u8_print(u8char_t c)
{
	if (u8_isvalid(c))
	{
		if (c <= 0x7f)
			printf("%c", c);
		if (0xC280 <= c && c <= 0xDFBF)
			printf("%c%c", (c & 0xFF00) >> 8, c & 0xFF);
		if (0xE0A080 <= c && c <= 0xEFBFBF)
			printf("%c%c%c", (c & 0xFF0000) >> 16, (c & 0xFF00) >> 8, c & 0xFF);
		if (0xF0908080 <= c && c <= 0xF48FBFBF)
			printf("%c%c%c%c", (c & 0xFF000000) >> 24, (c & 0xFF0000) >> 16, (c & 0xFF00) >> 8, c & 0xFF);
	}
	else
		// U+FFFD => 11101111 10111111 10111101 => EF BF BD
		printf("\xEF\xBF\xED");

}
