#ifndef NES_EMULATOR_OPCODES_6502
#define NES_EMULATOR_OPCODES_6502

#define NES_EM_DEBUG_MODE 1
#if NES_EM_DEBUG_MODE
	// Initialize the string implementation array for our instructions
	#define NES_EM_INIT_STRING_REPRESENTATION 1
#endif

#include <array>

namespace NesEm
{
#pragma region AddressingModes
	enum class AddressingMode : uint8_t
	{
		Accumulator, // Acts directly on accumulator
		Absolute, // Loads from absolute address
		AbsoluteX, // Absolute, X -> offset absolute address by X reg
		AbsoluteY, // Absolute, Y -> offset absolute address by Y reg
		Immediate, // Loads content itself instead of content from an address
		Implied,
		Indirect,  // 16 bit address is read to	get the actual 16 bit address (== pointer)
		IndirectX, // (Indirect, X) -> First uses ZeroPageX, so offset the zero page access by X, then takes the value found in memory on that location and uses this as an absolute access mode
		IndirectY, // (Indirect), Y -> Takes the "Indirect" zeropage value, then takes the value located there and uses it as an address, after that it adds Y to this absolute address, and then loads the value found at this address
		Relative, // Branching instructions	- branches to place relative on program counter based on current place
		ZeroPage, // First 256 bytes in memory for faster access
		ZeroPageX, // Works as ZeroPage but before getting contents, add value from X reg to the address (offset address by X reg)
		ZeroPageY, // Works as ZeroPage but before getting contents, add value from Y reg to the address (offset address by Y reg)
		Other // Other opcodes (only used for specific invalid opcodes)
	};
#pragma endregion

#pragma region OpCodeFunctions
	inline void ADC() noexcept
	{

	}

	inline void AND() noexcept
	{

	}

	inline void ASL() noexcept
	{

	}

	inline void BCC() noexcept
	{

	}

	inline void BCS() noexcept
	{

	}

	inline void BEQ() noexcept
	{

	}

	inline void BIT() noexcept
	{

	}

	inline void BMI() noexcept
	{

	}

	inline void BNE() noexcept
	{

	}

	inline void BPL() noexcept
	{

	}

	inline void BRK() noexcept
	{

	}

	inline void BVC() noexcept
	{

	}

	inline void BVS() noexcept
	{

	}

	inline void CLC() noexcept
	{

	}

	inline void CLD() noexcept
	{

	}

	inline void CLI() noexcept
	{

	}

	inline void CLV() noexcept
	{

	}

	inline void CMP() noexcept
	{

	}

	inline void CPX() noexcept
	{

	}

	inline void CPY() noexcept
	{

	}

	inline void DEC() noexcept
	{

	}

	inline void DEX() noexcept
	{

	}

	inline void DEY() noexcept
	{

	}

	inline void EOR() noexcept
	{

	}

	inline void INC() noexcept
	{

	}

	inline void INX() noexcept
	{

	}

	inline void INY() noexcept
	{

	}

	inline void JMP() noexcept
	{
		
	}

	inline void JSR() noexcept
	{

	}

	inline void LDA() noexcept
	{

	}

	inline void LDX() noexcept
	{

	}

	inline void LDY() noexcept
	{

	}

	inline void LSR() noexcept
	{

	}

	inline void NOP() noexcept
	{

	}

	inline void ORA() noexcept
	{

	}

	inline void PHA() noexcept
	{

	}

	inline void PHP() noexcept
	{

	}

	inline void PLA() noexcept
	{

	}

	inline void PLP() noexcept
	{

	}

	inline void ROL() noexcept
	{

	}

	inline void ROR() noexcept
	{

	}

	inline void RTI() noexcept
	{

	}

	inline void RTS() noexcept
	{

	}

	inline void SBC() noexcept
	{

	}

	inline void SEC() noexcept
	{

	}

	inline void SED() noexcept
	{

	}

	inline void SEI() noexcept
	{

	}

	inline void STA() noexcept
	{

	}

	inline void STX() noexcept
	{

	}

	inline void STY() noexcept
	{

	}

	inline void TAX() noexcept
	{

	}

	inline void TAY() noexcept
	{

	}

	inline void TSX() noexcept
	{

	}

	inline void TXA() noexcept
	{

	}

	inline void TXS() noexcept
	{

	}

	inline void TYA() noexcept
	{

	}

	// Invalid opcode
	inline void INV() noexcept
	{

	}
#pragma endregion

#pragma region OpCodesData
	enum class OpCodes : uint8_t;

