#include "OpcodeHandler.h"

#include "NESCPU.h"

#include "SDL3/SDL_log.h"

#include <iostream>
#include <limits>
#include <cassert>

namespace NesEm
{
	uint8_t OpcodeHandler::ExecuteOpcode(uint8_t opcode, CPU& cpu) const noexcept
    {
        auto const& [instructionID, addressMode, cycles] { OPCODES_6502[opcode] };

        // Call the address mode function to see by how much our cycles should be adjusted
        // And to adjust the address we are using if necessary
		uint16_t address{ 0 };
		[[maybe_unused]] uint8_t const addedCycles{ HandleAddressMode(addressMode, cpu, address) };

		// Call the specific opcode function
		// Opcode returns wether or not we should keep the added cycles
		static_assert(std::is_enum_v<decltype(instructionID)>);
		static_assert(std::is_unsigned_v<std::underlying_type_t<decltype(instructionID)>>);

        // We only want to add the cycles if the instruction requires this.
        if (OPCODES_6502_FUNCTIONS[static_cast<std::underlying_type_t<decltype(instructionID)>>(instructionID)](cpu, address, addressMode))
        {
			assert(addedCycles != std::numeric_limits<uint8_t>::max());
			assert(addressMode == AddressingMode::AbsoluteX 
				|| addressMode == AddressingMode::AbsoluteX 
				|| addressMode == AddressingMode::IndirectY 
				|| addressMode == AddressingMode::Relative);

			return cycles + addedCycles;
        }

        return cycles;
    }

