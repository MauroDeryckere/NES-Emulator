#include "ServiceLocator.h"
#include "SDLRenderer.h"

#include "Timer.h"

#include <thread>

int main()
{
	using namespace NesEm;

	Window gameWindow{ 
		"NES Emulator",
		200,
		200,
		SDL_WINDOW_RESIZABLE
		| SDL_WINDOW_HIGH_PIXEL_DENSITY
	};
	ServiceLocator::RegisterRenderer(std::make_unique<SDLRenderer>(gameWindow));

	Renderer& renderer{ ServiceLocator::GetRenderer() };
	auto& time = GameTime::GetInstance();
	time.SetFPS(60.f);


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
			default: break;
			}
		}

		while(time.IsLag())
		{
			//Fixed Update if necessary
			time.ProcessLag();
		}
		//Update
		//Cpu emulation, ...

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
				fpsTimer -= 1.f;
			}
		}

		//Cap FPS

		SDL_Delay(time.SleepTime());
		//std::this_thread::sleep_for(std::chrono::milliseconds(time.SleepTime()));
	}

    return 0;
}