	//Struct to represent a single instruction with all necessary data
	struct Instruction final
	{
		uint8_t id; // ID in the function table & string representation table
		AddressingMode mode; // What the address mode of the instruction is
		uint8_t cycles; // How many cycles the instruction takes

		constexpr Instruction() = default;
		constexpr Instruction(OpCodes o, AddressingMode m, uint8_t c):
			id{ static_cast<uint8_t>(o) },
			mode{ m },
			cycles{ c }{}
	};

	// because typing sucks
	using _i = Instruction;
	using _a = AddressingMode;
	using _o = OpCodes;

#pragma region OpCodesDataConstants
	constexpr auto AD_ACC{ _a::Accumulator };
	constexpr auto AD_ABS{ _a::Absolute };
	constexpr auto AD_ABX{ _a::AbsoluteX };
	constexpr auto AD_ABY{ _a::AbsoluteY };
	constexpr auto AD_IMM{ _a::Immediate };
	constexpr auto AD_IMP{ _a::Implied };
	constexpr auto AD_IND{ _a::Indirect };
	constexpr auto AD_INX{ _a::IndirectX };
	constexpr auto AD_INY{ _a::IndirectY };
	constexpr auto AD_REL{ _a::Relative };
	constexpr auto AD_ZPG{ _a::ZeroPage };
	constexpr auto AD_ZPX{ _a::ZeroPageX };
	constexpr auto AD_ZPY{ _a::ZeroPageY };
	constexpr auto AD_OTH{ _a::Other };
#pragma endregion

#pragma region OpCodesTables
	enum class OpCodes : uint8_t {
		ADC,
		AND,
		ASL,
		BCC,
		BCS,
		BEQ,
		BIT,
		BMI,
		BNE,
		BPL,
		BRK,
		BVC,
		BVS,
		CLC,
		CLD,
		CLI,
		CLV,
		CMP,
		CPX,
		CPY,
		DEC,
		DEX,
		DEY,
		EOR,
		INC,
		INX,
		INY,
		JMP,
		JSR,
		LDA,
		LDX,
		LDY,
		LSR,
		NOP,
		ORA,
		PHA,
		PHP,
		PLA,
		PLP,
		ROL,
		ROR,
		RTI,
		RTS,
		SBC,
		SEC,
		SED,
		SEI,
		STA,
		STX,
		STY,
		TAX,
		TAY,
		TSX,
		TXA,
		TXS,
		TYA,
		INV
	};

