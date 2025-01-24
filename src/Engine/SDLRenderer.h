#ifndef NES_EMULATOR_SDL_RENDERER
#define NES_EMULATOR_SDL_RENDERER

//3rd party
#pragma warning push 4820
#include <SDL3/SDL.h>
#pragma warning pop

//Engine
#include "SDLWindow.h"
#include "Window.h"
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

		virtual void ToggleFullScreen() noexcept override;

	private:
		SDLWindow m_Window;
		SDL_Renderer* m_pRenderer{ nullptr };
	};
}

#endif