#ifndef NES_EMULATOR_PPU
#define NES_EMULATOR_PPU

#include "NESMemory.h"

/* Various sources used during development of the Cartridge of our emulator:
 * https://www.youtube.com/watch?v=xdzOvpYPmGE&list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf&index=4
 * https://www.nesdev.org/wiki/PPU
 */

namespace NesEm
{
	// The PPU or Picture Processing Unit is basically a very early representation of a GPU
	// it has its own address space and handles anything related to background and sprite rendering
	class PPU final
	{
	public:
		PPU() = default;
		~PPU() = default;

		void Clock() noexcept
		{
			++m_CurrCyle;

			if (m_CurrCyle >= 341)
			{
				m_CurrCyle = 0;
				++m_CurrScanline;
				if (m_CurrScanline >= 261)
				{
					m_CurrScanline = -1;
					m_FrameComplete = true;
				}
			}
		}

		PPU(PPU const&) = delete;
		PPU(PPU&&) = delete;
		PPU& operator=(PPU const&) = delete;
		PPU& operator=(PPU&&) = delete;

	private:
		uint32_t m_CurrCyle{};
		uint16_t m_CurrScanline{};

		bool m_FrameComplete{ false };

		NESMemory<1024> m_Nametable_1{ };
		NESMemory<1024> m_Nametable_2{ };
		NESMemory<32> m_Pallete{ };
	};
}

#endif