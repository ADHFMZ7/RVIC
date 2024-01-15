# RVIC
RV32I core in C

## Notes

RISC-V comes in a few different base ISA's, which are the minimal set that complies to the RISC-V spec. RV32I and RV64I. The number represents the width of the address space. Extensions can be added on top of these base instruction sets to add functionality such as more math operations and atomic operations. This emulator will only implement the base 32-bit integer version(RV32I). 

For the base ISA and most of the standard extensions, RISC-V uses a 32-bit fixed-length instructions. These instructions are required to be aligned on 32-bit boundaries.

### Processor state
The RV32I Architecture has 31 general-purpose registers as well as the PC register. The 

### Instruction Formats
RISC-V has 6 different instruction formats. These formats each interpret the different fields of the 32 bits in different ways. They all reserve the first 7 bits for the opcode.
