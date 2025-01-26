#ifndef NES_EMULATOR_SDLWINDOW
#define NES_EMULATOR_SDLWINDOW

#pragma warning (push)
#pragma warning (disable: 4820)
#pragma warning (disable: 4514)
#pragma warning (disable: 4548)
	#include <SDL3/SDL.h>
#pragma warning (pop)

#include <string>
#include "Window.h"

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

		explicit SDLWindow(Window const& window)
		{
			pWindow = SDL_CreateWindow(window.title.c_str(), static_cast<int>(window.width), static_cast<int>(window.height), window.flags);
			SDL_assert(pWindow);
			if (!pWindow)
			{
				SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
				SDL_Quit();
			}
			else
			{
				SDL_Log("%s", "SDL Window Initialized");

				SDL_SetWindowAspectRatio(pWindow,
					window.minAspectRatio < 0.f ? 0.f : window.minAspectRatio,
					window.maxAspectRatio < 0.f ? 0.f : window.maxAspectRatio);
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