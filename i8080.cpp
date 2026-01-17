#include "i8080.h"

#include <format>
#include "Logger.h"

#include <iostream>

i8080::i8080( void ) {
	reset();

	// 
	memory.resize( 64 * 1024 );
	clear_memory();

	using i = i8080;
	/* { &i::OP-FUNC, AM, DST, SRC COND } */
	op_codes = {
		// 0x00 -> 0x0F
		{ &i::NOP, RG_NONE, RG_NONE, CN_NONE },
		{ &i::LXI,   RP_BC, RG_NONE, CN_NONE },
		{ &i::STAX,RG_NONE,   RP_BC, CN_NONE },
		{ &i::INX,   RP_BC, RG_NONE, CN_NONE },
		{ &i::INR,    RG_B, RG_NONE, CN_NONE },
		{ &i::DCR,    RG_B, RG_NONE, CN_NONE },
		{ &i::MVI,    RG_B, RG_NONE, CN_NONE },
		{ &i::RLC, RG_NONE, RG_NONE, CN_NONE },
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::DAD,   RP_HL,   RP_BC, CN_NONE },
		{ &i::LDAX,RG_NONE,   RP_BC, CN_NONE },
		{ &i::DCX,   RP_BC, RG_NONE, CN_NONE },
		{ &i::INR,    RG_C, RG_NONE, CN_NONE },
		{ &i::DCR,    RG_C, RG_NONE, CN_NONE },
		{ &i::MVI,    RG_C, RG_NONE, CN_NONE },
		{ &i::RRC, RG_NONE, RG_NONE, CN_NONE },

		// 0x10 -> 0x1F
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::LXI,   RP_DE, RG_NONE, CN_NONE },
		{ &i::STAX,RG_NONE,   RP_DE, CN_NONE },
		{ &i::INX,   RP_DE, RG_NONE, CN_NONE },
		{ &i::INR,    RG_D, RG_NONE, CN_NONE },
		{ &i::DCR,    RG_D, RG_NONE, CN_NONE },
		{ &i::MVI,    RG_D, RG_NONE, CN_NONE },
		{ &i::RAL, RG_NONE, RG_NONE, CN_NONE },
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::DAD,   RP_HL,   RP_DE, CN_NONE },
		{ &i::LDAX,RG_NONE,   RP_DE, CN_NONE },
		{ &i::DCX,   RP_DE, RG_NONE, CN_NONE },
		{ &i::INR,    RG_E, RG_NONE, CN_NONE },
		{ &i::DCR,    RG_E, RG_NONE, CN_NONE },
		{ &i::MVI,    RG_E, RG_NONE, CN_NONE },
		{ &i::RAR, RG_NONE, RG_NONE, CN_NONE },

		// 0x20 -> 0x2F
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::LXI,   RP_HL, RG_NONE, CN_NONE },
		{ &i::SHLD,RG_NONE, RG_NONE, CN_NONE },
		{ &i::INX,   RP_HL, RG_NONE, CN_NONE },
		{ &i::INR,    RG_H, RG_NONE, CN_NONE },
		{ &i::DCR,    RG_H, RG_NONE, CN_NONE },
		{ &i::MVI,    RG_H, RG_NONE, CN_NONE },
		{ &i::DAA, RG_NONE, RG_NONE, CN_NONE },
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::DAD,   RP_HL,   RP_HL, CN_NONE },
		{ &i::LHLD,RG_NONE, RG_NONE, CN_NONE },
		{ &i::DCX,   RP_HL, RG_NONE, CN_NONE },
		{ &i::INR,    RG_L, RG_NONE, CN_NONE },
		{ &i::DCR,    RG_L, RG_NONE, CN_NONE },
		{ &i::MVI,    RG_L, RG_NONE, CN_NONE },
		{ &i::CMA, RG_NONE, RG_NONE, CN_NONE },

		// 0x30 -> 0x3F
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::LXI,   RP_SP, RG_NONE, CN_NONE },
		{ &i::STAX,RG_NONE, RG_NONE, CN_NONE },
		{ &i::INX,   RP_SP, RG_NONE, CN_NONE },
		{ &i::INR, RG_NONE, RG_NONE, CN_NONE },
		{ &i::DCR, RG_NONE, RG_NONE, CN_NONE },
		{ &i::MVI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::STC, RG_NONE, RG_NONE, CN_NONE },
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::DAD,   RP_HL,   RP_SP, CN_NONE },
		{ &i::LDAX,RG_NONE, RG_NONE, CN_NONE },
		{ &i::DCX,   RP_SP, RG_NONE, CN_NONE },
		{ &i::INR,    RG_A, RG_NONE, CN_NONE },
		{ &i::DCR,    RG_A, RG_NONE, CN_NONE },
		{ &i::MVI,    RG_A, RG_NONE, CN_NONE },
		{ &i::CMC, RG_NONE, RG_NONE, CN_NONE },

		// 0x40 -> 0x4F
		{ &i::MOV,    RG_B,    RG_B, CN_NONE },
		{ &i::MOV,    RG_B,    RG_C, CN_NONE },
		{ &i::MOV,    RG_B,    RG_D, CN_NONE },
		{ &i::MOV,    RG_B,    RG_E, CN_NONE },
		{ &i::MOV,    RG_B,    RG_H, CN_NONE },
		{ &i::MOV,    RG_B,    RG_L, CN_NONE },
		{ &i::MOV,    RG_B, RG_NONE, CN_NONE },
		{ &i::MOV,    RG_B,    RG_A, CN_NONE },
		{ &i::MOV,    RG_C,    RG_B, CN_NONE },
		{ &i::MOV,    RG_C,    RG_C, CN_NONE },
		{ &i::MOV,    RG_C,    RG_D, CN_NONE },
		{ &i::MOV,    RG_C,    RG_E, CN_NONE },
		{ &i::MOV,    RG_C,    RG_H, CN_NONE },
		{ &i::MOV,    RG_C,    RG_L, CN_NONE },
		{ &i::MOV,    RG_C, RG_NONE, CN_NONE },
		{ &i::MOV,    RG_C,    RG_A, CN_NONE },

		// 0x50 -> 0x5F
		{ &i::MOV,    RG_D,    RG_B, CN_NONE },
		{ &i::MOV,    RG_D,    RG_C, CN_NONE },
		{ &i::MOV,    RG_D,    RG_D, CN_NONE },
		{ &i::MOV,    RG_D,    RG_E, CN_NONE },
		{ &i::MOV,    RG_D,    RG_H, CN_NONE },
		{ &i::MOV,    RG_D,    RG_L, CN_NONE },
		{ &i::MOV,    RG_D, RG_NONE, CN_NONE },
		{ &i::MOV,    RG_D,    RG_A, CN_NONE },
		{ &i::MOV,    RG_E,    RG_B, CN_NONE },
		{ &i::MOV,    RG_E,    RG_C, CN_NONE },
		{ &i::MOV,    RG_E,    RG_D, CN_NONE },
		{ &i::MOV,    RG_E,    RG_E, CN_NONE },
		{ &i::MOV,    RG_E,    RG_H, CN_NONE },
		{ &i::MOV,    RG_E,    RG_L, CN_NONE },
		{ &i::MOV,    RG_E, RG_NONE, CN_NONE },
		{ &i::MOV,    RG_E,    RG_A, CN_NONE },

		// 0x60 -> 0x6F
		{ &i::MOV,    RG_H,    RG_B, CN_NONE },
		{ &i::MOV,    RG_H,    RG_C, CN_NONE },
		{ &i::MOV,    RG_H,    RG_D, CN_NONE },
		{ &i::MOV,    RG_H,    RG_E, CN_NONE },
		{ &i::MOV,    RG_H,    RG_H, CN_NONE },
		{ &i::MOV,    RG_H,    RG_L, CN_NONE },
		{ &i::MOV,    RG_H, RG_NONE, CN_NONE },
		{ &i::MOV,    RG_H,    RG_A, CN_NONE },
		{ &i::MOV,    RG_L,    RG_B, CN_NONE },
		{ &i::MOV,    RG_L,    RG_C, CN_NONE },
		{ &i::MOV,    RG_L,    RG_D, CN_NONE },
		{ &i::MOV,    RG_L,    RG_E, CN_NONE },
		{ &i::MOV,    RG_L,    RG_H, CN_NONE },
		{ &i::MOV,    RG_L,    RG_L, CN_NONE },
		{ &i::MOV,    RG_L, RG_NONE, CN_NONE },
		{ &i::MOV,    RG_L,    RG_A, CN_NONE },

		// 0x70 -> 0x7F
		{ &i::MOV, RG_NONE,    RG_B, CN_NONE }, // r -> m
		{ &i::MOV, RG_NONE,    RG_C, CN_NONE },
		{ &i::MOV, RG_NONE,    RG_D, CN_NONE },
		{ &i::MOV, RG_NONE,    RG_E, CN_NONE },
		{ &i::MOV, RG_NONE,    RG_H, CN_NONE },
		{ &i::MOV, RG_NONE,    RG_L, CN_NONE },
		{ &i::HLT, RG_NONE, RG_NONE, CN_NONE },
		{ &i::MOV, RG_NONE,    RG_A, CN_NONE },
		{ &i::MOV,    RG_A,    RG_B, CN_NONE },
		{ &i::MOV,    RG_A,    RG_C, CN_NONE },
		{ &i::MOV,    RG_A,    RG_D, CN_NONE },
		{ &i::MOV,    RG_A,    RG_E, CN_NONE },
		{ &i::MOV,    RG_A,    RG_H, CN_NONE },
		{ &i::MOV,    RG_A,    RG_L, CN_NONE },
		{ &i::MOV,    RG_A, RG_NONE, CN_NONE },
		{ &i::MOV,    RG_A,    RG_A, CN_NONE },

		// 0x80 -> 0x8F
		{ &i::ADD, RG_NONE,    RG_B, CN_NONE },
		{ &i::ADD, RG_NONE,    RG_C, CN_NONE },
		{ &i::ADD, RG_NONE,    RG_D, CN_NONE },
		{ &i::ADD, RG_NONE,    RG_E, CN_NONE },
		{ &i::ADD, RG_NONE,    RG_H, CN_NONE },
		{ &i::ADD, RG_NONE,    RG_L, CN_NONE },
		{ &i::ADD, RG_NONE, RG_NONE, CN_NONE },
		{ &i::ADD, RG_NONE,    RG_A, CN_NONE },
		{ &i::ADC, RG_NONE,    RG_B, CN_NONE },
		{ &i::ADC, RG_NONE,    RG_C, CN_NONE },
		{ &i::ADC, RG_NONE,    RG_D, CN_NONE },
		{ &i::ADC, RG_NONE,    RG_E, CN_NONE },
		{ &i::ADC, RG_NONE,    RG_H, CN_NONE },
		{ &i::ADC, RG_NONE,    RG_L, CN_NONE },
		{ &i::ADC, RG_NONE, RG_NONE, CN_NONE },
		{ &i::ADC, RG_NONE,    RG_A, CN_NONE },

		// 0x90 -> 0x9F
		{ &i::SUB, RG_NONE,    RG_B, CN_NONE },
		{ &i::SUB, RG_NONE,    RG_C, CN_NONE },
		{ &i::SUB, RG_NONE,    RG_D, CN_NONE },
		{ &i::SUB, RG_NONE,    RG_E, CN_NONE },
		{ &i::SUB, RG_NONE,    RG_H, CN_NONE },
		{ &i::SUB, RG_NONE,    RG_L, CN_NONE },
		{ &i::SUB, RG_NONE, RG_NONE, CN_NONE },
		{ &i::SUB, RG_NONE,    RG_A, CN_NONE },
		{ &i::SBC, RG_NONE,    RG_B, CN_NONE },
		{ &i::SBC, RG_NONE,    RG_C, CN_NONE },
		{ &i::SBC, RG_NONE,    RG_D, CN_NONE },
		{ &i::SBC, RG_NONE,    RG_E, CN_NONE },
		{ &i::SBC, RG_NONE,    RG_H, CN_NONE },
		{ &i::SBC, RG_NONE,    RG_L, CN_NONE },
		{ &i::SBC, RG_NONE, RG_NONE, CN_NONE },
		{ &i::SBC, RG_NONE,    RG_A, CN_NONE },

		// 0xA0 -> 0xAF
		{ &i::ANA, RG_NONE,    RG_B, CN_NONE },
		{ &i::ANA, RG_NONE,    RG_C, CN_NONE },
		{ &i::ANA, RG_NONE,    RG_D, CN_NONE },
		{ &i::ANA, RG_NONE,    RG_E, CN_NONE },
		{ &i::ANA, RG_NONE,    RG_H, CN_NONE },
		{ &i::ANA, RG_NONE,    RG_L, CN_NONE },
		{ &i::ANA, RG_NONE, RG_NONE, CN_NONE },
		{ &i::ANA, RG_NONE,    RG_A, CN_NONE },
		{ &i::XRA, RG_NONE,    RG_B, CN_NONE },
		{ &i::XRA, RG_NONE,    RG_C, CN_NONE },
		{ &i::XRA, RG_NONE,    RG_D, CN_NONE },
		{ &i::XRA, RG_NONE,    RG_E, CN_NONE },
		{ &i::XRA, RG_NONE,    RG_H, CN_NONE },
		{ &i::XRA, RG_NONE,    RG_L, CN_NONE },
		{ &i::XRA, RG_NONE, RG_NONE, CN_NONE },
		{ &i::XRA, RG_NONE,    RG_A, CN_NONE },

		// 0xB0 -> 0xBF
		{ &i::ORA, RG_NONE,    RG_B, CN_NONE },
		{ &i::ORA, RG_NONE,    RG_C, CN_NONE },
		{ &i::ORA, RG_NONE,    RG_D, CN_NONE },
		{ &i::ORA, RG_NONE,    RG_E, CN_NONE },
		{ &i::ORA, RG_NONE,    RG_H, CN_NONE },
		{ &i::ORA, RG_NONE,    RG_L, CN_NONE },
		{ &i::ORA, RG_NONE, RG_NONE, CN_NONE },
		{ &i::ORA, RG_NONE,    RG_A, CN_NONE },
		{ &i::CMP, RG_NONE,    RG_B, CN_NONE },
		{ &i::CMP, RG_NONE,    RG_C, CN_NONE },
		{ &i::CMP, RG_NONE,    RG_D, CN_NONE },
		{ &i::CMP, RG_NONE,    RG_E, CN_NONE },
		{ &i::CMP, RG_NONE,    RG_H, CN_NONE },
		{ &i::CMP, RG_NONE,    RG_L, CN_NONE },
		{ &i::CMP, RG_NONE, RG_NONE, CN_NONE },
		{ &i::CMP, RG_NONE,    RG_A, CN_NONE },

		// 0xC0 -> 0xCF
		{ &i::RET, RG_NONE, RG_NONE,   CN_NZ },
		{ &i::POP,   RP_BC, RG_NONE, CN_NONE },
		{ &i::JMP, RG_NONE, RG_NONE,   CN_NZ },
		{ &i::JMP, RG_NONE, RG_NONE, CN_NONE },
		{ &i::CALL,RG_NONE, RG_NONE,   CN_NZ },
		{ &i::PUSH,RG_NONE,   RP_BC, CN_NONE },
		{ &i::ADI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RST, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RET, RG_NONE, RG_NONE,    CN_Z },
		{ &i::RET, RG_NONE, RG_NONE, CN_NONE },
		{ &i::JMP, RG_NONE, RG_NONE,    CN_Z },
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::CALL,RG_NONE, RG_NONE,    CN_Z },
		{ &i::CALL,RG_NONE, RG_NONE, CN_NONE },
		{ &i::ACI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RST, RG_NONE, RG_NONE, CN_NONE },

		// 0xD0 -> 0xDF
		{ &i::RET, RG_NONE, RG_NONE,   CN_NC },
		{ &i::POP,   RP_DE, RG_NONE, CN_NONE },
		{ &i::JMP, RG_NONE, RG_NONE,   CN_NC },
		{ &i::OUT, RG_NONE, RG_NONE, CN_NONE },
		{ &i::CALL,RG_NONE, RG_NONE,   CN_NC },
		{ &i::PUSH,RG_NONE,   RP_DE, CN_NONE },
		{ &i::SUI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RST, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RET, RG_NONE, RG_NONE,    CN_C },
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::JMP, RG_NONE, RG_NONE,    CN_C },
		{ &i::IN,  RG_NONE, RG_NONE, CN_NONE },
		{ &i::CALL,RG_NONE, RG_NONE,    CN_C },
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::SBI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RST, RG_NONE, RG_NONE, CN_NONE },

		// 0xE0 -> 0xEF
		{ &i::RET, RG_NONE, RG_NONE,   CN_PO },
		{ &i::POP,   RP_HL, RG_NONE, CN_NONE },
		{ &i::JMP, RG_NONE, RG_NONE,   CN_PO },
		{ &i::XTHL,RG_NONE, RG_NONE, CN_NONE },
		{ &i::CALL,RG_NONE, RG_NONE,   CN_PO },
		{ &i::PUSH,RG_NONE,   RP_HL, CN_NONE },
		{ &i::ANI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RST, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RET, RG_NONE, RG_NONE,   CN_PE },
		{ &i::PCHL,RG_NONE, RG_NONE, CN_NONE },
		{ &i::JMP, RG_NONE, RG_NONE,   CN_PE },
		{ &i::XCHG,RG_NONE, RG_NONE, CN_NONE },
		{ &i::CALL,RG_NONE, RG_NONE,   CN_PE },
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::XRI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RST, RG_NONE, RG_NONE, CN_NONE },

		// 0xF0 -> 0xFF
		{ &i::RET, RG_NONE, RG_NONE,    CN_P },
		{ &i::POP,   RP_AF, RG_NONE, CN_NONE },
		{ &i::JMP, RG_NONE, RG_NONE,    CN_P },
		{ &i::DI,  RG_NONE, RG_NONE, CN_NONE },
		{ &i::CALL,RG_NONE, RG_NONE,    CN_P },
		{ &i::PUSH,RG_NONE,   RP_AF, CN_NONE },
		{ &i::ORI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RST, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RET, RG_NONE, RG_NONE,    CN_N },
		{ &i::SPHL,RG_NONE, RG_NONE, CN_NONE },
		{ &i::JMP, RG_NONE, RG_NONE,    CN_N },
		{ &i::EI,  RG_NONE, RG_NONE, CN_NONE },
		{ &i::CALL,RG_NONE, RG_NONE,    CN_N },
		{ &i::UNI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::CPI, RG_NONE, RG_NONE, CN_NONE },
		{ &i::RST, RG_NONE, RG_NONE, CN_NONE },

	};
	
}

