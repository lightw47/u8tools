#ifndef __U8_TOOLS_H__
#define __U8_TOOLS_H__

#include <stdint.h>
#include <stdio.h>

typedef uint32_t u8char_t;

uint8_t u8_length(char *s);
u8char_t u8_load(char *p_char);
int u8_isvalid(u8char_t c);
int u8_fread(u8char_t *c, FILE *file);
void u8_print(u8char_t c);

#endif