	uint8_t OpcodeHandler::HandleAddressMode(AddressingMode mode, [[maybe_unused]] CPU& cpu, [[maybe_unused]] uint16_t& address) const noexcept
    {
		// More information for each address mode in more detail can be found at
		// https://www.masswerk.at/6502/6502_instruction_set.html
		// "6502 Address Modes in Detail"
	    switch (mode)
	    {
			case AddressingMode::Accumulator:
			{
				// No address is needed; the accumulator (A) is implicitly used
				return 0; 
			} break;

			case AddressingMode::Absolute:
			//Example:
			// Low: 0x0010 | High : 0x0030 << 8
			// -> 0x0010 | 0x3000
			// Address = 0x3010
			{
				// Read the low and high byte respectively to form a 16-bit address
				// LL | HH
				address = cpu.Read() | (cpu.Read() << 8);
				return 0;
			} break;

			case AddressingMode::AbsoluteX:
			//Example:
			// Low: 0x0010 | High : 0x0010 << 8
			// -> 0x0010 | 0x1000
			// Address = 0x1010
			// Y reg: 0x0001
			// Address == 0x1011
				// Page boundrary check : 0x0010 + 0x0001 > 0x00FF -> false
				// Alternative page boundrary check: 0x00FF + 0x0001 > 0x00FF -> true
			{
				// Read the low and high byte respectively to form a 16-bit address
				// And offset by X reg
				auto const lowByte{ cpu.Read() };
				// LL | HH
				address = (lowByte | (cpu.Read() << 8)) + cpu.m_XRegister;

				// Did we cross a page boundrary or not? -> Possibly add a cycle if we do
				return ((lowByte + cpu.m_XRegister) > 0x00FF);
			} break;

			case AddressingMode::AbsoluteY:
			//Example:
			// Low: 0x0010 | High : 0x0010 << 8
			// -> 0x0010 | 0x1000
			// Address = 0x1010
			// Y reg: 0x0001
			// Address == 0x1011
				// Page boundrary check : 0x0010 + 0x0001 > 0x00FF -> false
				// Alternative page boundrary check: 0x00FF + 0x0001 > 0x00FF -> true
			{
				// Read the low and high byte respectively to form a 16-bit address
				// And offset by Y reg
				auto const lowByte{ cpu.Read() };
				// LL | HH
				address = (lowByte | (cpu.Read() << 8)) + cpu.m_YRegister;

				// Did we cross a page boundrary or not? -> Possibly add a cycle if we do
				return ((lowByte + cpu.m_YRegister) > 0x00FF);
			} break;

			case AddressingMode::Immediate:
			//Example: 
			// LDA #2
			// PC set to the byte of "#2", when we execute LDA it will read this value and load it into the accumulator
			{
				// The instruction expects the next byte to be used as a value so we do have to adjust the program counter
				// Set the address to the current program counter since that is where we will read the immediate value
				address = cpu.m_ProgramCounter++;
				return 0;
			} break;

	    	case AddressingMode::Implied:
			{
				// These instructions act directly on one or more registers or flags
				// internal to the CPU. Therefor, these instructions are principally
				// single-byte instructions, lacking an explicit operand. The operand
				// is implied, as it is already provided by the very instruction.
	    		return 0;
			} break;

	    	case AddressingMode::Indirect:
			//Example:
			// JMP ($FF82) ; Jump to address given in locations "$FF82" and "$FF83"
			// Lookup $FF82 -> "C4 80" ; note: instructions are noted as LL HH and are then converted to HH LL
			// -> Effective target: $80C4
			{
				// Read the low and high byte respectively to form a 16-bit address
	    		// This is an indirect address and will be used to read the actual required value
				address = cpu.Read() | (cpu.Read() << 8);

				// Page boundrary hardware bug
				// http://www.6502.org/tutorials/6502opcodes.html#JMP
				// https://www.reddit.com/r/EmuDev/comments/fi29ah/6502_jump_indirect_error/
	    		if (address == 0x00FF)
				{
					// Read the address through the indirect address or "dereference" the address
					// LL | HH
					address = cpu.Read(address) | (cpu.Read(address & 0xFF00) << 8);
				}
			    else
			    {
		    		// Read the address through the indirect address or "dereference" the address
					// LL | HH
		    		address = cpu.Read(address) | (cpu.Read(address + 1) << 8);
			    }
				return 0;
			} break;

	    	case AddressingMode::IndirectX:
			//Example:
			// LDA ($70, X)
			// X = 5
			// ZPG address = $70 + X = $0075
			// Lookup at $0075
			// Actual address is found at $0075 and $0075 + 1
			// Offsetted address has to be wrapped around same as ZPG, X and ZPG, Y
			{
				// Calculate the zeropage address and offset it by X (same as ZPX address mode)
				address = (cpu.Read() + cpu.m_XRegister);

				// Set the address to whatever is located at indirection of offsetted ZPX ("Dereference")
				// LL | HH
				// &0x00FF to wrap around (stay in zero page), we do it here because address + 1 could wrap around even tho address + 0 does not 
				address = cpu.m_Memory.Read(address & 0x00FF) | (cpu.m_Memory.Read((address + 1) & 0x00FF) << 8);

				return 0;
			} break;

	    	case AddressingMode::IndirectY:
			//Example:
			// LDA ($70), Y
			// Lookup at $0070
			// E.g $3543
			// Y == 10
			// Offset address by 10
			// Address is $3553
			{
				// Calculate the ZPG address
				address = cpu.Read();

				// Set the address to whatever is located at indirection ("Dereference")
				// LL | HH
	    		auto const lowByte{ cpu.m_Memory.Read(address) };
	    		address = lowByte | (cpu.m_Memory.Read((address + 1) & 0x00FF) << 8); // it is still possible that the +1 wraps around

	    		// Offset the address by Y
				address += cpu.m_YRegister;

				// Did we cross the boundrary? 
				return ((lowByte + cpu.m_YRegister) > 0x00FF);
			} break;

	    	case AddressingMode::Relative:
			//Example:
			// PC is 0x1000
			// Offset is 0x10 (offsets are signed so this is +16 decimal - 2s complement)
			// Address = 0x1000 + 0x10 = 0x1010
				// Alternative outcome: 
				// Pc is 0x1000
				// Offset is 0xF0 (offsets are signed so this is - 16 decimal ( 2s complement ))
				// Address = 0x1000 + 0xF0 = 0x0FF0 (6502 will do thhis calculation using 2s compliment but since were humans , 4096 - 16 = 4080 -> 0x0FF0)
			{
				// Here, the instruction provides only a relative offset,
				// which is added to the contents of the program counter (PC)
	    		// Clamp [-128, +127]]

				// Read the byte, pc ++ , new pc + byte == rel address
				//C++ does this conversion the following way:
				// https://godbolt.org/z/WovWxhGa1 
				// uint8_t{ 0 } casted to int8_t is 0
				// uint8_t{ 127 } casted to int8_t is 127
				// uint8_t{ 128 } casted to int8_t is -128
				// uint8_t{ 255 } casted to int8_t is -1
				// Which is what we need for our 6502 emulation
				int8_t const offset{ static_cast<int8_t>(cpu.Read()) };

				// In this case our address (out) is set to the relative address
				address = cpu.m_ProgramCounter + offset;

				// Check if the branch occurs on the same page or a different page
				uint8_t const currentPage{ static_cast<uint8_t>(cpu.m_ProgramCounter >> 8) }; // high byte of current PC
				uint8_t const targetPage{ static_cast<uint8_t>(address >> 8) };				  // high byte of target address

				if (currentPage != targetPage) 
				{
					// add 2 to cycles if branch occurs to different page
					return 2;
				}
	    		// add 1 to cycle if branch occurs on same page
				return 1;

	    		// Note: In this case we only want to add cycles if the branch actually happens
				// Which we can only know during the actual opcode function
				// -> Opcode returns true or false
			} break;

	    	case AddressingMode::ZeroPage:
			//Example:
			// Address == 100 (value 100 is read at PC)
			// Instruction will use address 100
			{
				// Read the ZPG address
				address = cpu.Read();
				return 0;
			} break;

	    	case AddressingMode::ZeroPageX:
			//Example:
			// Address == 100 (value 100 is read at PC)
			// X == 200
			// 100 + 200 = 300 -> 300 (0x012C) &= 0x00FF = 44 (0x002C)
			// Instruction will use address 44 (0x002C)
			{
				// Read the ZPG address, offset it by X reg and & it to wrap around
				address = (cpu.Read() + cpu.m_XRegister) & 0x00FF;
				return 0;
			} break;

	    	case AddressingMode::ZeroPageY:
			//Example:
			// Address == 100 (value 100 is read at PC)
			// Y == 200
			// 100 + 200 = 300 -> 300 (0x012C) &= 0x00FF = 44 (0x002C)
			// Instruction will use address 44 (0x002C)
			{
				// Read the ZPG address, offset it by Y reg and & it to wrap around
				address = (cpu.Read() + cpu.m_YRegister) & 0x00FF;
				return 0;
			} break;

	    	case AddressingMode::Other:
			{
				assert(false);
				// Set address to 0xFFFF to make it easier to debug if we ever accidentally enter this address mode
				address = 0xFFFF;
				return std::numeric_limits<uint8_t>::max();
			} break;

	    	default: break;
	    }

        return std::numeric_limits<uint8_t>::max();
    }

#pragma region OpcodeFunctions
	FORCE_INLINE bool OpcodeHandler::ADC(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		return false;
	}
	FORCE_INLINE bool OpcodeHandler::AND(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		return false;
	}
	FORCE_INLINE bool OpcodeHandler::ASL(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		return false;
	}

