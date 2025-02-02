#include "Emulator.h"

namespace NesEm
{
	Emulator::Emulator():
	m_PPU{ },
	m_CPU{ m_PPU },
	m_Cartridge{ "Resources/test.nes" }
	{

	}
	void Emulator::Run() noexcept
	{
		// Run the PPU
		m_PPU.Clock();

		// This depends on the mode our NES runs in (PAL vs NTSC)
		if (m_MasterClock % 4 == 0)
		{
			// Run the CPU
			m_CPU.Clock();
		}

		++m_MasterClock;
	}

	void Emulator::Reset() noexcept
	{
		m_CPU.Reset();

		m_MasterClock = 0;
	}
}