	// 16x16 Array to represent all possible opcode bytes read from the ROM
	// $00 -> $FF (0x00 -> 0xFF) 
	// Table with all opcodes for reference: 
	// https://www.masswerk.at/6502/6502_instruction_set.html
	static constexpr std::array<Instruction, 256> OPCODES_6502
	{	   /*				0		     */		 /*				  1		       */		/*				2		     */	      /*			  3		       */		/*				4		     */		  /*			   5		    */		/*			 	 6		      */	  /*			   7		    */		 /*				 8		      */	   /*			    9		     */		 /*				  A		       */	   /*				B		     */		 /*				  C		       */	   /*				D		     */		  /*			   E		    */		/*				 F		      */
		/*0*/	_i{ _o::BRK, AD_IMP, 7 }, { _o::ORA, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::INV, AD_ZPG, 3 }, { _o::ORA, AD_ZPG, 3 }, { _o::ASL, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PHP, AD_IMP, 3 }, { _o::ORA, AD_IMM, 2 }, { _o::ASL, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::INV, AD_ABS, 4 }, { _o::ORA, AD_ABS, 4 }, { _o::ASL, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
		/*1*/	_i{ _o::BPL, AD_REL, 2 }, { _o::ORA, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::ORA, AD_ZPX, 4 }, { _o::ASL, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::CLC ,AD_IMP, 2 }, { _o::ORA, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::ORA, AD_ABX, 4 }, { _o::ASL, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
		/*2*/	_i{ _o::JSR, AD_ABS, 6 }, { _o::AND, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::BIT, AD_ZPG, 3 }, { _o::AND, AD_ZPG, 3 }, { _o::ROL, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PLP, AD_IMP, 4 }, { _o::AND, AD_IMM, 2 }, { _o::ROL, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::BIT, AD_ABS, 4 }, { _o::AND, AD_ABS, 4 }, { _o::ROL, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
		/*3*/	_i{ _o::BMI, AD_REL, 2 }, { _o::AND, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::AND, AD_ZPX, 4 }, { _o::ROL, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::SEC, AD_IMP, 2 }, { _o::AND, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::AND, AD_ABX, 4 }, { _o::ROL, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
		/*4*/	_i{ _o::RTI, AD_IMP, 6 }, { _o::EOR, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::INV, AD_ZPG, 3 }, { _o::EOR, AD_ZPG, 3 }, { _o::LSR, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PHA, AD_IMP, 3 }, { _o::EOR, AD_IMM, 2 }, { _o::LSR, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::JMP, AD_ABS, 3 }, { _o::EOR, AD_ABS, 4 }, { _o::LSR, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
		/*5*/	_i{ _o::BVC, AD_REL, 2 }, { _o::EOR, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::EOR, AD_ZPX, 4 }, { _o::LSR, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::CLI, AD_IMP, 2 }, { _o::EOR, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::EOR, AD_ABX, 4 }, { _o::LSR, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
		/*6*/	_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		/*7*/	_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		/*8*/	_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		/*9*/	_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		/*A*/	_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		/*B*/	_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		/*C*/	_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		/*D*/	_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		/*E*/	_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		/*F*/	_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
	};

	//One lone coders table just as an extra comparison to make sure all our cycles are correct
	//{
	//	{ "BRK", &a::BRK, &a::IMM, 7 }, { "ORA", &a::ORA, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 3 }, { "ORA", &a::ORA, &a::ZP0, 3 }, { "ASL", &a::ASL, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "PHP", &a::PHP, &a::IMP, 3 }, { "ORA", &a::ORA, &a::IMM, 2 }, { "ASL", &a::ASL, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::NOP, &a::IMP, 4 }, { "ORA", &a::ORA, &a::ABS, 4 }, { "ASL", &a::ASL, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BPL", &a::BPL, &a::REL, 2 }, { "ORA", &a::ORA, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "ORA", &a::ORA, &a::ZPX, 4 }, { "ASL", &a::ASL, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "CLC", &a::CLC, &a::IMP, 2 }, { "ORA", &a::ORA, &a::ABY, 4 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "ORA", &a::ORA, &a::ABX, 4 }, { "ASL", &a::ASL, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//	{ "JSR", &a::JSR, &a::ABS, 6 }, { "AND", &a::AND, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "BIT", &a::BIT, &a::ZP0, 3 }, { "AND", &a::AND, &a::ZP0, 3 }, { "ROL", &a::ROL, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "PLP", &a::PLP, &a::IMP, 4 }, { "AND", &a::AND, &a::IMM, 2 }, { "ROL", &a::ROL, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "BIT", &a::BIT, &a::ABS, 4 }, { "AND", &a::AND, &a::ABS, 4 }, { "ROL", &a::ROL, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BMI", &a::BMI, &a::REL, 2 }, { "AND", &a::AND, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "AND", &a::AND, &a::ZPX, 4 }, { "ROL", &a::ROL, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "SEC", &a::SEC, &a::IMP, 2 }, { "AND", &a::AND, &a::ABY, 4 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "AND", &a::AND, &a::ABX, 4 }, { "ROL", &a::ROL, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//	{ "RTI", &a::RTI, &a::IMP, 6 }, { "EOR", &a::EOR, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 3 }, { "EOR", &a::EOR, &a::ZP0, 3 }, { "LSR", &a::LSR, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "PHA", &a::PHA, &a::IMP, 3 }, { "EOR", &a::EOR, &a::IMM, 2 }, { "LSR", &a::LSR, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "JMP", &a::JMP, &a::ABS, 3 }, { "EOR", &a::EOR, &a::ABS, 4 }, { "LSR", &a::LSR, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BVC", &a::BVC, &a::REL, 2 }, { "EOR", &a::EOR, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "EOR", &a::EOR, &a::ZPX, 4 }, { "LSR", &a::LSR, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "CLI", &a::CLI, &a::IMP, 2 }, { "EOR", &a::EOR, &a::ABY, 4 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "EOR", &a::EOR, &a::ABX, 4 }, { "LSR", &a::LSR, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//	{ "RTS", &a::RTS, &a::IMP, 6 }, { "ADC", &a::ADC, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 3 }, { "ADC", &a::ADC, &a::ZP0, 3 }, { "ROR", &a::ROR, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "PLA", &a::PLA, &a::IMP, 4 }, { "ADC", &a::ADC, &a::IMM, 2 }, { "ROR", &a::ROR, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "JMP", &a::JMP, &a::IND, 5 }, { "ADC", &a::ADC, &a::ABS, 4 }, { "ROR", &a::ROR, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BVS", &a::BVS, &a::REL, 2 }, { "ADC", &a::ADC, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "ADC", &a::ADC, &a::ZPX, 4 }, { "ROR", &a::ROR, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "SEI", &a::SEI, &a::IMP, 2 }, { "ADC", &a::ADC, &a::ABY, 4 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "ADC", &a::ADC, &a::ABX, 4 }, { "ROR", &a::ROR, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//	{ "???", &a::NOP, &a::IMP, 2 }, { "STA", &a::STA, &a::IZX, 6 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 6 }, { "STY", &a::STY, &a::ZP0, 3 }, { "STA", &a::STA, &a::ZP0, 3 }, { "STX", &a::STX, &a::ZP0, 3 }, { "???", &a::XXX, &a::IMP, 3 }, { "DEY", &a::DEY, &a::IMP, 2 }, { "???", &a::NOP, &a::IMP, 2 }, { "TXA", &a::TXA, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "STY", &a::STY, &a::ABS, 4 }, { "STA", &a::STA, &a::ABS, 4 }, { "STX", &a::STX, &a::ABS, 4 }, { "???", &a::XXX, &a::IMP, 4 },
	//	{ "BCC", &a::BCC, &a::REL, 2 }, { "STA", &a::STA, &a::IZY, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 6 }, { "STY", &a::STY, &a::ZPX, 4 }, { "STA", &a::STA, &a::ZPX, 4 }, { "STX", &a::STX, &a::ZPY, 4 }, { "???", &a::XXX, &a::IMP, 4 }, { "TYA", &a::TYA, &a::IMP, 2 }, { "STA", &a::STA, &a::ABY, 5 }, { "TXS", &a::TXS, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 5 }, { "???", &a::NOP, &a::IMP, 5 }, { "STA", &a::STA, &a::ABX, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "???", &a::XXX, &a::IMP, 5 },
	//	{ "LDY", &a::LDY, &a::IMM, 2 }, { "LDA", &a::LDA, &a::IZX, 6 }, { "LDX", &a::LDX, &a::IMM, 2 }, { "???", &a::XXX, &a::IMP, 6 }, { "LDY", &a::LDY, &a::ZP0, 3 }, { "LDA", &a::LDA, &a::ZP0, 3 }, { "LDX", &a::LDX, &a::ZP0, 3 }, { "???", &a::XXX, &a::IMP, 3 }, { "TAY", &a::TAY, &a::IMP, 2 }, { "LDA", &a::LDA, &a::IMM, 2 }, { "TAX", &a::TAX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "LDY", &a::LDY, &a::ABS, 4 }, { "LDA", &a::LDA, &a::ABS, 4 }, { "LDX", &a::LDX, &a::ABS, 4 }, { "???", &a::XXX, &a::IMP, 4 },
	//	{ "BCS", &a::BCS, &a::REL, 2 }, { "LDA", &a::LDA, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 5 }, { "LDY", &a::LDY, &a::ZPX, 4 }, { "LDA", &a::LDA, &a::ZPX, 4 }, { "LDX", &a::LDX, &a::ZPY, 4 }, { "???", &a::XXX, &a::IMP, 4 }, { "CLV", &a::CLV, &a::IMP, 2 }, { "LDA", &a::LDA, &a::ABY, 4 }, { "TSX", &a::TSX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 4 }, { "LDY", &a::LDY, &a::ABX, 4 }, { "LDA", &a::LDA, &a::ABX, 4 }, { "LDX", &a::LDX, &a::ABY, 4 }, { "???", &a::XXX, &a::IMP, 4 },
	//	{ "CPY", &a::CPY, &a::IMM, 2 }, { "CMP", &a::CMP, &a::IZX, 6 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "CPY", &a::CPY, &a::ZP0, 3 }, { "CMP", &a::CMP, &a::ZP0, 3 }, { "DEC", &a::DEC, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "INY", &a::INY, &a::IMP, 2 }, { "CMP", &a::CMP, &a::IMM, 2 }, { "DEX", &a::DEX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "CPY", &a::CPY, &a::ABS, 4 }, { "CMP", &a::CMP, &a::ABS, 4 }, { "DEC", &a::DEC, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BNE", &a::BNE, &a::REL, 2 }, { "CMP", &a::CMP, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "CMP", &a::CMP, &a::ZPX, 4 }, { "DEC", &a::DEC, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "CLD", &a::CLD, &a::IMP, 2 }, { "CMP", &a::CMP, &a::ABY, 4 }, { "NOP", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "CMP", &a::CMP, &a::ABX, 4 }, { "DEC", &a::DEC, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//	{ "CPX", &a::CPX, &a::IMM, 2 }, { "SBC", &a::SBC, &a::IZX, 6 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "CPX", &a::CPX, &a::ZP0, 3 }, { "SBC", &a::SBC, &a::ZP0, 3 }, { "INC", &a::INC, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "INX", &a::INX, &a::IMP, 2 }, { "SBC", &a::SBC, &a::IMM, 2 }, { "NOP", &a::NOP, &a::IMP, 2 }, { "???", &a::SBC, &a::IMP, 2 }, { "CPX", &a::CPX, &a::ABS, 4 }, { "SBC", &a::SBC, &a::ABS, 4 }, { "INC", &a::INC, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BEQ", &a::BEQ, &a::REL, 2 }, { "SBC", &a::SBC, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "SBC", &a::SBC, &a::ZPX, 4 }, { "INC", &a::INC, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "SED", &a::SED, &a::IMP, 2 }, { "SBC", &a::SBC, &a::ABY, 4 }, { "NOP", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "SBC", &a::SBC, &a::ABX, 4 }, { "INC", &a::INC, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//};

	// Table of function pointers for each opcode
	using OpcodeFunction = void (*)();
	static constexpr std::array<OpcodeFunction, 57> OPCODES_6502_FUNCTIONS {
		ADC, // 0
		AND, // 1
		ASL, // 2
		BCC, // 3
		BCS, // 4
		BEQ, // 5
		BIT, // 6
		BMI, // 7
		BNE, // 8
		BPL, // 9
		BRK, // 10
		BVC, // 11
		BVS, // 12
		CLC, // 13
		CLD, // 14
		CLI, // 15
		CLV, // 16
		CMP, // 17
		CPX, // 18
		CPY, // 19
		DEC, // 20
		DEX, // 21
		DEY, // 22
		EOR, // 23
		INC, // 24
		INX, // 25
		INY, // 26
		JMP, // 27
		JSR, // 28
		LDA, // 29
		LDX, // 30
		LDY, // 31
		LSR, // 32
		NOP, // 33
		ORA, // 34
		PHA, // 35
		PHP, // 36
		PLA, // 37
		PLP, // 38
		ROL, // 39
		ROR, // 40
		RTI, // 41
		RTS, // 42
		SBC, // 43
		SEC, // 44
		SED, // 45
		SEI, // 46
		STA, // 47
		STX, // 48
		STY, // 49
		TAX, // 50
		TAY, // 51
		TSX, // 52
		TXA, // 53
		TXS, // 54
		TYA, // 55
		// Invalid opcode
		INV  // 56
	};

#ifdef NES_EM_INIT_STRING_REPRESENTATION
	// String representation of all the opcodes, useful for debugging, disassembly,...
	// Put in alphabetical for easy of use, index in array also commented next to each item  
	// https://www.masswerk.at/6502/6502_instruction_set.html -> Instructions by Name
	static constexpr std::array<const char*, 57> OPCODES_6502_NAMES {
		"ADC", // 0
		"AND", // 1
		"ASL", // 2
		"BCC", // 3
		"BCS", // 4
		"BEQ", // 5
		"BIT", // 6
		"BMI", // 7
		"BNE", // 8
		"BPL", // 9
		"BRK", // 10
		"BVC", // 11
		"BVS", // 12
		"CLC", // 13
		"CLD", // 14
		"CLI", // 15
		"CLV", // 16
		"CMP", // 17
		"CPX", // 18
		"CPY", // 19
		"DEC", // 20
		"DEX", // 21
		"DEY", // 22
		"EOR", // 23
		"INC", // 24
		"INX", // 25
		"INY", // 26
		"JMP", // 27
		"JSR", // 28
		"LDA", // 29
		"LDX", // 30
		"LDY", // 31
		"LSR", // 32
		"NOP", // 33
		"ORA", // 34
		"PHA", // 35
		"PHP", // 36
		"PLA", // 37
		"PLP", // 38
		"ROL", // 39
		"ROR", // 40
		"RTI", // 41
		"RTS", // 42
		"SBC", // 43
		"SEC", // 44
		"SED", // 45
		"SEI", // 46
		"STA", // 47
		"STX", // 48
		"STY", // 49
		"TAX", // 50
		"TAY", // 51
		"TSX", // 52
		"TXA", // 53
		"TXS", // 54
		"TYA", // 55
		// Invalid Opcode
		"???"  // 56
	};
#endif
#pragma endregion
#pragma endregion
}

#endif