	FORCE_INLINE bool OpcodeHandler::BCC(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Relative && "Branching onl supports relative address mode");

		// branch on C = 0
		if (not cpu.IsFlagSet(CPU::StatusFlags::C))
		{
			// the address mode function handles setting our address to the relative address
			cpu.m_ProgramCounter = address;

			//Flags: 
			// N Z C I D V
			// - - - - - -

			// Branch takes an extra cycle when it's executed and another cycle when boundrary is crossed
			return true;
		}

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::BCS(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Relative && "Branching onl supports relative address mode");

		// branch on C = 1
		if (cpu.IsFlagSet(CPU::StatusFlags::C))
		{
			// the address mode function handles setting our address to the relative address
			cpu.m_ProgramCounter = address;

			//Flags: 
			// N Z C I D V
			// - - - - - -

			// Branch takes an extra cycle when it's executed and another cycle when boundrary is crossed
			return true;
		}

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::BEQ(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Relative && "Branching onl supports relative address mode");

		// branch on Z = 1
		if (cpu.IsFlagSet(CPU::StatusFlags::Z))
		{
			// the address mode function handles setting our address to the relative address
			cpu.m_ProgramCounter = address;

			//Flags: 
			// N Z C I D V
			// - - - - - -

			// Branch takes an extra cycle when it's executed and another cycle when boundrary is crossed
			return true;
		}

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::BIT(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		return false;
	}

