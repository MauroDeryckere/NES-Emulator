#ifndef NES_EMULATOR_CARTRIDGE
#define NES_EMULATOR_CARTRIDGE

#include "NESMemory.h"

#include <cassert>
#include <vector>
#include <iostream>
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

			std::filesystem::path const fullPath{ std::filesystem::path(basePath) / filePath };
			assert(exists(fullPath));
			if (!exists(fullPath))
			{
				throw std::runtime_error("File does not exist: " + fullPath.string());
			}

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