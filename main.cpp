#include "i8080.h"

void i8080_debug_output( i8080* const c ) {
	uint8_t f = c->get_register( i8080::RG_F ) & 0xFF;

	printf( "PC: %04X, AF: %04X, BC: %04X, DE: %04X, HL: %04X, SP: %04X, CYC: %lu",
					c->PC, c->A << 8 | f, c->get_register( i8080::RP_BC ), c->get_register( i8080::RP_DE ), c->get_register( i8080::RP_HL ),
					c->SP, c->cycles );

	printf( "\t(%02X %02X %02X %02X)", c->read_byte( c->PC ), c->read_byte( c->PC + 1 ),
					c->read_byte( c->PC + 2 ), c->read_byte( c->PC + 3 ) );

	printf( "\n" );
}

void run_test( i8080* c, const char* filename, unsigned long cyc_expected ) {
	c->reset();

	if ( !c->load_rom( filename, 0x0100 ) ) {
		return;
	}

	printf( "*** TEST: %s\n", filename );

	c->PC = 0x0100;

	// inject "out 0,a" at 0x0000 (signal to stop the test)
	c->memory[ 0x0000 ] = 0xD3;
	c->memory[ 0x0001 ] = 0x00;

	// inject "out 1,a" at 0x0005 (signal to output some characters)
	c->memory[ 0x0005 ] = 0xD3;
	c->memory[ 0x0006 ] = 0x01;
	c->memory[ 0x0007 ] = 0xC9;

	long nb_instructions = 0;

	bool test_finished = false;
	while ( !test_finished ) {
		//i8080_debug_output( c );
		c->step();
		nb_instructions += 1;

		if ( c->halted ) {
			test_finished = true;
		}
	}

	long long diff = cyc_expected - c->cycles;
	printf( "\n*** %lu instructions executed on %lu cycles"
					" (expected=%lu, diff=%lld)\n\n",
					nb_instructions, c->cycles, cyc_expected, diff );
}

int main( int argc, char** argv ) {

	i8080 cpu;
	
	//run_test( &cpu, ".\\roms\\TST8080.COM", 4924LU );
	//run_test( &cpu, ".\\roms\\CPUTEST.COM", 255653383LU );
	//run_test( &cpu, ".\\roms\\8080PRE.COM", 7817LU );
	run_test( &cpu, ".\\roms\\8080EXM.COM", 23803381171LU );

	return 0;
}