	FORCE_INLINE bool OpcodeHandler::BMI(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Relative && "Branching onl supports relative address mode");

		// branch on N = 1
		if (cpu.IsFlagSet(CPU::StatusFlags::N))
		{
			// the address mode function handles setting our address to the relative address
			cpu.m_ProgramCounter = address;

			//Flags: 
			// N Z C I D V
			// - - - - - -

			// Branch takes an extra cycle when it's executed and another cycle when boundrary is crossed
			return true;
		}

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::BNE(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Relative && "Branching onl supports relative address mode");

		// branch on Z = 0
		if (not cpu.IsFlagSet(CPU::StatusFlags::Z))
		{
			// the address mode function handles setting our address to the relative address
			cpu.m_ProgramCounter = address;

			//Flags: 
			// N Z C I D V
			// - - - - - -

			// Branch takes an extra cycle when it's executed and another cycle when boundrary is crossed
			return true;
		}

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::BPL(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Relative && "Branching onl supports relative address mode");

		// branch on N = 0
		if (not cpu.IsFlagSet(CPU::StatusFlags::N))
		{
			// the address mode function handles setting our address to the relative address
			cpu.m_ProgramCounter = address;

			//Flags: 
			// N Z C I D V
			// - - - - - -

			// Branch takes an extra cycle when it's executed and another cycle when boundrary is crossed
			return true;
		}

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::BRK(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Only implied address mode is allowed for BRK instruction");

		// Force Break
		// BRK initiates a software interrupt similar to a hardware
		// interrupt(IRQ).The return address pushed to the stack is
		// PC + 2, providing an extra byte of spacing for a break mark
		// (identifying a reason for the break.)
		// The status register will be pushed to the stack with the break
		// flag set to 1. However, when retrieved during RTI or by a PLP
		// instruction, the break flag will be ignored.
		// The interrupt disable flag is not set automatically.

		// interrupt, push PC + 2, push SR
		++cpu.m_ProgramCounter; // PC + 2 means we need an additional increase for our PC first

		cpu.Push((cpu.m_ProgramCounter >> 8) & 0xFF); // High
		cpu.Push(cpu.m_ProgramCounter & 0xFF); // Low

		//Flags: 
		// N Z C I D V
		// - - - 1 - -
		// https://www.masswerk.at/6502/6502_instruction_set.html#break-flag

		cpu.SetFlag(CPU::StatusFlags::U); // Unused flag should always be set when pushing P to the stack
		cpu.SetFlag(CPU::StatusFlags::I); // "Prevent further IRQs from interrupting execution."

		// Break flag is set before we push the status register
		cpu.SetFlag(CPU::StatusFlags::B);
		cpu.Push(cpu.m_StatusRegister);
		// But is cleared after again
		cpu.ClearFlag(CPU::StatusFlags::B);

		// Load new PC from IRQ/BRK vector at $FFFE/$FFFF
		cpu.m_ProgramCounter = (cpu.Read(0xFFFF) << 8) | cpu.Read(0xFFFE);

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::BVC(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Relative && "Branching onl supports relative address mode");

		// branch on V = 0
		if (not cpu.IsFlagSet(CPU::StatusFlags::V))
		{
			// the address mode function handles setting our address to the relative address
			cpu.m_ProgramCounter = address;

			//Flags: 
			// N Z C I D V
			// - - - - - -

			// Branch takes an extra cycle when it's executed and another cycle when boundrary is crossed
			return true;
		}

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::BVS(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Relative && "Branching onl supports relative address mode");

		// branch on V = 1
		if (cpu.IsFlagSet(CPU::StatusFlags::V))
		{
			// the address mode function handles setting our address to the relative address
			cpu.m_ProgramCounter = address;

			//Flags: 
			// N Z C I D V
			// - - - - - -

			// Branch takes an extra cycle when it's executed and another cycle when boundrary is crossed
			return true;
		}

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::CLC(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Clear flag instructions only allows implied address modes");

		// 0 -> C

		//Flags:
		// N Z C I D V
		// - - 0 - - -

		cpu.ClearFlag(CPU::StatusFlags::C);

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::CLD(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Clear flag instructions only allows implied address modes");

		// 0 -> D

		//Flags:
		// N Z C I D V
		// - - - - 0 -

		cpu.ClearFlag(CPU::StatusFlags::D);

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::CLI(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Clear flag instructions only allows implied address modes");

		// 0 -> I

		//Flags:
		// N Z C I D V
		// - - - 0 - -

		cpu.ClearFlag(CPU::StatusFlags::I);

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::CLV(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Clear flag instructions only allows implied address modes");

		// 0 -> V

		//Flags:
		// N Z C I D V
		// - - - - - 0

		cpu.ClearFlag(CPU::StatusFlags::V);

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::CMP(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Immediate
			|| mode == AddressingMode::ZeroPage
			|| mode == AddressingMode::ZeroPageX
			|| mode == AddressingMode::Absolute
			|| mode == AddressingMode::AbsoluteX
			|| mode == AddressingMode::AbsoluteY
			|| mode == AddressingMode::IndirectX
			|| mode == AddressingMode::IndirectY && "unsupported address mode for CMP instruction");

		// A - M
		uint8_t const operand = cpu.Read(address);
		auto const result{ cpu.m_Accumulator - operand };

		//Flags:
		// N Z C I D V
		// + + + - - -

		// Carry Flag (C) - Set if A >= M (i.e., no borrow)
		cpu.SetOrClearFlag(CPU::StatusFlags::C, cpu.m_Accumulator >= operand);

		// Zero Flag (Z) - Set if (A - M) == 0
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not result));

		// Negative Flag (N) - Set if bit 7 of the result is set (result is negative)
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (result & 0b1000'0000));

		// CMP takes an extra cycle when crossing boundraries
		return true;
	}

	FORCE_INLINE bool OpcodeHandler::CPX(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Immediate
			  || mode == AddressingMode::ZeroPage
			  || mode == AddressingMode::Absolute && "unsupported address mode for CPX instruction");

		// X - M
		uint8_t const operand = cpu.Read(address);
		auto const result{ cpu.m_XRegister - operand };

		//Flags:
		// N Z C I D V
		// + + + - - -

		// Carry Flag (C) - Set if X >= M (i.e., no borrow)
		cpu.SetOrClearFlag(CPU::StatusFlags::C, cpu.m_XRegister >= operand);

		// Zero Flag (Z) - Set if (X - M) == 0
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not result));

		// Negative Flag (N) - Set if bit 7 of the result is set (result is negative)
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (result & 0b1000'0000));

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::CPY(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Immediate
			|| mode == AddressingMode::ZeroPage
			|| mode == AddressingMode::Absolute && "unsupported address mode for CPY instruction");

		// Y - M
		uint8_t const operand = cpu.Read(address);
		auto const result{ cpu.m_YRegister - operand };

		//Flags:
		// N Z C I D V
		// + + + - - -

		// Carry Flag (C) - Set if Y >= M (i.e., no borrow)
		cpu.SetOrClearFlag(CPU::StatusFlags::C, cpu.m_YRegister >= operand);

		// Zero Flag (Z) - Set if (Y - M) == 0
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not result));

