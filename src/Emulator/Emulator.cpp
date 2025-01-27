#include "Emulator.h"

namespace NesEm
{
	void Emulator::Run() noexcept
	{
		// Run the CPU
		m_Cpu.Clock();
	}
}