#include "parser.hpp"

#include <stdio.h>
#include <string.h>

int Parser::parse(const uint8_t *data, uint8_t len, int &received)
{
	unsigned int i = 0;

	while (i < len)
    {
		if (0 == idx)
		{
			if (0xFE == data[i])
			{
				idx ++;
			}
			i++;
		}
        else if (1 == idx)
        {
			buffer[idx] = data[i];
			size = data[i++] + 4;
			idx++;
		}
		else if (idx < size)
        {
			int l = min(size - idx, len - i);
			memcpy(buffer + idx, data + i, l);
			idx += l;
			i += l;
		}
		else
		{
			idx = 0;
			if (data[i] == crc8(0, buffer + 1, size - 1))
            {
				received = true;
			}
			else
            {
				received = false;
				printf("crc error size = %d\n", size);
			}
			i++;
			return i;
		}
	}

	return i;
}
