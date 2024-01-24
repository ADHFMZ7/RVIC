#include "macros.h"
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define ADDRESS_SPACE 1024*1024*1024*4
#define MEM_BASE      0x800000000

// TODO:
// - Finish implementing System instructions
// - Implement MMU
//
// Goals far down the line:
// - Add pipelining
// - Add additional extensions
// - Create a debugger
// - Create an assembler/dissassembler

u32 extract_bits(u32 instruction, u8 start_bit, u8 num_bits) {
	u32 bitmask = ((1U << num_bits) - 1) << start_bit;
	return (instruction & bitmask) >> start_bit;
}

i32 main(i32 argc, byte **argv)
{

	core cpu;

	// initialize memory space	
	cpu.memory = (u8 *) malloc (sizeof(u8) * 1024);
	for (int i = 0; i < 1024; ++i) memory_write(&cpu, i, 0);

	// initialize registers
	cpu.pc = 0x100;

	cpu.x = (u32 *) malloc(sizeof(u32) * 32);
	cpu.x[0] = 0;
	
	// TESTING
	for (int i = 0; i < 32; ++i) cpu.x[i] = 1;
	cpu.x[0] = 0;

	// load program into memory
	u32 size;
	load_file("tests/test.bin", &cpu.memory, &size, 0x100);

	// Make MMU so I dont have to allocate the entire addressable space

	// start fetch/execute/decode cycle

	u8 running = 1;

	while (--running)
	{
		// Make sure zero register is maintained.
		cpu.x[0] = 0;

		// Fetch
		u32 instruction = 0;
		for (int i = 0; i < 4; ++i) instruction += memory_read(&cpu, cpu.pc++) << (8 * i);

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
		if (opcode == 0b0110011) // OP
		{
			if (funct3 == 0x0 && funct7 == 0x0)        // ADD 
				cpu.x[rd] = cpu.x[rs1] + cpu.x[rs2];
			else if (funct3 == 0x0 && funct7 == 0x20)  // SUB
				cpu.x[rd] = cpu.x[rs1] - cpu.x[rs2];	
			else if (funct3 == 0x7 && funct7 == 0x0)   // AND
				cpu.x[rd] = cpu.x[rs1] & cpu.x[rs2];	
			else if (funct3 == 0x6 && funct7 == 0x0) 	 // OR
				cpu.x[rd] = cpu.x[rs1] | cpu.x[rs2];	
			else if (funct3 == 0x4 && funct7 == 0x0) 	 // XOR
				cpu.x[rd] = cpu.x[rs1] ^ cpu.x[rs2];	
			else if (funct3 == 0x1 && funct7 == 0x0) 	 // SLL
				cpu.x[rd] = cpu.x[rs1] << cpu.x[rs2];	
			else if (funct3 == 0x5 && funct7 == 0x0) 	 // SRL
				cpu.x[rd] = cpu.x[rs1] >> cpu.x[rs2];	
			else if (funct3 == 0x5 && funct7 == 0x20)  // SRA
				cpu.x[rd] = ((i32)cpu.x[rs1]) >> cpu.x[rs2];	
			else if (funct3 == 0x2 && funct7 == 0x0)   // SLT 
				cpu.x[rd] = (cpu.x[rs1] < cpu.x[rs2]) ? 0 : 1;	
			else if (funct3 == 0x3 && funct7 == 0x0)   // SLTU
				cpu.x[rd] = ((i32)cpu.x[rs1] < (i32)cpu.x[rs2]) ? 0 : 1;	
		}

		else if (opcode == 0b0010011) // 
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
			cpu.x[rd] = 0;
			if (funct3 == 0x0) 												// LB 
				cpu.x[rd] = (i32) memory_read(&cpu, rs1 + imm);
			else if (funct3 == 0x4)										// LBU TODO
				cpu.x[rd] = (u32) memory_read(&cpu, rs1 + imm);
			else if (funct3 == 0x1)										// LH TODO
			{
				for (int i = 0; i < 2; ++i) cpu.x[rd] += memory_read(&cpu, rs1 + imm) << (8 * i);
			}
			else if (funct3 == 0x5)										// LHU TODO
			{
				for (int i = 0; i < 2; ++i) cpu.x[rd] += memory_read(&cpu, rs1 + imm) << (8 * i);
				
			}
			else if (funct3 == 0x2)										// LW
				for (int i = 0; i < 4; ++i) cpu.x[rd] += memory_read(&cpu, rs1 + imm) << (8 * i);
		}
		else if (opcode == 0b0100011) // S Format (STORE)
		{
			if (funct3 == 0x2)												// SW	
				for(int i = 0; i < 4; ++i) memory_write(&cpu, rs1+imm+i, (u8)((rs2 >> (8 * i)) & 0xFF));
		}
		else if (opcode == 0b1100011)	// B Format (BRANCH)
		{
			if (funct3 == 0x0) 												// BEQ
				if (cpu.x[rs1] == cpu.x[rs2])
					cpu.pc += imm;
			if (funct3 == 0x1) 												// BNE 
				if (cpu.x[rs1] != cpu.x[rs2])
					cpu.pc += imm;
			if (funct3 == 0x4) 												// BLT
				if (cpu.x[rs1] < cpu.x[rs2])
					cpu.pc += imm;
			if (funct3 == 0x5) 												// BGE
				if (cpu.x[rs1] >= cpu.x[rs2])
					cpu.pc += imm;
			if (funct3 == 0x6) 												// BLTU
				if (cpu.x[rs1] < cpu.x[rs2])
					cpu.pc += imm;
			if (funct3 == 0x7) 												// BGEU
				if (cpu.x[rs1] >= cpu.x[rs2])
					cpu.pc += imm;
		}
		else if (opcode == 0b1101111)								// JAL
		{
			cpu.x[rd] = cpu.pc + 4;
			cpu.pc += imm;
		}
		else if (opcode == 0b01100111)							// JALR
		{
			cpu.x[rd] = cpu.pc + 4;
			cpu.pc += cpu.x[rs1] + imm;
		}
		else if (opcode == 0b0110111)								// LUI
		{
			cpu.x[rd] = imm << 12;
		}
		else if (opcode == 0b0010111)								// AUIPC
		{
			cpu.x[rd] = cpu.pc + (imm << 12);
		}
		else if (opcode == 0b1110011) // (SYSTEM)
		{
			if (funct3 == 0x0)	
			{

			}

		}
		else 
		{
			printf("UNKNOWN INSTRUCTION: %x\n", instruction);
			exit(EXIT_FAILURE);
		}
	}
	dump_registers(&cpu);
	dump_memory(&cpu);
	printf("\n");
}
