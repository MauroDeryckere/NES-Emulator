#include "CPU.h"

// 6502 opcodes & addressing stuff
#include "OpCodes.h"

#include <iostream>

namespace NesEm
{
	inline uint16_t CPU::GetAddress(AddressingMode mode, uint8_t& cycles) noexcept
	{
		switch (mode)
		{
		case AddressingMode::Accumulator:
			return 0;

		case AddressingMode::Relative:
			return 0;

		case AddressingMode::Immediate:
			return 0;

		case AddressingMode::ZeroPage:
			return 0;

		case AddressingMode::ZeroPageX:
			return 0;

		case AddressingMode::ZeroPageY:
			return 0;
		
		case AddressingMode::Absolute:
		{
			uint16_t lo = Fetch();
			uint16_t hi = Fetch();
			return (hi << 8) | lo;
			
		}
		
		case AddressingMode::AbsoluteX:
			return 0;

		case AddressingMode::AbsoluteY:
			return 0;

		case AddressingMode::Indirect:
			return 0;

		case AddressingMode::IndirectX:
			return 0;

		case AddressingMode::IndirectY:
			return 0;

		default: break;
		}

	}
	inline uint8_t CPU::Fetch() noexcept
	{
		return m_Memory.Read(m_ProgramCounter++);
	}

	void CPU::Clock() noexcept
	{
		if (m_CurrCycles == 0)
		{
			uint8_t const opcode{ Fetch() };

			// Get correct code from table
							
			// Update cycles based on instruction from the table

			// Call the address mode function

			// Call the opcode function
		}


		
	}
}
