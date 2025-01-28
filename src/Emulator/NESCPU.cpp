#include "NESCPU.h"

namespace NesEm
{
	void CPU::Clock() noexcept
	{
		//Wait until clock is available again to execute the next instruction
		if (m_CurrCycles == 0)
		{
			// Get correct code from table & increase program counter
			uint8_t const opcodeID{ Read() };

			// Update cycles based on instruction from the table
			m_CurrCycles = m_OpcodeHandler.ExecuteOpcode(opcodeID, (*this));
		}

		--m_CurrCycles;
	}
}
