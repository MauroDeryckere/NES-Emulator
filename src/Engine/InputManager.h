#ifndef NES_EMULATOR_INPUTMANAGER
#define NES_EMULATOR_INPUTMANAGER

#include "Singleton.h"

#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <unordered_set>

namespace NesEm
{
	class InputManager : public Singleton<InputManager>
	{
		public:
			// for now to keep things simple we only support keyboard keys, later on we should also consider gamepad, ...
			struct InputAction final
			{
				// The key that will be used to reference any actions
				std::string const actionName{};

				// The event type that will be used to know when an action is executed	
				enum class EventType : uint8_t
				{
					KeyUp,
					KeyDown,
					COUNT
				};
				// What is the scancode we look for
				uint16_t keyboardKeyCode{};
				EventType type{};
			};

			// Should be called once per frame
			// Return bool Should we quit the application? 
			bool ProcessInput() noexcept;


			// Param action The action struct with all required info to add to the input manager
			void AddAction(InputAction const& action) noexcept;

			// Param string The name of the action you wish to poll
			// Return bool Is this action executed or not
			[[nodiscard]] bool IsActionExecuted(std::string const& actionName) const noexcept;

			// A function could also be added to check executed scancodes directly if necessary in the future.

			InputManager(const InputManager&) = delete;
			InputManager(InputManager&&) = delete;
			InputManager& operator=(const InputManager&) = delete;
			InputManager& operator=(const InputManager&&) = delete;

		private:
			friend class NesEm::Singleton<InputManager>;

			InputManager() = default;
			~InputManager() = default;

			// looks confusing at first but here's how it works
			// each action has a vector of different Event types
			// for each event type, this action may have multiple keybinds

			// maps actions to specific scancodes
			std::unordered_map<std::string, std::vector<std::vector<uint16_t>>> m_ActionsMap;

			// similarly here, a vector for each event type

			// scancodes we are listening to | SDL scancodes do not go past 512 so uint16 is sufficient.
			std::vector<std::unordered_set<uint16_t>> m_KeyCodes;
	};
}

#endif