set(MODULE_NAME Input)
set(TARGET_NAME Pargon${MODULE_NAME})
set(MAIN_HEADER Include/Pargon/${MODULE_NAME}.h)

set(PUBLIC_HEADERS
	Include/Pargon/Input/ControllerDevice.h
	Include/Pargon/Input/ControllerInput.h
	Include/Pargon/Input/ControllerToMotionEmulator.h
	Include/Pargon/Input/KeyboardDevice.h
	Include/Pargon/Input/KeyboardInput.h
	Include/Pargon/Input/MotionDevice.h
	Include/Pargon/Input/MotionInput.h
	Include/Pargon/Input/MouseDevice.h
	Include/Pargon/Input/MouseInput.h
	Include/Pargon/Input/MouseToTouchEmulator.h
	Include/Pargon/Input/TouchDevice.h
	Include/Pargon/Input/TouchInput.h
)

set(SOURCES
	Source/Core/ControllerDevice.cpp
	Source/Core/ControllerInput.cpp
	Source/Core/ControllerToMotionEmulator.cpp
	Source/Core/KeyboardDevice.cpp
	Source/Core/MotionDevice.cpp
	Source/Core/MouseDevice.cpp
	Source/Core/MouseToTouchEmulator.cpp
	Source/Core/TouchDevice.cpp
)

set(DEPENDENCIES
	PargonApplication
	PargonContainers
	PargonMath
)

set(WIN32_SOURCES
	Source/Win32/Win32KeyboardInput.cpp
	Source/Win32/Win32KeyboardInput.h
	Source/Win32/Win32MouseInput.cpp
	Source/Win32/Win32MouseInput.h
)

set(XINPUT_SOURCES
	Source/XInput/XInputControllerInput.cpp
	Source/XInput/XInputControllerInput.h
)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
	list(APPEND SOURCES ${WIN32_SOURCES})
endif()

include(CheckIncludeFiles)
check_include_files(windows.h\;xinput.h HAS_XINPUT) # Just looking for xinput.h doesn't work.

if(HAS_XINPUT)
	list(APPEND SOURCES ${XINPUT_SOURCES})
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Include/Pargon/" FILES ${MAIN_HEADER})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Include/Pargon/${MODULE_NAME}/" PREFIX Include FILES ${PUBLIC_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Source/" FILES ${SOURCES})

add_library(${TARGET_NAME} STATIC)
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
target_include_directories(${TARGET_NAME} PUBLIC Include)
target_include_directories(${TARGET_NAME} PRIVATE Source)
target_link_libraries(${TARGET_NAME} PUBLIC ${DEPENDENCIES})
target_sources(${TARGET_NAME} PRIVATE "${MAIN_HEADER}" "${PUBLIC_HEADERS}" "${SOURCES}")
