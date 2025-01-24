#include "CPU.h"
#include "CPU.h"
#include "CPU.h"
#include "CPU.h"

namespace NesEm
{
	inline uint16_t CPU::GetAddress(AddressingMode mode) noexcept
	{
		switch (mode)
		{
		case CPU::AddressingMode::Accumulator:
			return 0;

		case CPU::AddressingMode::Relative:
			return 0;

		case CPU::AddressingMode::Immediate:
			return 0;

		case CPU::AddressingMode::ZeroPage:
			return 0;

		case CPU::AddressingMode::ZeroPageX:
			return 0;

		case CPU::AddressingMode::ZeroPageY:
			return 0;
		
		case CPU::AddressingMode::Absolute:
			uint16_t lo = Fetch();
			uint16_t hi = Fetch();
			return (hi << 8) | lo;
		
		case CPU::AddressingMode::AbsoluteX:
			return 0;

		case CPU::AddressingMode::AbsoluteY:
			return 0;

		case CPU::AddressingMode::IndirectX:
			return 0;

		case CPU::AddressingMode::IndirectY:
			return 0;

		case CPU::AddressingMode::NoneAddressing:
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
		if (cycles == 0)
		{
			uint8_t const opcode{ Fetch() };

			// Get correct code from table
							
			// Update cycles based on instruction from the table

			// Call the address mode function

			// Call the opcode function
		}


		
	}
}
