#include "cpu.h"

void dump_registers(core *cpu)
{
	for (int i = 0; i < 32; i++)
	{
		if (i % 16 == 0) printf("\n");
		printf("%d: %d\n", i, (i32)cpu->x[i]);
	}
}

void dump_memory(core *cpu)
{
	for (int i = 0; i < 1024; i++){
		if (i % 16 == 0) printf("\n%08x:  ", i);
		printf("%02x ", cpu->memory[i]);
	}
}

u8 memory_read(core *cpu, u32 address)
{
	return cpu->memory[address];
}

void memory_write(core *cpu, u32 address, u8 value)
{		
	cpu->memory[address] = value;
}

int load_file(const char *filename, u8 **buffer, u32 *size, u32 startIndex)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) 
		{
        perror("Error opening file");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);
    *buffer = (uint8_t*)malloc(startIndex + *size);
    if (*buffer == NULL) 
		{
        perror("Memory allocation error");
        fclose(file);
        return 1;
    }
    fread(*buffer + startIndex, sizeof(uint8_t), *size, file);
    fclose(file);
    return 0;
}
