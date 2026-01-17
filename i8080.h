#pragma once

#include <cstdint>
#include <vector>

#define GET_DST  op_codes[opcode].dst
#define GET_SRC  op_codes[opcode].src
#define GET_COND op_codes[opcode].cond

class i8080 {
// All items made public to make system logging easier
public:
	// Constructors
	i8080( void );
	~i8080( void );

	// System Registers
	uint8_t A;
	uint8_t F;
	uint8_t B;
	uint8_t C;
	uint8_t D;
	uint8_t E;
	uint8_t H;
	uint8_t L;

	// System Pointers
	uint16_t PC;
	uint16_t SP;

	// Emulation values
	bool halted;
	bool error;
	bool interrupt_enabled;
	int cycles;
	uint8_t opcode;

	// Emulator Enumerations
	// Use these to select registers during operations
	enum RG_NAME {
		RG_NONE,	// No register needed
		RG_A,			// Single Registers
		RG_F,			
		RG_B,
		RG_C,
		RG_D,
		RG_E,
		RG_H,
		RG_L,
		RP_AF,		// Register Pairs
		RP_BC,
		RP_DE,
		RP_HL,
		RP_SP
	};

	// Define Conditionals
	enum CONDITIONS {
		CN_NONE,    // No condition
		CN_Z,       // If Zero
		CN_NZ,      // If Not Zero
		CN_C,       // If Carry
		CN_NC,      // If Not Carry
		CN_PO,      // If Parity Even
		CN_PE,      // If Parity Odd
		CN_P,       // If Positive
		CN_N        // If Negative
	};

	// Define Flags
	enum FLAGS {
		F_S = 0x80, // Sign Flag
		F_Z = 0x40, // Zero Falg
		F_H = 0x10, // Half-Carry Flag
		F_P = 0x04, // Parity Flag
		F_C = 0x01  // Carry Flag
	};

	// Memory
	std::vector<uint8_t> memory;
	void clear_memory( void );
	uint8_t fetch_byte( void );
	uint16_t fetch_word( void );
	uint8_t read_byte( uint16_t addr );
	uint16_t read_word( uint16_t addr );
	void write_byte( uint16_t addr, uint8_t val );
	void write_word( uint16_t addr, uint16_t val );

	// Flags
	bool parse_cond( void );
	bool get_flag( FLAGS f );
	void set_flag( FLAGS f, bool val );
	void set_all_flags( int fs, int fz, int fh, int fp, int fc );
	bool parity( uint8_t data );

	// Registers
	uint16_t get_register( RG_NAME r );
	void set_register( RG_NAME r, uint8_t hi, uint8_t lo );
	void set_register( RG_NAME r, uint16_t val );
	
	// Emulation
	void step( void );
	void reset( void );
	void log( void );
	bool load_rom( const char* path, uint16_t offset );

	/* Utility Functions */
	uint8_t internal_add( uint8_t n1, uint8_t n2, bool cy );
	bool will_carry( uint8_t bit_no, uint8_t a, uint8_t b, bool cy );
	void push_to_stack( uint16_t data );
	uint16_t pop_from_stack( void );

	// Opcodes
	void UNI( void );

	// 0x00 -> 0x3F
	void NOP( void );
	void LXI( void );
	void STAX( void );
	void LDAX( void );
	void SHLD( void );
	void LHLD( void );
	void INX( void );
	void DCX( void );
	void INR( void );
	void DCR( void );
	void MVI( void );
	void RLC( void );
	void RAL( void );
	void DAA( void );
	void STC( void );
	void DAD( void );
	void RRC( void );
	void RAR( void );
	void CMA( void );
	void CMC( void );
	
	// 0x40 -> 0x7F
	void MOV( void );
	void HLT( void );

	// 0x80 -> 0xBF
	void ADD( void );
	void ADC( void );
	void SUB( void );
	void SBC( void );
	void ANA( void );
	void XRA( void );
	void ORA( void );
	void CMP( void );

	// 0xC0 -> 0xFF
	void RET( void );
	void POP( void );
	void JMP( void );
	void CALL( void );
	void PUSH( void );
	void ADI( void );
	void ACI( void );
	void SUI( void );
	void SBI( void );
	void ANI( void );
	void XRI( void );
	void ORI( void );
	void CPI( void );
	void RST( void );
	void OUT( void );
	void IN( void );
	void XTHL( void );
	void PCHL( void );
	void XCHG( void );
	void DI( void );
	void EI( void );
	void SPHL( void );

	// Define instruction
	struct INSTRUCTION {
		// Opcode pointer
		void ( i8080::* operate )( void ) = nullptr;
		// Define data destination
		RG_NAME dst;
		// Define data source
		RG_NAME src;
		// Define conditionals
		CONDITIONS cond;
	};
	// Define a vector for instruction definitions
	std::vector<INSTRUCTION> op_codes;
};

