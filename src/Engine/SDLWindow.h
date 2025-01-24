#ifndef NES_EMULATOR_SDLWINDOW
#define NES_EMULATOR_SDLWINDOW

#include <SDL3/SDL.h>
#include <string>

namespace NesEm
{
	struct SDLWindow final
	{
		SDL_Window* pWindow{ nullptr };

		void ReleaseWindow()
		{
			if (pWindow)
			{
				SDL_DestroyWindow(pWindow);
				pWindow = nullptr;
			}
		}

		explicit SDLWindow(std::string const& title, uint32_t w, uint32_t h, SDL_WindowFlags flags = {}) :
			pWindow{ SDL_CreateWindow(title.c_str(), static_cast<int>(w), static_cast<int>(h), flags) }
		{
			SDL_assert(pWindow);
			if (!pWindow)
			{
				SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
				SDL_Quit();
			}
		}
		~SDLWindow()
		{
			if (pWindow)
			{
				SDL_DestroyWindow(pWindow);
			}
		}
	};
}

#endif