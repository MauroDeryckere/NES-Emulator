// disable SDL warning
#pragma warning push 4820
#include <SDL3/SDL.h>
#pragma warning pop

#include "Window.h"

int main()
{
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
		SDL_Quit();
		return -1;
	}

	{
		//Width and height is not in pixels but depends on the platform/monitor
		uint32_t constexpr windowWidth{ 800 };
		uint32_t constexpr windowHeight{ 600 };
		Window window{ "SDL Window", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE };
			

		bool isRunning{ true };

		while (isRunning)
		{
			//Handle Input & other events
			SDL_Event event{ };
			while(SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_EVENT_QUIT:
					isRunning = false;
					break;
				default: break;
				}
			}
		}

		//Update

		//Render


	}
	//Quit specific subsystems or all of SDL
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();

    return 0;
}