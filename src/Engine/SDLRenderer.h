#ifndef NES_EMULATOR_SDL_RENDERER
#define NES_EMULATOR_SDL_RENDERER

#include <SDL3/SDL.h>

#include "SDLWindow.h"
#include "Renderer.h"

namespace NesEm
{
	class SDLRenderer final : public Renderer
	{
	public:
		SDLRenderer(Window const& w);
		~SDLRenderer() override;

		SDLRenderer(SDLRenderer const&) = delete;
		SDLRenderer(SDLRenderer&&) = delete;
		SDLRenderer& operator=(SDLRenderer const&) = delete;
		SDLRenderer& operator=(SDLRenderer&&) = delete;

		void Render() const override;

	private:
		SDLWindow m_Window;
	};
}

#endif