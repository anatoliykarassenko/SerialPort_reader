#ifndef PARSER_H
#define PARSER_H

#include "events.h"
#include "crc8.h"

static inline int min(int x, int y) {
	return x < y ? x : y;
}

class Parser
{
	uint8_t buffer[256];
	int idx;
	int size;

public:
	Parser()
	{
		idx = 0;
		size = 0;
	}
	int parse(const uint8_t *data, uint8_t len, int &received);

	const EventUnion *event()
	{
	    return (const EventUnion*)buffer;
    }
};

#endif
