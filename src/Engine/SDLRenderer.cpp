#include "SDLRenderer.h"

namespace NesEm
{
	SDLRenderer::SDLRenderer(Window const& w) :
		m_Window{w.title, w.width, w.height, w.flags}
	{
		if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
			SDL_Quit();
		}

		m_pRenderer = SDL_CreateRenderer(m_Window.pWindow, NULL);
		SDL_assert(m_pRenderer);
		if (!m_pRenderer)
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
			SDL_Quit();
		}
	}

	SDLRenderer::~SDLRenderer()
	{
		//Destroy the renderer
		SDL_DestroyRenderer(m_pRenderer);

		//Quit specific subsystems and all of SDL
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		SDL_Quit();
	}

	void SDLRenderer::Render() const
	{

	}
}
