#include "NESCPU.h"

#include <iostream>
namespace NesEm
{
	CPU::CPU(PPU& ppu, Cartridge& cart):
	m_PPU{ ppu },
	m_Cartridge{ cart },

	m_Memory{  },

	m_Accumulator{ 0 },
	m_XRegister{ 0 },
	m_YRegister{ 0 },

	m_StackPointer { STACK_PTR_INIT },

	m_StatusRegister{ 0 }
	{
		// https://www.nesdev.org/wiki/CPU_power_up_state

		// At power state of the CPU
		// Data at location 0x0FFC can be set by programmer, this is the "entry point" for the program
		// Program counter should be set to this data whenever we Reset
		// LL | HH
		m_ProgramCounter = Read(RESET_VECTOR) | (Read(RESET_VECTOR + 1) << 8);

		SDL_Log("%s", "Current program counter: ");
		SDL_Log("%d", int(m_ProgramCounter));

		SetFlag(StatusFlags::I);
		SetFlag(StatusFlags::U);
	}

	void CPU::Clock() noexcept
	{
		//Wait until clock is available again to execute the next instruction
		if (m_CurrCycles == 0)
		{
			// Get correct code from table & increase program counter
			uint8_t const opcodeID{ Read() };

			SDL_Log("%d", int(opcodeID));

			// Update cycles based on instruction from the table
			m_CurrCycles = m_OpcodeHandler.ExecuteOpcode(opcodeID, (*this));
		}

		--m_CurrCycles;
	}
}
