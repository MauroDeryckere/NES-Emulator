#ifndef NES_EMULATOR_PPU
#define NES_EMULATOR_PPU

#include "emulator_pch.h"

#include "NESMemory.h"
#include "EmulatorSettings.h"

/* Various sources used during development of the PPU of our emulator:
 * https://www.youtube.com/watch?v=xdzOvpYPmGE&list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf&index=4
 * https://www.nesdev.org/wiki/PPU
 * https://www.nesdev.org/wiki/PPU_registers
 * https://www.nesdev.org/wiki/Cycle_reference_chart
 * https://www.nesdev.org/wiki/PPU_frame_timing
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

		void Clock() noexcept;
		void Render() const noexcept;

		// Param uint16_t the address we're writing to
		// Param uint8_t the data we are writing to the address
		void Write(uint16_t address, uint8_t value) noexcept
		{
			// Handle mirroring
			address &= 7;
			assert(address <= 7 && "address out of bounds");

			switch (address)
			{

			case (PPU_CTRL_ADDRESS & 7):
			{
				m_PPUCtrl = value;
				m_PPUCtrl.raw = value;
			}break;

			case (PPU_MASK_ADDRESS & 7):
			{

			}break;

			case (PPU_STATUS_ADDRESS & 7):
			{

			}break;

			case (OAM_ADDR_ADDRESS & 7):
			{

			}break;

			case (OAM_DATA_ADDRESS & 7):
			{

			}break;

			case (PPU_SCROLL_ADDRESS & 7):
			{

			}break;

			case (PPU_ADDR_ADDRESS & 7):
			{

			}break;

			case (PPU_DATA_ADDRESS & 7):
			{

			}break;

			default: break;

			}
		}
		
		// Param uint16_t the address we're reading from
		// Return uint8_t the data read from the address
		[[nodiscard]] uint8_t Read(uint16_t address) const noexcept
		{
			// Handle mirroring
			address &= 7;
			assert(address <= 7 && "address out of bounds");

			switch (address)
			{

			case (PPU_CTRL_ADDRESS & 7):
			{

			}break;

			case (PPU_MASK_ADDRESS & 7):
			{

			}break;

			case (PPU_STATUS_ADDRESS & 7):
			{

			}break;

			case (OAM_ADDR_ADDRESS & 7):
			{

			}break;

			case (OAM_DATA_ADDRESS & 7):
			{

			}break;

			case (PPU_SCROLL_ADDRESS & 7):
			{

			}break;

			case (PPU_ADDR_ADDRESS & 7):
			{

			}break;

			case (PPU_DATA_ADDRESS & 7):
			{

			}break;

			default: break;

			}

			return 0;
		}

		PPU(PPU const&) = delete;
		PPU(PPU&&) = delete;
		PPU& operator=(PPU const&) = delete;
		PPU& operator=(PPU&&) = delete;

	private:
		uint16_t m_CurrCycle{ };
		uint16_t m_CurrScanline{ };

		//TODO
		bool m_FrameComplete{ false };
		
		NESMemory<1024> m_Nametable_1{ };
		NESMemory<1024> m_Nametable_2{ };

		NESMemory<32> m_Pallete{ };

#pragma region PPU_Registers
		// https://www.nesdev.org/wiki/PPU_registers
	#pragma region Register_addresses
		static constexpr uint16_t PPU_CTRL_ADDRESS{ 0x2000 };
		static constexpr uint16_t PPU_MASK_ADDRESS{ 0x2001 };
		static constexpr uint16_t PPU_STATUS_ADDRESS{ 0x2002 };
		static constexpr uint16_t OAM_ADDR_ADDRESS{ 0x2003 };
		static constexpr uint16_t OAM_DATA_ADDRESS{ 0x2004 };
		static constexpr uint16_t PPU_SCROLL_ADDRESS{ 0x2005 };
		static constexpr uint16_t PPU_ADDR_ADDRESS{ 0x2006 };
		static constexpr uint16_t PPU_DATA_ADDRESS{ 0x2007 };

		static constexpr uint16_t OAM_DMA_ADDRESS{ 0x4014 };
	#pragma endregion

		// PPU control register, located at address $2000 (write) --> Miscellaneous settings
		union PPU_CTRL_REG
		{
			// PPU control bitfield
			// 7  bit  0
			// VPHB SINN
			// https://www.nesdev.org/wiki/PPU_registers#PPUCTRL
			struct PPU_CTRL final
			{
				uint8_t nametableSelectX	 : 1; // Base nametable address X (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
				uint8_t nametableSelectY	 : 1; // Base nametable address Y (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
				uint8_t incrementMode		 : 1; // VRAM address increment per CPU read/write of PPUDATA (0: add 1, going across; 1: add 32, going down)
				uint8_t spriteTileSelect	 : 1; // Sprite pattern table address for 8x8 sprites (0: $0000; 1: $1000; ignored in 8x16 mode)
				uint8_t backgroundTileSelect : 1; // Background pattern table address (0: $0000; 1: $1000)
				uint8_t spriteHeight		 : 1; // Sprite size (0: 8x8 pixels; 1: 8x16 pixels)
				uint8_t ppuMasterSlave		 : 1; // PPU master/slave select (0: read backdrop from EXT pins; 1: output color on EXT pins)
				uint8_t nmiEnable			 : 1; // 0: read backdrop from EXT pins; 1: output color on EXT pins)
			};
			static_assert(sizeof(PPU_CTRL) == 1, "PPU_CTRL must be exactly 1 byte!");

			PPU_CTRL bits{ };
			uint8_t raw{ };

			// Allows assigning the full register directly
			PPU_CTRL_REG& operator=(uint8_t value)
			{
				raw = value;
				return *this;
			}
		};
		PPU_CTRL_REG m_PPUCtrl{  };

		// PPU mask register, located at address $2001 (write) --> Render settings
		union PPU_MASK_REG
		{
			// PPU mask bitfield
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
			static_assert(sizeof(PPU_MASK) == 1, "PPU_MASK must be exactly 1 byte!");

			PPU_MASK bits{ };
			uint8_t raw{ };

			// Allows assigning the full register directly
			PPU_MASK_REG& operator=(uint8_t value)
			{
				raw = value;
				return *this;
			}
		};
		PPU_MASK_REG m_PPUMask{  };

		// PPU status register, located at address $2002 (read) -- Rendering events
		union PPU_STATUS_REG
		{
			// PPU status bitfield
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
			static_assert(sizeof(PPU_STATUS) == 1, "PPU_STATUS must be exactly 1 byte!");

			PPU_STATUS bits{ };
			uint8_t raw{ };

			// Allows assigning the full register directly
			PPU_STATUS_REG& operator=(uint8_t value)
			{
				raw = value;
				return *this;
			}
		};
		PPU_STATUS_REG m_PPUStatus{};


		// PPU scroll regiter, located at address $2005 (write)
		//TODO
		uint8_t m_PPUScroll{  };
#pragma endregion


	};
}

#endif