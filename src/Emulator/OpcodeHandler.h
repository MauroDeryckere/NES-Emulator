#ifndef NES_EMULATOR_OPCODEHANDLER
#define NES_EMULATOR_OPCODEHANDLER

// Includes
#include "emulator_pch.h"

#include <array>

namespace NesEm
{
	class CPU;

	// Class handles anything related to opcodes for the 6502
	// This allows us to have an easily extendable code base in case other opcode handlers would be added (SNES, ...)
	class OpcodeHandler final
	{
	public:
		constexpr OpcodeHandler() = default;
		~OpcodeHandler() = default;

		constexpr OpcodeHandler(OpcodeHandler const&) = default;
		constexpr OpcodeHandler(OpcodeHandler&&) = default;
		constexpr OpcodeHandler& operator=(OpcodeHandler const&) = default;
		constexpr OpcodeHandler& operator=(OpcodeHandler&&) = default;

		// Return uint8_t; How many cycles opcode takes
		// Param uint8_t; The opcode we're executing
		// Param (in & out) CPU; The CPU the opcodes is executed on
		[[nodiscard]] uint8_t ExecuteOpcode(uint8_t opcode, CPU& cpu) const noexcept;
		
	private:
#pragma region AddressingModes
		// Info writen here is from
		// https://www.masswerk.at/6502/6502_instruction_set.html
		// Section: Address Modes
		enum class AddressingMode : uint8_t
		{
			Accumulator, // OPC A			operand is AC (implied single byte instruction)
			Absolute,	 // OPC $LLHH		operand is address $HHLL (1)
			AbsoluteX,	 // OPC $LLHH,X	    operand is address; effective address is address incremented by X with carry (2)
			AbsoluteY,	 // OPC $LLHH,Y		operand is address; effective address is address incremented by Y with carry (2)
			Immediate,	 // OPC #$BB		operand is byte BB
			Implied,	 // OPC				operand implied
			Indirect,	 // OPC ($LLHH)		operand is address; effective address is contents of word at address: C.w($HHLL)
			IndirectX,	 // OPC ($LL,X)		operand is zeropage address; effective address is word in (LL + X, LL + X + 1), inc. without carry: C.w($00LL + X)
			IndirectY,	 // OPC ($LL),Y		operand is zeropage address; effective address is word in (LL, LL + 1) incremented by Y with carry: C.w($00LL) + Y
			Relative,	 // OPC $BB			branch target is PC + signed offset BB (3)
			ZeroPage,	 // OPC $LL			operand is zeropage address (hi-byte is zero, address = $00LL)
			ZeroPageX,	 // OPC $LL,X		operand is zeropage address; effective address is address incremented by X without carry (2)
			ZeroPageY,	 // OPC $LL,Y		operand is zeropage address; effective address is address incremented by Y without carry (2)
			Other		 // Specific to invalid opcodes
		};
		// (1)
		/* 16-bit address words are little endian, lo(w)-byte first, followed by the hi(gh)-byte.
		   (An assembler will use a human readable, big-endian notation as in $HHLL.)
		*/
		// (2)
		/*	The available 16 - bit address space is conceived as consisting of pages of 256 bytes each, with
			address hi - bytes represententing the page index.An increment with carry may affect the hi - byte
			and may thus result in a crossing of page boundaries, adding an extra cycle to the execution.
			Increments without carry do not affect the hi - byte of an address and no page transitions do occur.
			Generally, increments of 16 - bit addresses include a carry, increments of zeropage addresses don't.
			Notably this is not related in any way to the state of the carry flag in the status register.
		 */
		// (3)
		/*  Branch offsets are signed 8-bit values, -128 ... +127, negative offsets in two's complement.
			Page transitions may occur and add an extra cycle to the exucution.
		*/