i8080::~i8080( void ) {

}

/* Memory Access */
void i8080::clear_memory( void ) {
	for ( int i = 0; i < memory.size(); i++ ) {
		memory[ i ] = 0x00;
	}
}

uint8_t i8080::fetch_byte( void ) {
	uint8_t ret = read_byte( PC );
	PC++;
	return ret;
}

uint16_t i8080::fetch_word( void ) {
	uint8_t lo = fetch_byte();
	uint8_t hi = fetch_byte();

	return ( hi << 8 ) | lo;
}

uint8_t i8080::read_byte( uint16_t addr ) {
	return memory[ addr ];
}

uint16_t i8080::read_word( uint16_t addr ) {
	return ( memory[ addr + 1 ] << 8 ) | memory[ addr ];
}

void i8080::write_byte( uint16_t addr, uint8_t val ) {
	memory[ addr ] = val;
}

void i8080::write_word( uint16_t addr, uint16_t val ) {
	memory[ addr ] = val & 0xFF;
	memory[ addr + 1 ] = val >> 8;
}

/* Flag Access */
bool i8080::parse_cond( void ) {
	bool ret = false;
	switch ( GET_COND ) {
		case CN_NONE: ret = true; break;
		case CN_Z:    ret =  get_flag( F_Z ); break;
		case CN_NZ:   ret = !get_flag( F_Z ); break;
		case CN_C:    ret =  get_flag( F_C ); break;
		case CN_NC:   ret = !get_flag( F_C ); break;
		case CN_PO:   ret = !get_flag( F_P ); break;
		case CN_PE:   ret =  get_flag( F_P ); break;
		case CN_P:    ret = !get_flag( F_S ); break;
		case CN_N:    ret =  get_flag( F_S ); break;
		default: UNI(); break;
	}

	return ret;
}

