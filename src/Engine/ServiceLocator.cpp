#include "ServiceLocator.h"

//Default intitialize systems in the service locator
namespace NesEm
{
	std::unique_ptr<Renderer> ServiceLocator::m_pRenderer{ std::make_unique<NullRenderer>() };
}