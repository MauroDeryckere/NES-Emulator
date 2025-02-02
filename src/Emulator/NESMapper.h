#ifndef NES_EMULATOR_MAPPER
#define NES_EMULATOR_MAPPER

#include "emulator_pch.h"

// Sources
// https://www.nesdev.org/wiki/Mapper
// https://www.youtube.com/watch?v=xdzOvpYPmGE

namespace NesEm
{
	// Mapper base class, all implemented mappers will inherit from this
	class Mapper
	{
	public:
		Mapper(uint8_t chrBanks, uint8_t prgBanks):
		m_CHRBanks{ chrBanks },
		m_PRGBanks{ prgBanks } { }

		virtual ~Mapper() = default; 

		// Param (out) uint16_t remapped address
		// Return bool is the address in the PRG bank? 
		[[nodiscard]] virtual bool MapAddress(uint16_t& address) const noexcept = 0;

		Mapper(Mapper const&) = delete;
		Mapper(Mapper&&) = delete;
		Mapper& operator=(Mapper const&) = delete;
		Mapper& operator=(Mapper&&) = delete;

	protected:
		// Data all mappers need

		// Number of CHR banks
		uint8_t m_CHRBanks{ };
		// Number of PRG banks
		uint8_t m_PRGBanks{ };

	private:
	};
}

#endif