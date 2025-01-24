#include "SDLRenderer.h"

namespace NesEm
{
	SDLRenderer::SDLRenderer(Window const& w) :
		m_Window{w.title, w.width, w.height, w.flags}
	{
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

		//SDL_SetWindowAspectRatio(m_Window.pWindow, 19 / 6.f, 19 / 6.f);
		//SDL_SetDisplayContentScale()
		//SDL_SetRenderScale(m_pRenderer, 2.0f, 2.0f);
		//SDL_SetWindowFullscreen(m_Window.pWindow, true);
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
		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 100, 255);
		SDL_RenderClear(m_pRenderer);

		SDL_RenderPresent(m_pRenderer);
	}
}