bool i8080::get_flag( FLAGS f ) {
	return F & f;
}

void i8080::set_flag( FLAGS f, bool val ) {
	if ( val ) {
		F |= f;
	}else {
		F &= ~f;
	}
}
void i8080::set_all_flags( int fs, int fz, int fh, int fp, int fc ) {
	// Sign Flag
	if ( fs != -1 ) {
		set_flag( F_S, fs );
	}

	// Zero Flag
	if ( fz != -1 ) {
		set_flag( F_Z, fz );
	}

	// Aux Flag
	if ( fh != -1 ) {
		set_flag( F_H, fh );
	}

	// Parity Flag
	if ( fp != -1 ) {
		set_flag( F_P, fp );
	}

	// Carry Flag
	if ( fc != -1 ) {
		set_flag( F_C, fc );
	}
}

bool i8080::parity( uint8_t data ) {
	// Set parity flag
	uint8_t cnt = 0;
	for ( int index = 0; index < 8; index++ ) {
		if ( ( data & ( 1 << index ) ) != 0 ) {
			cnt++;
		}
	}

	// Check to see if count is an even number
	if ( ( cnt & 0x01 ) == 0x00 ) {
		return true;
	} else {
		return false;
	}
}

/* Register Access */
uint16_t i8080::get_register( RG_NAME r ) {
	uint16_t ret = 0x0000;

	switch ( r ) {
		case  RG_A: ret = A; break;
		case  RG_F: ret = F; break;
		case  RG_B: ret = B; break;
		case  RG_C: ret = C; break;
		case  RG_D: ret = D; break;
		case  RG_E: ret = E; break;
		case  RG_H: ret = H; break;
		case  RG_L: ret = L; break;
		case RP_AF: ret = ( A << 8 ) | F; break;
		case RP_BC: ret = ( B << 8 ) | C; break;
		case RP_DE: ret = ( D << 8 ) | E; break;
		case RP_HL: ret = ( H << 8 ) | L; break;
		case RP_SP: ret = SP; break;
		default: ret = 0x0000; break;
	}

	return ret;
}

