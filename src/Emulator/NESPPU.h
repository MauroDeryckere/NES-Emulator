#ifndef NES_EMULATOR_PPU
#define NES_EMULATOR_PPU

#include "NESMemory.h"

namespace NesEm
{
	class PPU final
	{
	public:
		PPU() = default;
		~PPU() = default;

		void Clock() noexcept
		{
			
		}

		PPU(PPU const&) = delete;
		PPU(PPU&&) = delete;
		PPU& operator=(PPU const&) = delete;
		PPU& operator=(PPU&&) = delete;

	private:
		NESMemory<1024> m_Nametable_1{ };
		NESMemory<1024> m_Nametable_2{ };
		NESMemory<32> m_Pallete{ };
	};
}

#endif