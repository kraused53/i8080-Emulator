#include "i8080.h"

int main( int argc, char** argv ) {
	
	i8080 cpu;

	return 0;

	bool running = true;
	while ( running ) {
		cpu.step();
		if ( cpu.error || cpu.halted ) {
			running = false;
		}
	}

	return 0;
}