void i8080::set_register( RG_NAME r, uint8_t hi, uint8_t lo ) {
	switch ( r ) {
		case  RG_A: A = lo; break;
		case  RG_F: lo |= 0x02; lo &= 0b11010111; F = lo; break;
		case  RG_B: B = lo; break;
		case  RG_C: C = lo; break;
		case  RG_D: D = lo; break;
		case  RG_E: E = lo; break;
		case  RG_H: H = lo; break;
		case  RG_L: L = lo; break;
		case RP_AF: A = hi;  lo |= 0x02; lo &= 0b11010111; F = lo; break;
		case RP_BC: B = hi; C = lo; break;
		case RP_DE: D = hi; E = lo; break;
		case RP_HL: H = hi; L = lo; break;
		case RP_SP: SP = ( hi << 8 ) | lo; break;
		default: break;
	}
}

void i8080::set_register( RG_NAME r, uint16_t val ) {
	switch ( r ) {
		case  RG_A:
		case  RG_F:
		case  RG_B:
		case  RG_C:
		case  RG_D:
		case  RG_E:
		case  RG_H:
		case  RG_L:
		case RP_AF:
		case RP_BC:
		case RP_DE:
		case RP_HL:
			set_register( r, ( val >> 8 ), ( val & 0xFF ) );
			break;
		case RP_SP: SP = val; break;
		default: break;
	}
}

