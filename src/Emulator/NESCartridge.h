#ifndef NES_EMULATOR_CARTRIDGE
#define NES_EMULATOR_CARTRIDGE

#include "PCH.h"

#include "NESMemory.h"

#include <exception>
#include <vector>
#include <filesystem>
#include <fstream>

#include "SDL3/SDL.h"

#include "NROMMapper.h"

/* Various sources used during development of the Cartridge of our emulator:
 * https://www.youtube.com/watch?v=xdzOvpYPmGE&list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf&index=4
 * https://www.nesdev.org/wiki/INES
 */


namespace NesEm
{
	// Cartridge contains program memory (PRG) and pattern memory (CHR)
	// The CPU and PPU communicate with this
	// Mappers may change behaviour here a lot, but currently we are only trying to support the NROM mapper for our games.
	class Cartridge final
	{
	public:
		Cartridge(std::filesystem::path const& filePath);

		~Cartridge() = default;

		// Read from cartridge
		[[nodiscard]] uint8_t Read(uint16_t address) const noexcept
		{
			// use the mapper to "redirect" our address and read the value

			uint16_t mappedAddr{ address };
			bool const isPRG{ m_pMapper->MapAddress(mappedAddr) };

			if (isPRG)
			{
				assert(mappedAddr <= m_PRG.size());
				return m_PRG[mappedAddr];
			}

			assert(mappedAddr <= m_CHR.size());
			return m_CHR[mappedAddr];
		}

		// Write to cartridge
		void Write(uint16_t address, uint8_t value) noexcept
		{
			// use the mapper to "redirect" our address and read the value

			uint16_t mappedAddr{ address };
			bool const isPRG{ m_pMapper->MapAddress(mappedAddr) };

			if (isPRG)
			{
				assert(mappedAddr <= m_PRG.size());
				m_PRG[mappedAddr] = value;
			}

			assert(mappedAddr <= m_CHR.size());
			m_CHR[mappedAddr] = value;
		}

		Cartridge(Cartridge const&) = delete;
		Cartridge(Cartridge&&) = delete;
		Cartridge& operator=(Cartridge const&) = delete;
		Cartridge& operator=(Cartridge&&) = delete;

	private:
		// ROM - PRG
		std::vector<uint8_t> m_PRG{};

		// ROM - CHR
		std::vector<uint8_t> m_CHR{};

		// How manybanks are there for each of the memory types
		uint8_t m_CHRBanks{ 0 };
		uint8_t m_PRGBanks{ 0 };

		uint8_t m_Flags6{ 0 };
		uint8_t m_Flags7{ 0 };

		// Which mapper are we using
		uint8_t m_MapperID{ 0 };

		std::shared_ptr<Mapper> m_pMapper;
	};
}

#endif