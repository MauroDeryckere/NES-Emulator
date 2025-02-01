#include "Emulator.h"

namespace NesEm
{
	Emulator::Emulator():
	m_PPU{ },
	m_CPU{ m_PPU },
	m_Cartridge{ "Resources/test.rom" }
	{

	}
	void Emulator::Run() noexcept
	{
		// Run the CPU
		m_CPU.Clock();
		//m_PPU.Clock();
	}

	void Emulator::Reset() noexcept
	{
		m_CPU.Reset();

		m_SystemClock = 0;
	}
}