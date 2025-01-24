#ifndef NES_EMULATOR_SERVICE_LOCATOR
#define NES_EMULATOR_SERVICE_LOCATOR

#include <memory>
#include "Renderer.h"

namespace NesEm
{
	class ServiceLocator final
	{
	public:
		[[nodiscard]] static Renderer& GetRenderer() { return (*m_pRenderer); }
		static void RegisterRenderer(std::unique_ptr<Renderer>&& pRenderer)
		{
			m_pRenderer = ((!pRenderer) ? std::make_unique<NullRenderer>() : std::move(pRenderer));
		}

	private:
		static std::unique_ptr<Renderer> m_pRenderer;
	};

}

#endif