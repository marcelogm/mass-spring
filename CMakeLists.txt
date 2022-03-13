cmake_minimum_required (VERSION 3.8)

project (mass-spring)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG /mavx2")

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RUN_DIR ${PROJECT_SOURCE_DIR}/out/build)

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/out/build)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

set(COMMON_LIBS mass-spring ${OPENGL_LIBRARIES} glfw)
set(COMMON_HEADERS include/vgl.h)
set(COMMON_HEADERS include/stb_image.h)
set(COMMON_HEADERS include/stb_image_write.h)
set(IMGUI_SOURCE_DIR lib/imgui)

add_library(mass-spring
			lib/gl3w.c
			lib/LoadShaders.cpp
			${IMGUI_SOURCE_DIR}/imgui.cpp
			${IMGUI_SOURCE_DIR}/imgui_demo.cpp 
			${IMGUI_SOURCE_DIR}/imgui_draw.cpp 
			${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
			${IMGUI_SOURCE_DIR}/imgui_tables.cpp
			${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
			${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp 
			"src/Application.cpp" 
			"src/simulation/Frametime.cpp" 
			"src/configuration/Configuration.cpp" 
			"src/camera/Camera.cpp"  
			"src/entity/ObjectProvider.cpp"
			"src/entity/Object.cpp"
			"src/entity/Entity.cpp"
			"src/scene/Scene.cpp"
			"src/scene/Renderer.cpp"			
			"src/scene/ClothSceneFactory.cpp" 
			"src/gui/GuiRenderer.cpp" 
			"src/gui/WindowEngine.cpp" 
			"src/gui/CommandService.cpp"
			"src/gui/CommandStrategy.cpp" 
			"src/simulation/Simulator.cpp"
			"src/simulation/ExternalForce.cpp" "src/entity/Particle.cpp")
			
find_package(OpenMP)
if (OPENMP_FOUND)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(main WIN32 src/main.cpp ${COMMON_HEADERS} "src/entity/Particle.cpp")
target_link_libraries(main ${COMMON_LIBS})

include_directories(include)
include_directories(lib/glfw/include)
include_directories(lib/glm)
include_directories(lib/imgui)
