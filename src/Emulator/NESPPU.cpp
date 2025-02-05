#include "NESPPU.h"

namespace NesEm
{
	void PPU::Clock() noexcept
	{
		// https://www.nesdev.org/wiki/PPU_frame_timing
		//TODO

		++m_CurrCycle;

		switch (Config::MODE)
		{

		case Config::NES_MODE::PAL:
		{
			// PAL total number of dots per frame:
			// 341 x 312
			if (m_CurrCycle >= 341)
			{
				m_CurrCycle = 0;
				++m_CurrScanline;
				if (m_CurrScanline >= 312)
				{
					m_CurrScanline = -1;
					m_FrameComplete = true;
				}
			}
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

	void PPU::Render() const noexcept
	{
		//TODO
	}
}
