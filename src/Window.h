#ifndef NES_EMULATOR_WINDOW
#define NES_EMULATOR_WINDOW

#include <SDL3/SDL.h>

#include <memory>
#include <string>

struct Window
{
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> pWindow{ nullptr, SDL_DestroyWindow };

	explicit Window(std::string const& title, uint32_t w, uint32_t h, SDL_WindowFlags flags = {}):
		pWindow{ SDL_CreateWindow(title.c_str(), static_cast<int>(w), static_cast<int>(h), flags), SDL_DestroyWindow }
	{
		SDL_assert(pWindow);
		if (!pWindow)
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
			SDL_Quit();
		}
	}
};

#endif