#ifndef NES_EMULATOR_WINDOW
#define NES_EMULATOR_WINDOW

#include <cstdint>
#include <string>

namespace NesEm
{
	struct Window final
	{
		//Title of the window
		std::string title{};

		//Width and height is not in pixels but depends on the platform/monitor
		uint32_t width{};
		uint32_t height{};

		//Flags for the window
		uint64_t flags{};

		float minAspectRatio{-1.f};
		float maxAspectRatio{-1.f};
	};
}

#endif