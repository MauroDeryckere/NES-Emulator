#include "Emulator.h"
#include "EmulatorSettings.h"

namespace NesEm
{
	Emulator::Emulator():
	// We should load the cartridge first (or make a function in the CPU, ... where initialize / load the cartridge).
	m_Cartridge{ "Resources/test.nes" },
	m_PPU{ },
	m_CPU{ m_PPU, m_Cartridge }
	{

	}
	void Emulator::Run() noexcept
	{
		//TODO how many clocks per frame
		//constexpr int SPEED{ 50 };
		//while (m_MasterClock < SPEED)
		{
			// Run the PPU
			m_PPU.Clock();

			switch (Config::MODE)
			{
			case Config::NES_MODE::PAL:
			{
				// 5 master clocks pet PPU dot
				if (m_MasterClock % 4 == 0)
				{
					// Run the CPU
					m_CPU.Clock();
				}
			}break;

			case Config::NES_MODE::NTSC:
			{
				// 4 master clocks pet PPU dot
				if (m_MasterClock % 3 == 0)
				{
					// Run the CPU
					m_CPU.Clock();
				}

			}break;

			default: break;
			}

			++m_MasterClock;
		}

		//m_MasterClock = 0;
	}

	void Emulator::Reset() noexcept
	{
		m_CPU.Reset();

		m_MasterClock = 0;
	}
}