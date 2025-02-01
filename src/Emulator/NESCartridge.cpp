#include "NESCartridge.h"

namespace  NesEm
{
	Cartridge::Cartridge(std::filesystem::path const& filePath)
	{
		// Note: more robust exception handling here is possible but currently we only deal with simple files

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

		if (std::ifstream input{ filePath, std::ios::binary | std::ios::in }; input.is_open())
		{
			// All header bytes
			std::array<uint8_t, 16> header{  };
			input.read(reinterpret_cast<char*>(header.data()), header.size());

			if (input.gcount() != 16)
			{
				throw std::runtime_error("Invalid NES file: Header too short");
			}

			// Confirm it is a NES file
			if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S' || header[3] != 0x1A)
			{
				throw std::runtime_error("Invalid NES file");
			}

			// Extract all header info and save it
			m_PRGBanks = header[4];
			m_CHRBanks = header[5];

			m_Flags6 = header[6];
			m_Flags7 = header[7];

			m_MapperID = (m_Flags6 >> 4) | (m_Flags7 & 0xF0);

			// If bit 2 of flags6 is set, a 512-byte trainer exists and should be skipped.
			if (m_Flags6 & 0x04)
			{
				input.seekg(512, std::ios::cur);
			}

			// Read the CHR and PRG data
			m_PRG.resize(m_PRGBanks * 16384);
			input.read(reinterpret_cast<char*>(m_PRG.data()), m_PRG.size());

			if (input.gcount() != m_PRG.size())
			{
				throw std::runtime_error("Invalid NES file: PRG ROM is incomplete");
			}

			m_CHR.resize(m_CHRBanks * 8192);
			if (m_CHR.size() > 0)
			{
				input.read(reinterpret_cast<char*>(m_CHR.data()), m_CHR.size());
				if (input.gcount() != m_CHR.size())
				{
					throw std::runtime_error("Invalid NES file: CHR ROM is incomplete");
				}
			}
		}
		else
		{
			throw std::runtime_error("Could not open file");
		}
	}
}
