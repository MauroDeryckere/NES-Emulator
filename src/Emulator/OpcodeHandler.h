#ifndef NES_EMULATOR_OPCODEHANDLER
#define NES_EMULATOR_OPCODEHANDLER

#define NES_EM_DEBUG_MODE 1
	#if NES_EM_DEBUG_MODE
	// Initialize the string implementation array for our instructions
		#define NES_EM_INIT_STRING_REPRESENTATION 1
	#endif

// For MSVC
#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
// For GCC and Clang
#elif defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif

#include <cstdint>
#include <array>

namespace NesEm
{
	// Class handles anything related to opcodes for the 6502
	// This allows us to have an easily extendable code base in case other opcode handlers would be added (SNES, ...)
	class OpcodeHandler final
	{
	public:
		constexpr OpcodeHandler() = default;
		~OpcodeHandler() = default;

		OpcodeHandler(OpcodeHandler const&) = delete;
		OpcodeHandler(OpcodeHandler&&) = delete;
		OpcodeHandler& operator=(OpcodeHandler const&) = delete;
		OpcodeHandler& operator=(OpcodeHandler&&) = delete;

		//TODO: Interface interactions with opcodes
		//bool ExecuteOpcode(CPU& cpu, uint8_t opcode, uint16_t address)	
		//HandleAddressMode(uint8_t opcode)

