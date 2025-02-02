#ifndef NES_EMULATOR_SETTINGS
#define NES_EMULATOR_SETTINGS

#include <cstdint>

namespace NesEm
{
	namespace Config
	{
		enum class NES_MODE : uint8_t
		{
			PAL = 0,
			NTSC = 1
		};

		constexpr NES_MODE MODE{ NES_MODE::PAL };
	}
}

#endif