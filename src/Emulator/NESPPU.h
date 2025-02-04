#ifndef NES_EMULATOR_PPU
#define NES_EMULATOR_PPU

#include "emulator_pch.h"

#include "NESMemory.h"
#include "EmulatorSettings.h"

/* Various sources used during development of the PPU of our emulator:
 * https://www.youtube.com/watch?v=xdzOvpYPmGE&list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf&index=4
 * https://www.nesdev.org/wiki/PPU
 * https://www.nesdev.org/wiki/Cycle_reference_chart
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
			++m_CurrCycle;

			switch (Config::MODE)
			{

			case Config::NES_MODE::PAL:
			{
				// PAL total number of dots per frame:
				// 341 x 312
			}break;

			case Config::NES_MODE::NTSC:
			{
				// NTSC total number of dots per frame:
				// 341 x 261  + 340.5 (pre render line is one dot shorter in every odd frame)
				if (m_CurrCycle >= 341)
				{
					m_CurrCycle = 0;
					++m_CurrScanline;
					if (m_CurrScanline >= 261)
					{
						m_CurrScanline = -1;
						m_FrameComplete = true;
					}
				}

			}break;

			default: break;

			}
		}

		PPU(PPU const&) = delete;
		PPU(PPU&&) = delete;
		PPU& operator=(PPU const&) = delete;
		PPU& operator=(PPU&&) = delete;

	private:
		uint16_t m_CurrCycle{};
		uint16_t m_CurrScanline{};

		bool m_FrameComplete{ false };

		NESMemory<1024> m_Nametable_1{ };
		NESMemory<1024> m_Nametable_2{ };

		NESMemory<32> m_Pallete{ };
	};
}

#endif