		// Return uint8_t; How many addtional cycles the address mode could take
		// Param AddressingMode; The mode address mode the opcode is executed in
		// Param CPU; The CPU the opcodes is executed on
		// Param (in & out) uint16_t; The address, the address mode returns 
		[[nodiscard]] uint8_t HandleAddressMode(AddressingMode mode, CPU& cpu, uint16_t& address) const noexcept;
#pragma endregion
#pragma region Opcodes
		// Which opcode links to which ID in the function ptr table
		enum class Opcodes : uint8_t {
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
#pragma endregion

#pragma region Instruction
		//Struct to represent a single instruction with all necessary data
		struct Instruction final
		{
			Opcodes id; // ID in the function table & string representation table
			AddressingMode mode; // What the address mode of the instruction is
			uint8_t cycles; // How many cycles the instruction takes
		};

		
#pragma endregion
#pragma region OpCodesDataConstants
		// because typing sucks
		using _i = Instruction;
		using _a = AddressingMode;
		using _o = Opcodes;

		static constexpr auto AD_ACC{ _a::Accumulator };
		static constexpr auto AD_ABS{ _a::Absolute };
		static constexpr auto AD_ABX{ _a::AbsoluteX };
		static constexpr auto AD_ABY{ _a::AbsoluteY };
		static constexpr auto AD_IMM{ _a::Immediate };
		static constexpr auto AD_IMP{ _a::Implied };
		static constexpr auto AD_IND{ _a::Indirect };
		static constexpr auto AD_INX{ _a::IndirectX };
		static constexpr auto AD_INY{ _a::IndirectY };
		static constexpr auto AD_REL{ _a::Relative };
		static constexpr auto AD_ZPG{ _a::ZeroPage };
		static constexpr auto AD_ZPX{ _a::ZeroPageX };
		static constexpr auto AD_ZPY{ _a::ZeroPageY };
		static constexpr auto AD_OTH{ _a::Other };
#pragma endregion

#pragma region OpcodesTables
		// 16x16 Array to represent all possible instructions read from the ROM
		// $00 -> $FF (0x00 -> 0xFF)
		// The table does include illegal opcodes but the opcodes will not do anything and just call the invalid opcode function
		// Cycles and address modes are accurate though, so the system can be expanded to work with illegal opcodes if necessary.
		// Table with all opcodes for reference:
		// https://www.masswerk.at/6502/6502_instruction_set.html
		NES_EM_TABLE std::array<Instruction, 256> OPCODES_6502
		{	           /*			         0			  */	  /*		            1		     */		 /*					   2			*/	    /*					  3			   */      /*					 4			  */	  /*					5			 */	     /*					   6		     */		 /*					  7				*/		/*					 8			  */	   /*					9			  */	  /*				    A			 */	     /*					  B				*/	    /*					  C			   */	    /*					 D			  */	  /*					E			 */		  /*				   F			*/
			/*0*/	_i{ _o::BRK, AD_IMP, 7 }, { _o::ORA, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::INV, AD_ZPG, 3 }, { _o::ORA, AD_ZPG, 3 }, { _o::ASL, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PHP, AD_IMP, 3 }, { _o::ORA, AD_IMM, 2 }, { _o::ASL, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::INV, AD_ABS, 4 }, { _o::ORA, AD_ABS, 4 }, { _o::ASL, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
			/*1*/	_i{ _o::BPL, AD_REL, 2 }, { _o::ORA, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::ORA, AD_ZPX, 4 }, { _o::ASL, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::CLC ,AD_IMP, 2 }, { _o::ORA, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::ORA, AD_ABX, 4 }, { _o::ASL, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
			/*2*/	_i{ _o::JSR, AD_ABS, 6 }, { _o::AND, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::BIT, AD_ZPG, 3 }, { _o::AND, AD_ZPG, 3 }, { _o::ROL, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PLP, AD_IMP, 4 }, { _o::AND, AD_IMM, 2 }, { _o::ROL, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::BIT, AD_ABS, 4 }, { _o::AND, AD_ABS, 4 }, { _o::ROL, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
			/*3*/	_i{ _o::BMI, AD_REL, 2 }, { _o::AND, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::AND, AD_ZPX, 4 }, { _o::ROL, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::SEC, AD_IMP, 2 }, { _o::AND, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::AND, AD_ABX, 4 }, { _o::ROL, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
			/*4*/	_i{ _o::RTI, AD_IMP, 6 }, { _o::EOR, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::INV, AD_ZPG, 3 }, { _o::EOR, AD_ZPG, 3 }, { _o::LSR, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PHA, AD_IMP, 3 }, { _o::EOR, AD_IMM, 2 }, { _o::LSR, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::JMP, AD_ABS, 3 }, { _o::EOR, AD_ABS, 4 }, { _o::LSR, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
			/*5*/	_i{ _o::BVC, AD_REL, 2 }, { _o::EOR, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::EOR, AD_ZPX, 4 }, { _o::LSR, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::CLI, AD_IMP, 2 }, { _o::EOR, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::EOR, AD_ABX, 4 }, { _o::LSR, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
			/*6*/	_i{ _o::RTS, AD_IMP, 6 }, { _o::ADC, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::INV, AD_ZPG, 3 }, { _o::ADC, AD_ZPG, 3 }, { _o::ROR, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PLA, AD_IMP, 4 }, { _o::ADC, AD_IMM, 2 }, { _o::ROR, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::JMP, AD_IND, 5 }, { _o::ADC, AD_ABS, 4 }, { _o::ROR, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
			/*7*/	_i{ _o::BVS, AD_REL, 2 }, { _o::ADC, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::ADC, AD_ZPX, 4 }, { _o::ROR, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::SEI, AD_IMP, 2 }, { _o::ADC, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::ADC, AD_ABX, 4 }, { _o::ROR, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
			/*8*/	_i{ _o::INV, AD_IMM, 2 }, { _o::STA, AD_INX, 6 }, { _o::INV, AD_IMM, 2 }, { _o::INV, AD_INX, 6 }, { _o::STY, AD_ZPG, 3 }, { _o::STA, AD_ZPG, 3 }, { _o::STX, AD_ZPG, 3 }, { _o::INV, AD_ZPG, 3 }, { _o::DEY, AD_IMP, 2 }, { _o::INV, AD_IMM, 2 }, { _o::TXA, AD_IMP, 2 }, { _o::INV, AD_IMM, 2 }, { _o::STY, AD_ABS, 4 }, { _o::STA, AD_ABS, 4 }, { _o::STX, AD_ABS, 4 }, { _o::INV, AD_ABS, 4 },
			/*9*/	_i{ _o::BCC, AD_REL, 2 }, { _o::STA, AD_INY, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 6 }, { _o::STY, AD_ZPX, 4 }, { _o::STA, AD_ZPX, 4 }, { _o::STX, AD_ZPY, 4 }, { _o::INV, AD_ZPY, 4 }, { _o::TYA, AD_IMP, 2 }, { _o::STA, AD_ABY, 5 }, { _o::TXS, AD_IMP, 2 }, { _o::INV, AD_ABY, 5 }, { _o::INV, AD_ABX, 5 }, { _o::STA, AD_ABX, 5 }, { _o::INV, AD_ABY, 5 }, { _o::INV, AD_ABY, 5 },
			/*A*/	_i{ _o::LDY, AD_IMM, 2 }, { _o::LDA, AD_INX, 6 }, { _o::LDX, AD_IMM, 2 }, { _o::INV, AD_INX, 6 }, { _o::LDY, AD_ZPG, 3 }, { _o::LDA, AD_ZPG, 3 }, { _o::LDX, AD_ZPG, 3 }, { _o::INV, AD_ZPG, 3 }, { _o::TAY, AD_IMP, 2 }, { _o::LDA, AD_IMM, 2 }, { _o::TAX, AD_IMP, 2 }, { _o::INV, AD_IMM, 2 }, { _o::LDY, AD_ABS, 4 }, { _o::LDA, AD_ABS, 4 }, { _o::LDX, AD_ABS, 4 }, { _o::INV, AD_ABS, 4 },
			/*B*/	_i{ _o::BCS, AD_REL, 2 }, { _o::LDA, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 5 }, { _o::LDY, AD_ZPX, 4 }, { _o::LDA, AD_ZPX, 4 }, { _o::LDX, AD_ZPY, 4 }, { _o::INV, AD_ZPY, 4 }, { _o::CLV, AD_IMP, 2 }, { _o::LDA, AD_ABY, 4 }, { _o::TSX, AD_IMP, 2 }, { _o::INV, AD_ABY, 4 }, { _o::LDY, AD_ABX, 4 }, { _o::LDA, AD_ABX, 4 }, { _o::LDX, AD_ABY, 4 }, { _o::INV, AD_ABY, 4 },
			/*C*/	_i{ _o::CPY, AD_IMM, 2 }, { _o::CMP, AD_INX, 6 }, { _o::INV, AD_IMM, 2 }, { _o::INV, AD_INX, 8 }, { _o::CPY, AD_ZPG, 3 }, { _o::CMP, AD_ZPG, 3 }, { _o::DEC, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::INY, AD_IMP, 2 }, { _o::CMP, AD_IMM, 2 }, { _o::DEX, AD_IMP, 2 }, { _o::INV, AD_IMM, 2 }, { _o::CPY, AD_ABS, 4 }, { _o::CMP, AD_ABS, 4 }, { _o::DEC, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
			/*D*/	_i{ _o::BNE, AD_REL, 2 }, { _o::CMP, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::CMP, AD_ZPX, 4 }, { _o::DEC, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::CLD, AD_IMP, 2 }, { _o::CMP, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::CMP, AD_ABX, 4 }, { _o::DEC, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
			/*E*/	_i{ _o::CPX, AD_IMM, 2 }, { _o::SBC, AD_INX, 6 }, { _o::INV, AD_IMM, 2 }, { _o::INV, AD_INX, 8 }, { _o::CPX, AD_ZPG, 3 }, { _o::SBC, AD_ZPG, 3 }, { _o::INC, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::INX, AD_IMP, 2 }, { _o::SBC, AD_IMM, 2 }, { _o::NOP, AD_IMP, 2 }, { _o::INV, AD_IMM, 2 }, { _o::CPX, AD_ABS, 4 }, { _o::SBC, AD_ABS, 4 }, { _o::INC, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
			/*F*/	_i{ _o::BEQ, AD_REL, 2 }, { _o::SBC, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::SBC, AD_ZPX, 4 }, { _o::INC, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::SED, AD_IMP, 2 }, { _o::SBC, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::SBC, AD_ABX, 4 }, { _o::INC, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
		};

	#ifdef NES_EM_INIT_STRING_REPRESENTATION
	// String representation of all the opcodes, useful for debugging, disassembly,...
	// Put in alphabetical for easy of use, index in array also commented next to each item  
	// https://www.masswerk.at/6502/6502_instruction_set.html -> Instructions by Name
	NES_EM_TABLE std::array<const char*, 57> OPCODES_6502_NAMES{
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

#pragma region 6502OpcodeFunctions
		// Functions return wether or not cycles may change when certain criteria are (not) met
		// Param CPU: The CPU the instruction acts on
		// Param AddressingMode: The Addressing mode the instruction was executed in

		// Add Memory to Accumulator with Carry
		FORCE_INLINE static bool ADC(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// AND Memory with Accumulator
		FORCE_INLINE static bool AND(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Shift Left One Bit (Memory or Accumulator)
		FORCE_INLINE static bool ASL(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Branch on Carry Clear
		FORCE_INLINE static bool BCC(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Branch on Carry Set
		FORCE_INLINE static bool BCS(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Branch on Result Zero
		FORCE_INLINE static bool BEQ(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Test Bits in Memory with Accumulator
		FORCE_INLINE static bool BIT(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Branch on Result Minus
		FORCE_INLINE static bool BMI(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Branch on Result not Zero
		FORCE_INLINE static bool BNE(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Branch on Result Plus
		FORCE_INLINE static bool BPL(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Force Break
		FORCE_INLINE static bool BRK(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Branch on Overflow Clear
		FORCE_INLINE static bool BVC(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Branch on Overflow Set
		FORCE_INLINE static bool BVS(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Clear Carry Flag
		FORCE_INLINE static bool CLC(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Clear Decimal Mode
		FORCE_INLINE static bool CLD(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Clear Interrupt Disable Bit
		FORCE_INLINE static bool CLI(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Clear Overflow Flag
		FORCE_INLINE static bool CLV(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Compare Memory with Accumulator
		FORCE_INLINE static bool CMP(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Compare Memory and Index X
		FORCE_INLINE static bool CPX(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Compare Memory and Index Y
		FORCE_INLINE static bool CPY(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Decrement Memory by One
		FORCE_INLINE static bool DEC(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Decrement Index X by One
		FORCE_INLINE static bool DEX(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Decrement Index Y by One
		FORCE_INLINE static bool DEY(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Exclusive - OR Memory with Accumulator
		FORCE_INLINE static bool EOR(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Increment Memory by One
		FORCE_INLINE static bool INC(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Increment Index X by One
		FORCE_INLINE static bool INX(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Increment Index Y by One
		FORCE_INLINE static bool INY(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Jump to New Location
		FORCE_INLINE static bool JMP(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Jump to New Location Saving Return Address
		FORCE_INLINE static bool JSR(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Load Accumulator with Memory
		FORCE_INLINE static bool LDA(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Load Index X with Memory
		FORCE_INLINE static bool LDX(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Load Index Y with Memory
		FORCE_INLINE static bool LDY(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Shift One Bit Right(Memory or Accumulator)
		FORCE_INLINE static bool LSR(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// No Operation
		FORCE_INLINE static bool NOP(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// OR Memory with Accumulator
		FORCE_INLINE static bool ORA(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Push Accumulator on Stack
		FORCE_INLINE static bool PHA(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Push Processor Status on Stack
		FORCE_INLINE static bool PHP(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Pull Accumulator from Stack
		FORCE_INLINE static bool PLA(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Pull Processor Status from Stack
		FORCE_INLINE static bool PLP(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Rotate One Bit Left(Memory or Accumulator)
		FORCE_INLINE static bool ROL(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Rotate One Bit Right(Memory or Accumulator)
		FORCE_INLINE static bool ROR(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Return from Interrupt
		FORCE_INLINE static bool RTI(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Return from Subroutine
		FORCE_INLINE static bool RTS(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Subtract Memory from Accumulator with Borrow
		FORCE_INLINE static bool SBC(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Set Carry Flag
		FORCE_INLINE static bool SEC(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Set Decimal Flag
		FORCE_INLINE static bool SED(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Set Interrupt Disable Status
		FORCE_INLINE static bool SEI(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Store Accumulator in Memory
		FORCE_INLINE static bool STA(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Store Index X in Memory
		FORCE_INLINE static bool STX(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Store Index Y in Memory
		FORCE_INLINE static bool STY(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Transfer Accumulator to Index X
		FORCE_INLINE static bool TAX(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Transfer Accumulator to Index Y
		FORCE_INLINE static bool TAY(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Transfer Stack Pointer to Index X
		FORCE_INLINE static bool TSX(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Transfer Index X to Accumulator
		FORCE_INLINE static bool TXA(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Transfer Index X to Stack Register
		FORCE_INLINE static bool TXS(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Transfer Index Y to Accumulator
		FORCE_INLINE static bool TYA(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
		// Invalid opcode
		FORCE_INLINE static bool INV(CPU& cpu, uint16_t address, AddressingMode mode) noexcept;
#pragma endregion
		//Opcode function ptr
		// Cpu, address, addressing mode
		using OpcodeFunction = bool (*)(CPU&, uint16_t, AddressingMode);

		// Table of function pointers for each opcode
		NES_EM_TABLE std::array<OpcodeFunction, 57> OPCODES_6502_FUNCTIONS
		{
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
	};
}

#endif