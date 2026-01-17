#include "i8080.h"

int main( int argc, char** argv ) {
	
	const char* path = ".\\roms\\TST8080.COM";

	i8080 cpu;
	
	// Load rom into memory
	if ( !cpu.load_rom( path, 0x0100 ) ) {
		return 1;
	}

  /* FOR TEST ROMS ONLY */
  // Set PC to starting point
  cpu.PC = 0x0100;

  // inject "out 0,a" at 0x0000 (signal to stop the test)
  cpu.memory[ 0x0000 ] = 0xD3;
  cpu.memory[ 0x0001 ] = 0x00;

  // inject "out 1,a" at 0x0005 (signal to output some characters)
  cpu.memory[ 0x0005 ] = 0xD3;
  cpu.memory[ 0x0006 ] = 0x01;
  cpu.memory[ 0x0007 ] = 0xC9;

	// Loop until CPU is halted
	bool running = true;
	while ( running ) {
		// Emulate instruction
		cpu.step();
		//cpu.log();

		if ( cpu.error || cpu.halted ) {
			running = false;
		}
	}

	return 0;
}