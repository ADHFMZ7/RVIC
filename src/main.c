#include "macros.h"
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define address_space 1024*1024*1024*4

// TODO:
// - Finish implementing instructions
// - Implement MMU
// - Think about optable with func pointers approach
//
// Goals far down the line:
// - Add pipelining
// - Add additional extensions
// - Create a debugger
// - Create an assembler/dissassembler

int load_file(const char *filename, uint8_t **buffer, size_t *size, size_t startIndex) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);
    *buffer = (uint8_t*)malloc(startIndex + *size);
    if (*buffer == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return 1;
    }
    fread(*buffer + startIndex, sizeof(uint8_t), *size, file);
    fclose(file);
    return 0;
}

u32 extract_bits(u32 instruction, u8 start_bit, u8 num_bits) {
	u32 bitmask = ((1U << num_bits) - 1) << start_bit;
	return (instruction & bitmask) >> start_bit;
}

i32 main(i32 argc, byte **argv)
{

	core cpu;

	// initialize memory space	
	cpu.memory = (u8 *) malloc (sizeof(u8) * 1024);
	for (int i = 0; i < 1024; i++) cpu.memory[i]=0;

	// initialize registers
	cpu.pc = 0x100;

	cpu.x = (u32 *) malloc(sizeof(u32) * 32);
	cpu.x[0] = 0;

	// TESTING
	for (int i = 0; i < 32; i++) cpu.x[i] = 1;
	cpu.x[0] = 0;

	// load program into memory
	size_t size;
	load_file("tests/test.bin", &cpu.memory, &size, 0x100);

	// Make MMU so I dont have to allocate the entire addressable space

	// start fetch/execute/decode cycle

	u8 running = 1;

	while (running--)
	{
		// Make sure zero register is maintained.
		cpu.x[0] = 0;

		// Fetch
		u32 instruction = 0;
		for (int i = 0; i < 4; i++) instruction += cpu.memory[cpu.pc++] << (8 * i);

		// printf("%08x AT PC: %x\n", instruction, cpu.pc);
		printf("%08x\n", instruction);
			
		// Decode
		u8 opcode = extract_bits(instruction, 0, 7);
		u8 funct3 = extract_bits(instruction, 12, 3);
		u8 funct7 = extract_bits(instruction, 25, 7);

		u8 rd     = extract_bits(instruction, 7, 5);
		u8 rs1    = extract_bits(instruction, 15, 5);
		u8 rs2    = extract_bits(instruction, 20, 5);

		u16 imm   = extract_bits(instruction, 20, 12);
		u8 imm_2  = extract_bits(instruction, 20, 5);

		// Execute	
		if (opcode == 0b0110011) // R Type 
		{
			if (funct3 == 0x0 && funct7 == 0x0)       // ADD 
				cpu.x[rd] = cpu.x[rs1] + cpu.x[rs2];
			else if (funct3 == 0x0 && funct7 == 0x20) // SUB
				cpu.x[rd] = cpu.x[rs1] - cpu.x[rs2];	
			else if (funct3 == 0x7 && funct7 == 0x0)  // AND
				cpu.x[rd] = cpu.x[rs1] & cpu.x[rs2];	
			else if (funct3 == 0x6 && funct7 == 0x0)  // OR
				cpu.x[rd] = cpu.x[rs1] | cpu.x[rs2];	
			else if (funct3 == 0x4 && funct7 == 0x0)  // XOR
				cpu.x[rd] = cpu.x[rs1] ^ cpu.x[rs2];	
			else if (funct3 == 0x1 && funct7 == 0x0)  // SLL
				cpu.x[rd] = cpu.x[rs1] << cpu.x[rs2];	
			else if (funct3 == 0x5 && funct7 == 0x0)  // SRL
				cpu.x[rd] = cpu.x[rs1] >> cpu.x[rs2];	
			else if (funct3 == 0x5 && funct7 == 0x20)  // SRA
				cpu.x[rd] = ((i32)cpu.x[rs1]) >> cpu.x[rs2];	
			else if (funct3 == 0x2 && funct7 == 0x0)  // SLT 
				cpu.x[rd] = (cpu.x[rs1] < cpu.x[rs2]) ? 0 : 1;	
			else if (funct3 == 0x3 && funct7 == 0x0)  // SLTU
				cpu.x[rd] = ((i32)cpu.x[rs1] < (i32)cpu.x[rs2]) ? 0 : 1;	
		}

		else if (opcode == 0b0010011) // I format 
		{
			if (funct3 == 0x0)												 // ADDI
			{cpu.x[rd] = cpu.x[rs1] + imm;	
			printf("%d = %d + %d\n", cpu.x[rd], cpu.x[rd], cpu.x[rs1]);}
			else if (funct3 == 0x7)										 // ANDI
				cpu.x[rd] = cpu.x[rs1] & imm;	
			else if (funct3 == 0x6)										 // ORI 
				cpu.x[rd] = cpu.x[rs1] | imm;	
			else if (funct3 == 0x4)										 // XORI
				cpu.x[rd] = cpu.x[rs1] ^ imm;	
			else if (funct3 == 0x1)										 // SLLI
				cpu.x[rd] = cpu.x[rs1]  << imm_2;
			else if (funct3 == 0x5 && funct7 == 0x0)   // SRLI 
				cpu.x[rd] = cpu.x[rs1]  >> imm_2;
			else if (funct3 == 0x5 && funct7 == 0x20)  // SRAI 
				cpu.x[rd] = (i32) cpu.x[rs1]  >> imm_2;
			else if (funct3 == 0x5)										 // SLTI 
				cpu.x[rd] = (cpu.x[rs1] < imm) ? 1 : 0;
			else if (funct3 == 0x5)										 // SLTIU	 
				cpu.x[rd] = ((i32)cpu.x[rs1] < (i32) imm) ? 1 : 0;
		}
		else if (opcode == 0b0000011) // I format load
		{
			if (funct3 == 0x0) 												// LB
				cpu.x[rd] = 1;
			else if (funct3 == 0x4)										// LBU
				cpu.x[rd] = 1;
			else if (funct3 == 0x1)										// LH
				cpu.x[rd] = 1;
			else if (funct3 == 0x5)										// LHU
				cpu.x[rd] = 1;
			else if (funct3 == 0x2)										// LW
				for (int i = 0; i < 4; i++) cpu.x[rd] = cpu.memory[rs1 + imm] << (8 * i);
		}
		else if (opcode == 0b0100011) // S Format (STORE)
		{


			if (funct3 == 0x2)												// SW	
				for(int i = 0; i < 4; i++) cpu.memory[rs1+imm+i] = (u8)((rs2 >> (8 * i)) & 0xFF);;
		}
		
		// execute

	}
	dump_registers(&cpu);
	// for (int i = 0; i < 1024; i++)
	// 	if (cpu.memory[i] != 0)
	// 		printf("MEMORY %x: %x\n", i, cpu.memory[i]);
}
