#ifndef NES_EMULATOR_CARTRIDGE
#define NES_EMULATOR_CARTRIDGE

#include "NESMemory.h"

#include <cassert>
#include <vector>
#include <filesystem>

#include "SDL3/SDL.h"

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
		Cartridge(std::filesystem::path const& filePath)
		{
			// Whenever a cartridge is created, we have to read the file and create all data accordingly.
			const char* basePath{ SDL_GetBasePath() };
			assert(basePath);

			std::filesystem::path const fullPath{ basePath / filePath };
			assert(exists(fullPath));
			if (!exists(fullPath))
			{
				throw std::runtime_error("Can not load cartridge, file does not exist: " + fullPath.string());
			}

			SDL_Log("%s", "Loading cartridge");

			// https://www.nesdev.org/wiki/INES
			// An iNES file consists of the following sections, in order :
			// Header (16 bytes)
			// Trainer, if present (0 or 512 bytes)
			// PRG ROM data (16384 * x bytes)
			// CHR ROM data, if present (8192 * y bytes)
			// PlayChoice INST-ROM, if present (0 or 8192 bytes)
			// PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut) (this is often missing; see PC10 ROM-Images for details)

			// Some ROM-Images additionally contain a 128-byte (or sometimes 127-byte) title at the end of the file.


			// Header format: 
			// Bytes	Description
			// 0-3		Constant $4E $45 $53 $1A (ASCII "NES" followed by MS-DOS end-of-file)
			// 4		Size of PRG ROM in 16 KB units
			// 5		Size of CHR ROM in 8 KB units (value 0 means the board uses CHR RAM)
			// 6		Flags 6 – Mapper, mirroring, battery, trainer
			// 7		Flags 7 – Mapper, VS/Playchoice, NES 2.0
			// 8		Flags 8 – PRG-RAM size (rarely used extension)
			// 9		Flags 9 – TV system (rarely used extension)
			// 10		Flags 10 – TV system, PRG-RAM presence (unofficial, rarely used extension)
			// 11-15	Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)
			
			// More on each flag as we deal with reading / saving these

			// Read the file - TODO
		}

		~Cartridge() = default;


		Cartridge(Cartridge const&) = delete;
		Cartridge(Cartridge&&) = delete;
		Cartridge& operator=(Cartridge const&) = delete;
		Cartridge& operator=(Cartridge&&) = delete;

	private:
		// ROM - PRG
		std::vector<uint8_t> m_PRG{};
		// ROM - CHR
		std::vector<uint8_t> m_CHR{};

		// Which mapper are we using
		uint8_t m_MapperID{ 0 };
		// How manybanks are there for each of the memory types
		uint8_t m_CHRBanks{ 0 };
		uint8_t m_PRGBanks{ 0 };

		bool m_IsMirrorVertical{ false };
	};
}

#endif