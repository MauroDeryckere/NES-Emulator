// disable SDL warning
#pragma warning push 4820
#include <SDL3/SDL.h>
#pragma warning pop

#include "ServiceLocator.h"
#include "SDLRenderer.h"

int main()
{
	using namespace NesEm;

	Window gameWindow{ "NES Emulator", 800, 600, SDL_WINDOW_RESIZABLE};
	ServiceLocator::RegisterRenderer(std::make_unique<SDLRenderer>(gameWindow));

	Renderer& renderer{ ServiceLocator::GetRenderer() };

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

		//Update

		//Cpu emulation, ...

		//Render
		renderer.Render();
	}
		

    return 0;
}