/* Emulation */
// Execute a clock cycle
void i8080::step( void ) {
	// Get next opcode
	opcode = fetch_byte();

	// Execute new opcode
	( this->*op_codes[ opcode ].operate )( );
}

// Reset system variables to known values
void i8080::reset( void ) {
	A = 0x00;
	F = 0x02;
	B = 0x00;
	C = 0x00;
	D = 0x00;
	E = 0x00;
	H = 0x00;
	L = 0x00;

	PC = 0x0000;
	SP = 0x0000;

	error = false;
	halted = false;
	interrupt_enabled = false;
	cycles = 0;
	opcode = 0x00;
}

void i8080::log( void ) {
	Logger::Log( 
		std::format( 
			"|AF: {:04X}|BC: {:04X}|DE: {:04X}|HL: {:04X}|PC: {:04X}|SP: {:04X}|OC: {:02X}|C: {:2d}|H: {}|E: {}|",
			get_register( RP_AF ),
			get_register( RP_BC ),
			get_register( RP_DE ),
			get_register( RP_HL ),
			PC,
			SP,
			opcode,
			cycles,
			halted ? "Y" : "N",
			error  ? "Y" : "N"
		)
	);
}

bool i8080::load_rom( const char* path, uint16_t offset ) {
	//Logger::Log( std::format( "Opening file: {}", path ) );

	// Open rom
	FILE* fptr = nullptr;
	fopen_s( &fptr, path, "rb" );
	if ( !fptr ) {
		Logger::Err( "Could not open file" );
		return false;
	}

	// file size check:
	fseek( fptr, 0, SEEK_END );
	size_t file_size = ftell( fptr );
	rewind( fptr );

	if ( file_size + offset >= 0xFFFF ) {
		fclose( fptr );
		Logger::Err( "Overflow loading ROM. File too large" );
		return false;
	}

	// Load rom into memory
	// copying the bytes in memory:
	size_t result = fread( &memory[ offset ], sizeof( uint8_t ), file_size, fptr );
	if ( result != file_size ) {
		fclose( fptr );
		Logger::Err( std::format( "Error loading file into memory: {}", path ));
		return 1;
	}

	fclose( fptr );

	//Logger::Log( std::format( "File loaded into memory: {}", path ) );
	// No errors
	return true;
}

/* Utility Functions */
uint8_t i8080::internal_add( uint8_t a, uint8_t b, bool cy ) {
	uint8_t res = a + b;
	if ( cy ) {
		res++;
	}

	set_all_flags(
		res & 0x80,
		res == 0,
		will_carry( 4, a, b, cy ),
		parity( res ),
		will_carry( 8, a, b, cy )
	);
	
	return res;
}

