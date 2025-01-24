#include "SDLRenderer.h"

namespace NesEm
{
	SDLRenderer::SDLRenderer(Window const& w) :
		m_Window{w}
	{
		//Clamp aspect ratio to specific size range
		//SDL_SetWindowAspectRatio(m_Window.pWindow, 1920 / 1080.f, 1920 / 1080.f);

		SDL_Log("%s %d", "Running SDL Version: ", SDL_GetVersion());

		if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
			SDL_Quit();
		}
		SDL_Log("%s", "SDL Window initialized");

		m_pRenderer = SDL_CreateRenderer(m_Window.pWindow, NULL);
		SDL_assert(m_pRenderer);
		if (!m_pRenderer)
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
			SDL_Quit();
		}
		SDL_Log("%s", "SDL Renderer initialized");
	}

	SDLRenderer::~SDLRenderer()
	{
		//Destroy the renderer
		if (m_pRenderer)
		{
			SDL_DestroyRenderer(m_pRenderer);
			SDL_Log("%s", "SDL Renderer destroyed");
		}

		m_Window.ReleaseWindow();

		//Quit specific subsystems and all of SDL
		SDL_QuitSubSystem(SDL_INIT_VIDEO);

		SDL_Log("%s", "SDL quit successfully");
		SDL_Quit();
	}

	void SDLRenderer::Render() const
	{
		//Scale based on user monitor but keep logic at same "canvas size" for game
		//SDL_SetRenderLogicalPresentation(m_pRenderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 100, 255);
		SDL_RenderClear(m_pRenderer);

		SDL_RenderPresent(m_pRenderer);
	}
}
