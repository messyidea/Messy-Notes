#include "lcd1602.h"

void lcdinit()
{
	lcdWriteCmd(0x38);
}

bit lcdWriteCmd(unsigned char cmd)
{
	return (1);
}