bool i8080::will_carry( uint8_t bit_no, uint8_t a, uint8_t b, bool cy ) {
	uint16_t result = a + b;
	if ( cy ) {
		result++;
	}

	uint16_t carry = result ^ a ^ b;

	if ( carry & ( 1 << bit_no ) ) {
		return true;
	}

	return false;
}

void i8080::push_to_stack( uint16_t data ) {
	SP -= 2;

	write_word( SP, data );
}

uint16_t i8080::pop_from_stack( void ) {
	uint16_t ret = read_word( SP );

	SP += 2;

	return ret;
}

/* Opcodes */
void i8080::UNI( void ) {
	cycles += 1;
	halted = true;
	error = true;
	Logger::Err( std::format( "Unknown Opcode {:02X} at address {:04X}", opcode, PC - 1 ));
}

// 0x00 -> 0x3F
void i8080::NOP( void ) {
	cycles += 4;
}

void i8080::LXI( void ) {
	switch ( GET_DST ) {
		case RP_BC:
		case RP_DE:
		case RP_HL:
		case RP_SP:
			set_register( GET_DST, fetch_word() );
			break;

		default: UNI(); break;
	}

	cycles += 10;
}

void i8080::STAX( void ) {
	if ( opcode == 0x32 ) {
		write_byte( fetch_word(), A );
		cycles += 13;
	} else {
		write_byte( get_register( GET_SRC ), A );
		cycles += 7;
	}
}

void i8080::LDAX( void ) {
	if ( opcode == 0x3A ) {
		set_register( RG_A, read_byte( fetch_word() ) );
		cycles += 13;
	} else {
		set_register( RG_A, read_byte( get_register( GET_SRC ) ) );
		cycles += 7;
	}
}

void i8080::SHLD( void ) {
	write_word( fetch_word(), get_register( RP_HL ) );
	cycles += 16;
}

void i8080::LHLD( void ) {
	set_register( RP_HL, read_word( fetch_word() ) );
	cycles += 16;
}

void i8080::INX( void ) {
	set_register( GET_DST, get_register( GET_DST ) + 1 );
	cycles += 5;
}

void i8080::DCX( void ) {
	set_register( GET_DST, get_register( GET_DST ) - 1 );
	cycles += 5;
}

void i8080::INR( void ) {
	uint8_t check = 0;
	if ( opcode == 0x34 ) {
		write_byte(
			get_register( RP_HL ),
			read_byte( get_register( RP_HL ) ) + 1
		);
		check = read_byte( get_register( RP_HL ) );
		cycles += 10;
	} else {
		set_register( GET_DST, ( get_register( GET_DST ) + 1 ) & 0xFF );
		check = get_register( GET_DST ) & 0xFF;
		cycles += 5;
	}

	set_all_flags(
		check & 0x80,		// Sign
		check == 0x00,	// Zero
		( check & 0x0F ) == 0x00,	// Aux
		parity( check ),// Parity
		-1							// Carry
	);
}

void i8080::DCR( void ) {
	uint8_t check = 0;
	if ( opcode == 0x35 ) {
		write_byte(
			get_register( RP_HL ),
			read_byte( get_register( RP_HL ) ) - 1
		);
		check = read_byte( get_register( RP_HL ) );
		cycles += 10;
	} else {
		set_register( GET_DST, ( get_register( GET_DST ) - 1 ) & 0xFF );
		check = get_register( GET_DST ) & 0xFF;
		cycles += 5;
	}

	set_all_flags(
		check & 0x80,		// Sign
		check == 0x00,	// Zero
		!( ( check & 0xF ) == 0xF ),	// Aux
		parity( check ),// Parity
		-1							// Carry
	);
}

void i8080::MVI( void ) {
	if ( opcode == 0x36 ) {
		write_byte( get_register( RP_HL ), fetch_byte() );
		cycles += 10;
	} else {
		set_register( GET_DST, fetch_byte() );
		cycles += 7;
	}
}

void i8080::RLC( void ) {
	uint8_t tmp = get_register( RG_A ) & 0xFF;
	set_flag( F_C, tmp & 0x80 );

	tmp <<= 1;
	if ( get_flag( F_C ) ) {
		tmp |= 0x01;
	}

	set_register( RG_A, tmp );
	cycles += 4;
}


void i8080::RAL( void ) {
	bool old_c = get_flag( F_C );
	set_flag( F_C, A & 0x80 );

	A <<= 1;
	if ( old_c ) {
		A |= 0x01;
	}

	cycles += 4;
}

void i8080::DAA( void ) {
	bool cy = get_flag( F_C );
	uint8_t correction = 0;

	uint8_t lsb = A & 0x0F;
	uint8_t msb = A >> 4;

	if ( get_flag( F_H ) || lsb > 9 ) {
		correction += 0x06;
	}

	if ( get_flag( F_C ) || msb > 9 || ( msb >= 9 && lsb > 9 ) ) {
		correction += 0x60;
		cy = 1;
	}

	A = internal_add( A, correction, false );
	set_flag( F_C, cy );

	cycles += 4;
}

void i8080::STC( void ) {
	set_flag( F_C, true );
	cycles += 4;
}

void i8080::DAD( void ) {
	uint16_t to_add = get_register( GET_SRC );
	uint32_t res = get_register( GET_DST ) + to_add;
	
	set_register( GET_DST, res & 0xFFFF );

	set_flag( F_C, res > 0x0000FFFF );

	cycles += 10;
}

