#include "ServiceLocator.h"
#include "SDLRenderer.h"
#include "InputManager.h"

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
	auto& input = InputManager::GetInstance();

	// Setup the game time
	time.SetFPS(50.f);

	// Setup the inputmanager
	input.AddAction({"Fullscreen", 67, InputManager::InputAction::EventType::KeyDownThisFrame });


	// Initialize the NES emulator
	Emulator emulator{ };


	// toggle displaying fps in console window
	constexpr bool displayFPS{ false };
	float fpsTimer{ 0.f };
	auto fpsCount{ 0 };


	bool isRunning{ true };
	while (isRunning)
	{
		time.Update();

		isRunning = input.ProcessInput();
		if (input.IsActionExecuted("Fullscreen"))
		{
			renderer.ToggleFullScreen();
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