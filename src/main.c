#include "macros.h"
#include <stdlib.h>

#define address_space 1024*1024*1024*4

i32 main(int argc, char **argv)
{

	// initialize memory space	

	u8 *memory = (u8 *) malloc (sizeof(u8) * 1024);

	// initialize registers
	u32 pc;

	u32 *x = (u32 *) malloc(sizeof(u32) * 32);
	x[0] = 0;

	// load program into memory


	// start fetch/execute/decode cycle

	u8 running = 1;

	while (running)
	{
		// fetch

		// decode

		// execute
	}

}
