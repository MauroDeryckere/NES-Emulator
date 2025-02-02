#ifndef NES_EMULATOR_NROM_MAPPER
#define NES_EMULATOR_NROM_MAPPER

#include "NESMapper.h"

// https://www.nesdev.org/wiki/NROM
// https://www.youtube.com/watch?v=xdzOvpYPmGE

namespace NesEm
{
	// NROM Mapper information: 
	// NROM basically only supports changing how many banks there are.
	// It either supports 1 PRG bank (mirrored) or 2 PRG banks.
	// and typically 1 CHR bank
	class NROMMapper final : public Mapper
	{
	public:
		NROMMapper(uint8_t chrBanks, uint8_t prgBanks) :
			Mapper{ chrBanks, prgBanks } { }

		virtual ~NROMMapper() override = default;

		[[nodiscard]] virtual bool MapAddress(uint16_t& address) const noexcept override
		{
			// https://www.nesdev.org/wiki/NROM
			// (CPU $6000 - $7FFF: Family basic only; PRG RAM, mirrored as necessary to fill entire 9 KiB window, write protectable with an extarnal switch)
			// CPU $8000 - $BFFF: First 16 KB of ROM
			// CPU $C000 - $FFFF: Last 16 KB of ROM (NROM-256) or mirror of $8000 - $BFFF (NROM-128)

			if (address >= 0x8000)
			{
				// PRG mapping
				if (m_PRGBanks == 1)
				{
					// Mirror 16KB PRG-ROM
					address -= 0x8000; // We want to clamp the value to the correct array IDX 
					address %= 0x4000; // Mirror
				}
				else
				{
					assert(m_PRGBanks == 2);

					// Direct 32KB PRG-ROM access
					address -= 0x8000; // We want to clamp the value to the correct array IDX
				}

				// Is PRG map return
				return true;
			}

			// No CHR mapping is required
			// No PRG map return

			return false;
		}

		// PPU remapping?


		NROMMapper(NROMMapper const&) = delete;
		NROMMapper(NROMMapper&&) = delete;
		NROMMapper& operator=(NROMMapper const&) = delete;
		NROMMapper& operator=(NROMMapper&&) = delete;

	private:
	};
}

#endif