#ifndef NES_EMULATOR_PPU
#define NES_EMULATOR_PPU

#include "emulator_pch.h"

#include "NESMemory.h"
#include "EmulatorSettings.h"

/* Various sources used during development of the PPU of our emulator:
 * https://www.youtube.com/watch?v=xdzOvpYPmGE&list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf&index=4
 * https://www.nesdev.org/wiki/PPU
 * https://www.nesdev.org/wiki/Cycle_reference_chart
 * https://www.nesdev.org/wiki/PPU_registers
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

		void Render() const noexcept;

		void Write(uint16_t address, uint8_t value) noexcept
		{

		}
		

		[[nodiscard]] uint8_t Read(uint16_t address) const noexcept
		{
			return 0;
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

#pragma region PPU_Registers
		// https://www.nesdev.org/wiki/PPU_registers

		// PPU control register, located at address $2000 (write) --> Miscellaneous settings
		// 7  bit  0
		// VPHB SINN
		// https://www.nesdev.org/wiki/PPU_registers#PPUCTRL
		struct PPU_CTRL final
		{
			uint8_t nametableSelectX	: 1; // Base nametable address X (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
			uint8_t nametableSelectY	: 1; // Base nametable address Y (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
			uint8_t incrementMode		: 1; // VRAM address increment per CPU read/write of PPUDATA (0: add 1, going across; 1: add 32, going down)
			uint8_t spriteTileSelect	: 1; // Sprite pattern table address for 8x8 sprites (0: $0000; 1: $1000; ignored in 8x16 mode)
			uint8_t backgroundTileSelect: 1; // Background pattern table address (0: $0000; 1: $1000)
			uint8_t spriteHeight		: 1; // Sprite size (0: 8x8 pixels; 1: 8x16 pixels)
			uint8_t ppuMasterSlave		: 1; // PPU master/slave select (0: read backdrop from EXT pins; 1: output color on EXT pins)
			uint8_t nmiEnable			: 1; // 0: read backdrop from EXT pins; 1: output color on EXT pins)
		};
		PPU_CTRL m_PPUCtrl{  };

		// PPU mask register, located at address $2001 (write) --> Render settings
		// 7  bit  0
		// BGRs bMmG
		// https://www.nesdev.org/wiki/PPU_registers#PPUMASK
		struct PPU_MASK final
		{
			uint8_t greyScale				: 1; // Greyscale (0: normal color, 1: greyscale)
			uint8_t backgroundLeftColEnable : 1; // 1: Show background in leftmost 8 pixels of screen, 0: Hide
			uint8_t spriteLeftColEnable		: 1; // 1: Show sprites in leftmost 8 pixels of screen, 0: Hide
			uint8_t backgroundEnable		: 1; // 1: Enable background rendering
			uint8_t spriteEnable			: 1; // 1: Enable sprite rendering
			uint8_t emphasizeRed			: 1; // Emphasize red (green on PAL/Dendy)
			uint8_t emphasizeGreen			: 1; // Emphasize green (red on PAL/Dendy)
			uint8_t emphasizeBlue			: 1; // Emphasize blue
		};
		PPU_MASK m_PPUMask{  };
		
		// PPU status register, located at address $2002 (read) -- Rendering events
		// 7  bit  0
		// VSOx xxxx
		// https://www.nesdev.org/wiki/PPU_registers#PPUSTATUS
		struct PPU_STATUS final
		{
			uint8_t unused				: 5; // (PPU open bus or 2C05 PPU identifier)
			uint8_t spriteOverflowFlag	: 1; // Sprite overflow flag
			uint8_t sprite0HitFlag		: 1; // Sprite 0 hit flag
			uint8_t vblankFlag			: 1; // Vblank flag, cleared on read.
		};
		PPU_STATUS m_PPUStatus{  };

		// PPU scroll regiter, located at address $2005
		uint8_t m_PPUScroll{  };




#pragma endregion
	};
}

#endif