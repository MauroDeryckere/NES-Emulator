#include "ServiceLocator.h"
#include "SDLRenderer.h"

#include "Timer.h"

#include "Emulator.h"

#include <thread>

#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
	using namespace NesEm;

	Window gameWindow{ 
		"NES Emulator",
		1920,
		1080,
		SDL_WINDOW_RESIZABLE
		| SDL_WINDOW_HIGH_PIXEL_DENSITY
		| SDL_WINDOW_MAXIMIZED
		| SDL_WINDOW_INPUT_FOCUS
		| SDL_WINDOW_MOUSE_FOCUS
		| SDL_WINDOW_MOUSE_CAPTURE
		| SDL_WINDOW_KEYBOARD_GRABBED,
		16.f / 9.f,
		16.f / 9.f
	};
	ServiceLocator::RegisterRenderer(std::make_unique<SDLRenderer>(gameWindow));

	Renderer& renderer{ ServiceLocator::GetRenderer() };
	auto& time = GameTime::GetInstance();
	time.SetFPS(50.f);

	// Initialize the NES emulator
	Emulator emulator{ };

	// toggle displaying fps in console window
	constexpr bool displayFPS{ true };
	float fpsTimer{ 0.f };
	auto fpsCount{ 0 };

	bool isRunning{ true };


	while (isRunning)
	{
		time.Update();

		//Handle Input & other events
		SDL_Event event{ };
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				isRunning = false;
				break;
			case SDL_EVENT_KEY_DOWN:
				if (event.key.scancode == SDL_SCANCODE_F10)
				{
					renderer.ToggleFullScreen();
				}
				break;
			default: break;
			}
		}

		while(time.IsLag())
		{
			//Fixed Update if necessary
			time.ProcessLag();
		}

		//Update
		emulator.Run();

		//Render
		renderer.Render();

		//TODO
		/*#ifdef USE_STEAMWORKS
				SteamAPI_RunCallbacks();
		#endif*/

		fpsTimer += time.ElapsedSec();
		++fpsCount;
		if (displayFPS)
		{
			if (fpsTimer >= 1.0f)
			{
				SDL_Log("FPS: %.1f", static_cast<float>(fpsCount) / fpsTimer);
				fpsCount = 0;
				fpsTimer = 0.f;
			}
		}

		//Cap FPS
		SDL_Delay(time.SleepTime());
	}

    return 0;
}