# Add any sources to the SOURCES variable in the parent scope
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/ServiceLocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/InputManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/SDLRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/cpp.hint
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/OpcodeHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/NESMemory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/NESCPU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/NESPPU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/NESCartridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/Emulator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/iosLaunchScreen.storyboard
    PARENT_SCOPE
)