		// Negative Flag (N) - Set if bit 7 of the result is set (result is negative)
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (result & 0b1000'0000));

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::DEC(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::ZeroPage
			|| mode == AddressingMode::ZeroPageX
			|| mode == AddressingMode::Absolute
			|| mode == AddressingMode::AbsoluteX && "unsupported address mode for DEC instruction");


		//M - 1 -> M
		auto const newValue{ cpu.Read(address) - 1};
		cpu.Write(address, newValue);

		//Flags:
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not newValue)); // check if val reg is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (newValue & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::DEX(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "DEX only supports implied address mode");

		// X - 1 -> X
		--cpu.m_XRegister;

		//Flags:
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_XRegister)); // check if X reg is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_XRegister & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::DEY(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "DEY only supports implied address mode");

		// X - 1 -> Y
		--cpu.m_YRegister;

		//Flags:
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_YRegister)); // check if Y reg is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_YRegister & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::EOR(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		return false;
	}

	FORCE_INLINE bool OpcodeHandler::INC(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::ZeroPage
			|| mode == AddressingMode::ZeroPageX
			|| mode == AddressingMode::Absolute
			|| mode == AddressingMode::AbsoluteX && "unsupported address mode for INC instruction");


		//M + 1 -> M
		auto const newValue{ cpu.Read(address) + 1 };
		cpu.Write(address, newValue);

		//Flags:
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not newValue)); // check if val reg is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (newValue & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::INX(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "INX only supports implied address mode");

		// X + 1 -> X
		++cpu.m_XRegister;

		//Flags:
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_XRegister)); // check if X reg is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_XRegister & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::INY(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "INY only supports implied address mode");

		// Y + 1 -> Y
		++cpu.m_YRegister;

		//Flags:
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_YRegister)); // check if Y reg is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_YRegister & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::JMP(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Absolute 
			 || mode == AddressingMode::Indirect && "JMP only supports absolute and indirect address mode");

		// operand 1st byte -> PCL
		// operand 2nd byte -> PCH
		// But this was already set in the address mode function so we can just set our address

		cpu.m_ProgramCounter = address;

		//Flags:
		// N Z C I D V
		// - - - - - -

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::JSR(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Absolute && "JSR only supports absolute and indirect address mode");

		// push(PC + 2)
		// PC + 2 means we need an additional increase for our PC first
		// But Absolute address mode increases our PC twice so we should decrease the current PC before we push
		--cpu.m_ProgramCounter;

		cpu.Push((cpu.m_ProgramCounter >> 8) & 0xFF); // High
		cpu.Push(cpu.m_ProgramCounter & 0xFF); // Low

		// operand 1st byte->PCL
		// operand 2nd byte->PCH
		// But this was already set in the address mode function so we can just set our address

		cpu.m_ProgramCounter = address;

		//Flags:
		// N Z C I D V
		// - - - - - -

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::LDA(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Immediate
			|| mode == AddressingMode::ZeroPage
			|| mode == AddressingMode::ZeroPageX
			|| mode == AddressingMode::Absolute
			|| mode == AddressingMode::AbsoluteX
			|| mode == AddressingMode::AbsoluteY
			|| mode == AddressingMode::IndirectX
			|| mode == AddressingMode::IndirectY && "unsupported address mode for LDA instruction");

		// M -> A
		cpu.m_Accumulator = cpu.Read(address);

		//Flags: 
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_Accumulator)); // check if accumulator is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_Accumulator & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		// LDA instruction possibly takes an extra cycle 
		return true;
	}

	FORCE_INLINE bool OpcodeHandler::LDX(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Immediate
			|| mode == AddressingMode::ZeroPage
			|| mode == AddressingMode::ZeroPageY
			|| mode == AddressingMode::Absolute
			|| mode == AddressingMode::AbsoluteY && "unsupported address mode for LDX instruction");

		// M -> X
		cpu.m_XRegister = cpu.Read(address);

		//Flags: 
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_XRegister)); // check if X reg is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_XRegister & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		// LDX instruction possibly takes an extra cycle 
		return true;
	}

	FORCE_INLINE bool OpcodeHandler::LDY(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Immediate
			|| mode == AddressingMode::ZeroPage
			|| mode == AddressingMode::ZeroPageX
			|| mode == AddressingMode::Absolute
			|| mode == AddressingMode::AbsoluteX && "unsupported address mode for LDY instruction");

		// M -> Y
		cpu.m_YRegister = cpu.Read(address);

		//Flags: 
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_YRegister)); // check if Y reg is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_YRegister & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		// LDY instruction possibly takes an extra cycle 
		return true;
	}

	FORCE_INLINE bool OpcodeHandler::LSR(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		return false;
	}

	FORCE_INLINE bool OpcodeHandler::NOP(CPU&, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "NOP only supports implied address mode");
		// No operation, does nothing

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::ORA(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		return false;
	}

	FORCE_INLINE bool OpcodeHandler::PHA(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Only implied address mode is supported for stack operation");

		// push A
		cpu.Push(cpu.m_Accumulator);

		//Flags: 
		// N Z C I D V
		// - - - - - -

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::PHP(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Only implied address mode is supported for stack operation");

		// The status register will be pushed with the break flag and bit 5 set to 1.
		// push SR
		cpu.SetFlag(CPU::StatusFlags::B);
		cpu.SetFlag(CPU::StatusFlags::U);

		cpu.Push(cpu.m_StatusRegister);

		cpu.ClearFlag(CPU::StatusFlags::B);

		//Flags: 
		// N Z C I D V
		// - - - - - -

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::PLA(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Only implied address mode is supported for stack operation");

		cpu.m_Accumulator = cpu.Pop();

		//Flags: 
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_Accumulator)); // check if accumulator is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_Accumulator & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::PLP(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Only implied address mode is supported for stack operation");

		// The status register will be pulled with the break flag and bit 5 ignored.
		// pull SR
		cpu.m_StatusRegister = cpu.Pop();

		cpu.ClearFlag(CPU::StatusFlags::B);
		cpu.SetFlag(CPU::StatusFlags::U); // Unused bit is always set to 1 

		//Flags: 
		// N Z C I D V
		// From stack

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::ROL(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		return false;
	}
	FORCE_INLINE bool OpcodeHandler::ROR(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		return false;
	}

	FORCE_INLINE bool OpcodeHandler::RTI(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Only implied addressing mode is supported by RTI");

		// The status register is pulled with the break flag and bit 5 ignored.Then PC is pulled from the stack.
		// pull SR, pull PC
		cpu.m_StatusRegister  = cpu.Pop();
		cpu.ClearFlag(CPU::StatusFlags::B);
		cpu.SetFlag(CPU::StatusFlags::U); // Unused bit is always set to 1 

		// LL | HH
		cpu.m_ProgramCounter = cpu.Pop() | (cpu.Pop() << 8);

		// N Z C I D V
		// From stack

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::RTS(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Only implied addressing mode is supported by RTS");

		// pull PC, PC+1 -> PC
		// LL | HH
		cpu.m_ProgramCounter = cpu.Pop() | (cpu.Pop() << 8);

		//Flags: 
		// N Z C I D V
		// - - - - - -

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::SBC(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		return false;
	}

	FORCE_INLINE bool OpcodeHandler::SEC(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Set flag instructions only allows implied address modes");

		// 1 -> C

		//Flags:
		// N Z C I D V
		// - - 1 - - -

		cpu.SetFlag(CPU::StatusFlags::C);

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::SED(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Set flag instructions only allows implied address modes");

		// 1 -> D

		//Flags:
		// N Z C I D V
		// - - - - 1 -

		cpu.SetFlag(CPU::StatusFlags::D);

		return false;
	}
	FORCE_INLINE bool OpcodeHandler::SEI(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Set flag instructions only allows implied address modes");

		// 1 -> I

		//Flags:
		// N Z C I D V
		// - - - 1 - -

		cpu.SetFlag(CPU::StatusFlags::I);

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::STA(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::ZeroPage
			|| mode == AddressingMode::ZeroPageX
			|| mode == AddressingMode::Absolute
			|| mode == AddressingMode::AbsoluteX
			|| mode == AddressingMode::AbsoluteY
			|| mode == AddressingMode::IndirectX
			|| mode == AddressingMode::IndirectY && "STA only allows ZPG, ZPX, ABS, ABX, ABY, INX or INY address mode");

		// A -> M
		cpu.Write(address, cpu.m_Accumulator);

		//Flags:
		// N Z C I D V
		// - - - - - -

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::STX(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::ZeroPage 
			|| mode == AddressingMode::ZeroPageY 
			|| mode == AddressingMode::Absolute && "STX only allows ZPG, ZPY or ABS address mode");

		// X -> M
		cpu.Write(address, cpu.m_XRegister);

		//Flags:
		// N Z C I D V
		// - - - - - -

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::STY(CPU& cpu, uint16_t address, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::ZeroPage
			|| mode == AddressingMode::ZeroPageX
			|| mode == AddressingMode::Absolute && "STY only allows ZPG, ZPX or ABS address mode");

		// Y -> M
		cpu.Write(address, cpu.m_YRegister);

		//Flags:
		// N Z C I D V
		// - - - - - -

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::TAX(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Transfer opcodes only allow implied address modes");

		// A -> X
		cpu.m_XRegister = cpu.m_Accumulator;

		//Flags: 
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_XRegister)); // check if X register is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_XRegister & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::TAY(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Transfer opcodes only allow implied address modes");

		// A -> Y
		cpu.m_YRegister = cpu.m_Accumulator;

		//Flags: 
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_YRegister)); // check if Y register is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_YRegister & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::TSX(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Transfer opcodes only allow implied address modes");

		// SP -> X
		cpu.m_XRegister = cpu.m_StackPointer;

		//Flags: 
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_XRegister)); // check if X register is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_XRegister & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::TXA(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Transfer opcodes only allow implied address modes");

		// X -> A
		cpu.m_Accumulator = cpu.m_XRegister;

		//Flags: 
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_Accumulator)); // check if accumulator is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_Accumulator & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::TXS(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Transfer opcodes only allow implied address modes");

		// X -> SP
		cpu.m_StackPointer = cpu.m_XRegister;

		//Flags:
		// N Z C I D V
		// - - - - - - 

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::TYA(CPU& cpu, uint16_t, [[maybe_unused]] AddressingMode mode) noexcept
	{
		assert(mode == AddressingMode::Implied && "Transfer opcodes only allow implied address modes");

		// Y -> A
		cpu.m_Accumulator = cpu.m_YRegister;

		//Flags: 
		// N Z C I D V
		// + + - - - -
		cpu.SetOrClearFlag(CPU::StatusFlags::Z, (not cpu.m_Accumulator)); // check if accumulator is 0 or not, if 0 -> zero flag -> true, else -> false
		cpu.SetOrClearFlag(CPU::StatusFlags::N, (cpu.m_Accumulator & 0b1000'0000)); // check negative bit, if negative bit is set, negative flag -> true; else -> false

		return false;
	}

	FORCE_INLINE bool OpcodeHandler::INV(CPU&, uint16_t, AddressingMode) noexcept
	{
		//We don't do anything with illegal opcodes at the moment.
		#if NES_EM_DEBUG_MODE
			SDL_Log("%s", "Invalid opcode executed.");
		#endif

		return false;
	}
#pragma endregion
}