void i8080::RRC( void ) {
	uint8_t tmp = get_register( RG_A ) & 0xFF;
	set_flag( F_C, tmp & 0x01 );

	tmp >>= 1;
	if ( get_flag( F_C ) ) {
		tmp |= 0x80;
	}

	set_register( RG_A, tmp );
	cycles += 4;
}

void i8080::RAR( void ) {
	uint8_t tmp = get_register( RG_A ) & 0xFF;
	bool old_c = get_flag( F_C );
	set_flag( F_C, tmp & 0x01 );

	tmp >>= 1;
	if ( old_c ) {
		tmp |= 0x80;
	}

	set_register( RG_A, tmp );
	cycles += 4;
}

void i8080::CMA( void ) {
	set_register( RG_A, ~get_register( RG_A ) );
	cycles += 4;
}

void i8080::CMC( void ) {
	set_flag( F_C, !get_flag( F_C ) );
	cycles += 4;
}

// 0x40 -> 0x7F
void i8080::MOV( void ) {
	switch ( opcode ) {
		// M -> R
		case 0x46:
		case 0x56:
		case 0x66:
		case 0x4E:
		case 0x5E:
		case 0x6E:
		case 0x7E:
			set_register( GET_DST, read_byte( get_register( RP_HL ) ) );
			cycles += 7;
			break;

		// R -> M
		case 0x70:
		case 0x71:
		case 0x72:
		case 0x73:
		case 0x74:
		case 0x75:
		case 0x77:
			write_byte( get_register( RP_HL ), get_register( GET_SRC ) & 0xFF );
			cycles += 7;
			break;
		
		// R -> R
		default: 
			set_register( GET_DST, get_register( GET_SRC ) ); 
			cycles += 5;
			break;
	}
}

void i8080::HLT( void ) {
	halted = true;
	cycles += 7;
}

// 0x80 -> 0xBF
void i8080::ADD( void ) {
	uint8_t to_add = 0x00;

	if ( opcode == 0x86 ) {
		to_add = read_byte( get_register( RP_HL ) );
		cycles += 7;
	} else {
		to_add = get_register( GET_SRC ) & 0xFF ;
		cycles += 4;
	}

	A = internal_add( A, to_add, false );

}

void i8080::ADC( void ) {
	uint8_t to_add = 0x00;

	if ( opcode == 0x8E ) {
		to_add = read_byte( get_register( RP_HL ) );
		cycles += 7;
	} else {
		to_add = get_register( GET_SRC ) & 0xFF ;
		cycles += 4;
	}

	A = internal_add( A, to_add, get_flag( F_C ) );

}

void i8080::SUB( void ) {
	uint8_t to_add = 0x00;

	if ( opcode == 0x96 ) {
		to_add = read_byte( get_register( RP_HL ) );
		cycles += 7;
	} else {
		to_add = get_register( GET_SRC ) & 0xFF ;
		cycles += 4;
	}

	A = internal_add( A, ~( to_add ), true);
	set_flag( F_C, !get_flag( F_C ) );	// Flip carry flag for SUB operations
}

void i8080::SBC( void ) {
	uint8_t to_add = 0x00;

	if ( opcode == 0x9E ) {
		to_add = read_byte( get_register( RP_HL ) );
		cycles += 7;
	} else {
		to_add = get_register( GET_SRC ) & 0xFF ;
		cycles += 4;
	}

	A = internal_add( A, ~( to_add ), !get_flag( F_C ) );
	set_flag( F_C, !get_flag( F_C ) );	// Flip carry flag for SUB operations
}

void i8080::ANA( void ) {
	uint8_t op = 0x00;
	uint8_t res = 0x00;

	if ( opcode == 0xA6 ) {
		op = read_byte( get_register( RP_HL ) );
		cycles += 7;
	} else {
		op = get_register( GET_SRC ) & 0xFF ;
		cycles += 4;
	}

	res = A & op;

	set_all_flags(
		res & 0x80,
		res == 0x00,
		( ( A | op ) & 0x08 ) != 0,
		parity( res ),
		false
	);

	A = res;

}

void i8080::XRA( void ) {
	uint8_t op = 0x00;

	if ( opcode == 0xAE ) {
		op = read_byte( get_register( RP_HL ) );
		cycles += 7;
	} else {
		op = get_register( GET_SRC ) & 0xFF ;
		cycles += 4;
	}

	A = A ^ op;

	set_all_flags(
		A & 0x80,
		A == 0x00,
		false,
		parity( A ),
		false
	);

}

void i8080::ORA( void ) {
	uint8_t op = 0x00;

	if ( opcode == 0xB6 ) {
		op = read_byte( get_register( RP_HL ) );
		cycles += 7;
	} else {
		op = get_register( GET_SRC ) & 0xFF ;
		cycles += 4;
	}

	A = A | op;

	set_all_flags(
		A & 0x80,
		A == 0x00,
		false,
		parity( A ),
		false
	);

}

void i8080::CMP( void ) {
	uint8_t op = 0x00;
	uint16_t res = 0x00;

	if ( opcode == 0xBE ) {
		op = read_byte( get_register( RP_HL ) );
		cycles += 7;
	} else {
		op = get_register( GET_SRC ) & 0xFF ;
		cycles += 4;
	}

	res = A - op;

	set_all_flags(
		res & 0x80,
		( res & 0xFF ) == 0x00,
		( ~( A ^ res ^ op ) & 0x10 ),
		parity( res & 0xFF ),
		res > 0xFF
	);

}

