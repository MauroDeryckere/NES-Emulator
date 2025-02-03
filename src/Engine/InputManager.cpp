#include "InputManager.h"

#include "SDL3/SDL.h"

namespace NesEm
{
	bool InputManager::ProcessInput() noexcept
	{
		// Clear last frames info
		m_KeyCodes.clear();
		m_KeyCodes.resize(static_cast<size_t>(InputAction::EventType::COUNT));


		SDL_Event event{ };
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_EVENT_QUIT:
				{
					return false;
				}break;
				case SDL_EVENT_KEY_DOWN:
				{
					m_KeyCodes[static_cast<size_t>(InputAction::EventType::KeyDown)].emplace(static_cast<uint16_t>(event.key.scancode));
				}break;
				case SDL_EVENT_KEY_UP:
				{
					m_KeyCodes[static_cast<size_t>(InputAction::EventType::KeyUp)].emplace(static_cast<uint16_t>(event.key.scancode));
				}break;
			}
		}

		return true;
	}
	void InputManager::AddAction(InputAction const& action) noexcept
	{
		// First add the action to our actions map if it does not exist yet
		auto [it, inserted] = m_ActionsMap.emplace(
			action.actionName,
			// Insert a vector resized to the amount of event types we have
			std::vector<std::vector<uint16_t>>(static_cast<size_t>(InputAction::EventType::COUNT))
		);

		// add the specific scancode
		it->second[static_cast<size_t>(action.type)].emplace_back(action.keyboardKeyCode);

	}

	bool InputManager::IsActionExecuted(std::string const& actionName) const noexcept
	{
		auto it = m_ActionsMap.find(actionName);
		if (it == end(m_ActionsMap))
		{
			SDL_Log("%s, %s", "Action name not found ->" , actionName);
			assert(false);
			return false;
		}

		auto const& vec{ it->second };
		for (size_t i{ 0 }; i < vec.size(); ++i)
		{
			for (auto key : vec[i])
			{
				auto setIt{ m_KeyCodes[i].find(key) };
				if (setIt != end(m_KeyCodes[i]))
				{
					return true;
				}
			}
		}

		return false;
	}
}