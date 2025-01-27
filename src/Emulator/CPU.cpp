#include "CPU.h"

namespace NesEm
{
	//Cycles by ref
	//Address return val

	//inline uint8_t CPU::CalculateAddressModeCycles(AddressingMode mode) noexcept
	//{
	//	switch (mode)
	//	{
	//	case AddressingMode::Accumulator:

	//	case AddressingMode::Relative:

	//	case AddressingMode::Immediate:

	//	case AddressingMode::ZeroPage:

	//	case AddressingMode::ZeroPageX:

	//	case AddressingMode::ZeroPageY:
	//	
	//	case AddressingMode::Absolute:
	//	{
	//		uint8_t const lo{ m_Memory.Read(m_ProgramCounter++) };
	//		uint8_t const hi{ m_Memory.Read(m_ProgramCounter++) };
	//		// address = (hi << 8) | lo;
	//		return 0;
	//	}
	//	
	//	case AddressingMode::AbsoluteX:

	//	case AddressingMode::AbsoluteY:

	//	case AddressingMode::Indirect:

	//	case AddressingMode::IndirectX:

	//	case AddressingMode::IndirectY:

	//	default: break;
	//	}

	//}
	inline uint8_t CPU::Fetch() noexcept
	{
		//TODO
		return 0;
	}

	void CPU::Clock() noexcept
	{
		if (m_CurrCycles == 0)
		{
			// Get correct code from table & increase program counter
			uint8_t const opcode{ m_Memory.Read(m_ProgramCounter++) };

			// Update cycles based on instruction from the table
			//m_CurrCycles = OPCODES_6502[opcode].cycles;

			// Call the address mode function
			//uint8_t addedCycles{ CalculateAddressModeCycles(OPCODES_6502[opcode].mode) };

			// Call the opcode function
			//if (OPCODES_6502_FUNCTIONS[OPCODES_6502[opcode].id]())
			//{
			//	m_CurrCycles += addedCycles;
			//}
		}

		--m_CurrCycles;
	}
}