// 0xC0 -> 0xFF
void i8080::RET( void ) {
	if ( parse_cond() ) {
		PC = pop_from_stack();
		if ( GET_COND == CN_NONE ) {
			cycles += 10;
		} else {
			cycles += 11;
		}
	} else {
		cycles += 5;
	}
}

void i8080::POP( void ) {
	set_register( GET_DST, pop_from_stack() );
	cycles += 10;
}

void i8080::JMP( void ) {
	if ( parse_cond() ) {
		PC = fetch_word();
	} else {
		PC += 2;
	}

	cycles += 10;
}

void i8080::CALL( void ) {
	if ( parse_cond() ) {
		push_to_stack( PC + 2 );
		PC = read_word( PC );
		cycles += 17;
	} else {
		PC += 2;
		cycles += 11;
	}
}

void i8080::PUSH( void ) {
	push_to_stack( get_register( GET_SRC ) );
	cycles += 11;
}

void i8080::ADI( void ) {
	A = internal_add( A, fetch_byte(), false );
	cycles += 7;
}

void i8080::ACI( void ) {
	A = internal_add( A, fetch_byte(), get_flag(F_C));
	cycles += 7;
}

void i8080::SUI( void ) {
	A = internal_add( A, ~( fetch_byte() ), true );
	set_flag( F_C, !get_flag( F_C ) );	// Flip carry flag for SUB operations
	cycles += 7;
}

void i8080::SBI( void ) {
	A = internal_add( A, ~( fetch_byte() ), !get_flag( F_C ) );
	set_flag( F_C, !get_flag( F_C ) );	// Flip carry flag for SUB operations
	cycles += 7;
}

void i8080::ANI( void ) {
	uint8_t res = 0x00;
	uint8_t op = fetch_byte();
	cycles += 7;

	res = A & op;

	set_all_flags(
		res & 0x80,
		res == 0x00,
		( ( A | op ) & 0x08 ) != 0,
		parity( res ),
		false
	);

	A = res;

}

void i8080::XRI( void ) {
	uint8_t op = fetch_byte();
	cycles += 7;

	A = A ^ op;

	set_all_flags(
		A & 0x80,
		A == 0x00,
		false,
		parity( A ),
		false
	);

}

void i8080::ORI( void ) {
	uint8_t op = fetch_byte();
	cycles += 7;

	A = A | op;

	set_all_flags(
		A & 0x80,
		A == 0x00,
		false,
		parity( A ),
		false
	);

}

void i8080::CPI( void ) {
	uint8_t op = fetch_byte();
	uint16_t res = 0x00;
	cycles += 7;

	res = A - op;

	set_all_flags(
		res & 0x80,
		( res & 0xFF ) == 0x00,
		( ~( A ^ res ^ op ) & 0x10 ),
		parity( res & 0xFF ),
		res > 0xFF
	);
}

void i8080::RST( void ) {
	switch ( opcode ) {
		case 0xC7: push_to_stack( PC ); PC = 0x0000; break; // RST 0 -> $0000
		case 0xCF: push_to_stack( PC ); PC = 0x0008; break; // RST 1 -> $0008
		case 0xD7: push_to_stack( PC ); PC = 0x0010; break; // RST 2 -> $0010
		case 0xDF: push_to_stack( PC ); PC = 0x0018; break; // RST 3 -> $0018
		case 0xE7: push_to_stack( PC ); PC = 0x0020; break; // RST 4 -> $0020
		case 0xEF: push_to_stack( PC ); PC = 0x0028; break; // RST 5 -> $0028
		case 0xF7: push_to_stack( PC ); PC = 0x0030; break; // RST 6 -> $0030
		case 0xFF: push_to_stack( PC ); PC = 0x0038; break; // RST 7 -> $0038
		default: UNI(); break;
	}
	cycles += 11;
}

void i8080::OUT( void ) {
	uint8_t port = fetch_byte();

	if ( port == 0x00 ) {
		//Logger::Log( "OUT, port 0 called: program halted!" );
		halted = true;
	} else if ( port == 1 ) {
		if ( C == 2 ) {
			printf( "%c", E );
		} else if ( C == 9 ) {
			uint16_t addr = ( D << 8 ) | E;
			do {
				printf( "%c", read_byte( addr ) );
				addr++;
			} while ( read_byte( addr ) != '$' );
		}
	}

	cycles += 10;
}

void i8080::IN( void ) {
	// TODO

	PC += 1;
	cycles += 10;
}

void i8080::XTHL( void ) {
	uint8_t old_l = read_byte( SP );
	uint8_t old_h = read_byte( SP + 1 );

	write_byte( SP, get_register( RG_L ) & 0xFF );
	write_byte( SP + 1, get_register( RG_H ) & 0xFF );

	set_register( RP_HL, old_h, old_l );
	cycles += 18;
}

void i8080::PCHL( void ) {
	PC = get_register( RP_HL );
	cycles += 5;
}

void i8080::XCHG( void ) {
	uint16_t old_hl = get_register( RP_HL );
	set_register( RP_HL, get_register( RP_DE ) );
	set_register( RP_DE, old_hl );
	cycles += 4;
}

void i8080::DI( void ) {
	interrupt_enabled = false;
	cycles += 4;
}

void i8080::EI( void ) {
	interrupt_enabled = true;
	cycles += 4;
}

void i8080::SPHL( void ) {
	set_register( RP_SP, get_register( RP_HL ) );
	cycles += 5;
}