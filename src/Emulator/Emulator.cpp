#include "Emulator.h"
#include "EmulatorSettings.h"

namespace NesEm
{
	Emulator::Emulator():
	m_PPU{ },
	m_CPU{ m_PPU, m_Cartridge },
	m_Cartridge{ "Resources/test.nes" }
	{

	}
	void Emulator::Run() noexcept
	{
		// Run the PPU
		m_PPU.Clock();

		switch (Config::MODE)
		{
		case Config::NES_MODE::PAL:
		{
			// 5 master clocks pet PPU dot
			if (m_MasterClock % 5 == 0)
			{
				// Run the CPU
				m_CPU.Clock();
			}
		}break;

		case Config::NES_MODE::NTSC:
		{
			// 4 master clocks pet PPU dot
			if (m_MasterClock % 4 == 0)
			{
				// Run the CPU
				m_CPU.Clock();
			}

		}break;

		default: break;
		}

		++m_MasterClock;
	}

	void Emulator::Reset() noexcept
	{
		m_CPU.Reset();

		m_MasterClock = 0;
	}
}