	private:
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
		// Table with all opcodes for reference: 
		// https://www.masswerk.at/6502/6502_instruction_set.html
		inline static constexpr std::array<OpcodeHandler::Instruction, 256> OPCODES_6502
		{	           /*			0		  */	  /*		 1		 */		 /*	       2	    */	    /*	      3		  */      /*         4		 */		 /*		   5	   */	    /*	      6		  */	  /*	    7		*/		 /*		   8	   */	   /*		  9		 */		  /*	    A		*/	     /*		   B	   */	   /*	     C		 */	      /*	    D		*/		 /*		   E	  */	   /*	     F		 */
			/*0*/	_i{ _o::BRK, AD_IMP, 7 }, { _o::ORA, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::INV, AD_ZPG, 3 }, { _o::ORA, AD_ZPG, 3 }, { _o::ASL, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PHP, AD_IMP, 3 }, { _o::ORA, AD_IMM, 2 }, { _o::ASL, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::INV, AD_ABS, 4 }, { _o::ORA, AD_ABS, 4 }, { _o::ASL, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
			/*1*/	_i{ _o::BPL, AD_REL, 2 }, { _o::ORA, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::ORA, AD_ZPX, 4 }, { _o::ASL, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::CLC ,AD_IMP, 2 }, { _o::ORA, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::ORA, AD_ABX, 4 }, { _o::ASL, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
			/*2*/	_i{ _o::JSR, AD_ABS, 6 }, { _o::AND, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::BIT, AD_ZPG, 3 }, { _o::AND, AD_ZPG, 3 }, { _o::ROL, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PLP, AD_IMP, 4 }, { _o::AND, AD_IMM, 2 }, { _o::ROL, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::BIT, AD_ABS, 4 }, { _o::AND, AD_ABS, 4 }, { _o::ROL, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
			/*3*/	_i{ _o::BMI, AD_REL, 2 }, { _o::AND, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::AND, AD_ZPX, 4 }, { _o::ROL, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::SEC, AD_IMP, 2 }, { _o::AND, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::AND, AD_ABX, 4 }, { _o::ROL, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
			/*4*/	_i{ _o::RTI, AD_IMP, 6 }, { _o::EOR, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::INV, AD_ZPG, 3 }, { _o::EOR, AD_ZPG, 3 }, { _o::LSR, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PHA, AD_IMP, 3 }, { _o::EOR, AD_IMM, 2 }, { _o::LSR, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::JMP, AD_ABS, 3 }, { _o::EOR, AD_ABS, 4 }, { _o::LSR, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
			/*5*/	_i{ _o::BVC, AD_REL, 2 }, { _o::EOR, AD_INY, 5 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INY, 8 }, { _o::INV, AD_ZPX, 4 }, { _o::EOR, AD_ZPX, 4 }, { _o::LSR, AD_ZPX, 6 }, { _o::INV, AD_ZPX, 6 }, { _o::CLI, AD_IMP, 2 }, { _o::EOR, AD_ABY, 4 }, { _o::INV, AD_IMP, 2 }, { _o::INV, AD_ABY, 7 }, { _o::INV, AD_ABX, 4 }, { _o::EOR, AD_ABX, 4 }, { _o::LSR, AD_ABX, 7 }, { _o::INV, AD_ABX, 7 },
			/*6*/	_i{ _o::RTS, AD_IMP, 6 }, { _o::ADC, AD_INX, 6 }, { _o::INV, AD_OTH, 2 }, { _o::INV, AD_INX, 8 }, { _o::INV, AD_ZPG, 3 }, { _o::ADC, AD_ZPG, 3 }, { _o::ROR, AD_ZPG, 5 }, { _o::INV, AD_ZPG, 5 }, { _o::PLA, AD_IMP, 4 }, { _o::ADC, AD_IMM, 2 }, { _o::ROR, AD_ACC, 2 }, { _o::INV, AD_IMM, 2 }, { _o::JMP, AD_IND, 5 }, { _o::ADC, AD_ABS, 4 }, { _o::ROR, AD_ABS, 6 }, { _o::INV, AD_ABS, 6 },
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

	#ifdef NES_EM_INIT_STRING_REPRESENTATION
	// String representation of all the opcodes, useful for debugging, disassembly,...
	// Put in alphabetical for easy of use, index in array also commented next to each item  
	// https://www.masswerk.at/6502/6502_instruction_set.html -> Instructions by Name
	inline static constexpr std::array<const char*, 57> OPCODES_6502_NAMES{
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
		// Functions return wether or not cycles may change
		// when certain criteria are (not) met
		// Param Instruction: the specific instruction (since e.g ADC can be called via different address modes)
		// Param Address: The address the opcode acts on if necessary
			//[[maybe_unused]] Instruction const& instruction, [[maybe_unused]] uint16_t address
		inline static bool ADC() noexcept
		{
			return true;
		}

		inline static bool AND() noexcept
		{
			return true;
		}

		inline static bool ASL() noexcept
		{
			return true;
		}

		inline static bool BCC() noexcept
		{
			return true;
		}

		inline static bool BCS() noexcept
		{
			return true;
		}

		inline static bool BEQ() noexcept
		{
			return true;
		}

		inline static bool BIT() noexcept
		{
			return true;
		}

		inline static bool BMI() noexcept
		{
			return true;
		}

		inline static bool BNE() noexcept
		{
			return true;
		}

		FORCE_INLINE inline static bool BPL() noexcept
		{
			return true;
		}

		inline static bool BRK() noexcept
		{
			return true;
		}

		inline static bool BVC() noexcept
		{
			return true;
		}

		inline static bool BVS() noexcept
		{
			return true;
		}

		inline static bool CLC() noexcept
		{
			return true;
		}

		inline static bool CLD() noexcept
		{
			return true;
		}

		inline static bool CLI() noexcept
		{
			return true;
		}

		inline static bool CLV() noexcept
		{
			return true;
		}

		inline static bool CMP() noexcept
		{
			return true;
		}

		inline static bool CPX() noexcept
		{
			return true;
		}

		inline static bool CPY() noexcept
		{
			return true;
		}

		inline static bool DEC() noexcept
		{
			return true;
		}

		inline static bool DEX() noexcept
		{
			return true;
		}

		inline static bool DEY() noexcept
		{
			return true;
		}

		inline static bool EOR() noexcept
		{
			return true;
		}

		inline static bool INC() noexcept
		{
			return true;
		}

		inline static bool INX() noexcept
		{
			return true;
		}

		inline static bool INY() noexcept
		{
			return true;
		}

		inline static bool JMP() noexcept
		{
			return true;
		}

		inline static bool JSR() noexcept
		{
			return true;
		}

		inline static bool LDA() noexcept
		{
			return true;
		}

		inline static bool LDX() noexcept
		{
			return true;
		}

		inline static bool LDY() noexcept
		{
			return true;
		}

		inline static bool LSR() noexcept
		{
			return true;
		}

		inline static bool NOP() noexcept
		{
			return true;
		}

		inline static bool ORA() noexcept
		{
			return true;
		}

		inline static bool PHA() noexcept
		{
			return true;
		}

		inline static bool PHP() noexcept
		{
			return true;
		}

		inline static bool PLA() noexcept
		{
			return true;
		}

		inline static bool PLP() noexcept
		{
			return true;
		}

		inline static bool ROL() noexcept
		{
			return true;
		}

		inline static bool ROR() noexcept
		{
			return true;
		}

		inline static bool RTI() noexcept
		{
			return true;
		}

		inline static bool RTS() noexcept
		{
			return true;
		}

		inline static bool SBC() noexcept
		{
			return true;
		}

		inline static bool SEC() noexcept
		{
			return true;
		}

		inline static bool SED() noexcept
		{
			return true;
		}

		inline static bool SEI() noexcept
		{
			return true;
		}

		inline static bool STA() noexcept
		{
			return true;
		}

		inline static bool STX() noexcept
		{
			return true;
		}

		inline static bool STY() noexcept
		{
			return true;
		}

		inline static bool TAX() noexcept
		{
			return true;
		}

		inline static bool TAY() noexcept
		{
			return true;
		}

		inline static bool TSX() noexcept
		{
			return true;
		}

		inline static bool TXA() noexcept
		{
			return true;
		}

		inline static bool TXS() noexcept
		{
			return true;
		}

		inline static bool TYA() noexcept
		{
			return true;
		}

		// Invalid opcode
		inline static bool INV() noexcept
		{
			return true;
		}
#pragma endregion
		// Table of function pointers for each opcode
		using OpcodeFunction = bool (*)();
		inline static constexpr std::array<OpcodeFunction, 57> OPCODES_6502_FUNCTIONS
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