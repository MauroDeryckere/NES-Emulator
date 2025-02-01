#ifndef NES_EMULATOR_NROM_MAPPER
#define NES_EMULATOR_NROM_MAPPER

#include "NESMapper.h"

// https://www.nesdev.org/wiki/NROM

namespace NesEm
{
	// NROM Mapper information: 
	// https://www.nesdev.org/wiki/NROM
	// NROM basically only supports changing how many banks there are.
	class NROMMapper final : public Mapper
	{
	public:
		NROMMapper() = default;
		virtual ~NROMMapper() override = default;

		NROMMapper(NROMMapper const&) = delete;
		NROMMapper(NROMMapper&&) = delete;
		NROMMapper& operator=(NROMMapper const&) = delete;
		NROMMapper& operator=(NROMMapper&&) = delete;

		// CPU may have bank switching
		// PPU has no bank switching

	private:
	}
}