project(FluidSim)

set(SOURCE_FILES
        main.cpp
        simulation/SimulationSystem.cpp
        simulation/Grid.cpp
        simulation/DistanceField.cpp
        simulation/FluidSimulation.cpp
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_glfw.cpp
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_stdlib.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        ui/ViewSpace.cpp
        ui/ConstantsEditor.cpp
        ui/MapTree.cpp
        etc/MapFile.cpp
        file_browser/ImGuiFileBrowser.cpp
        ui/StatsViewer.cpp
        simulation/GridRenderSystem.cpp
        simulation/SpriteRenderSystem.cpp simulation/ParticleRenderSystem.cpp)
add_executable(FluidSim ${SOURCE_FILES})

target_include_directories(FluidSim
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)

#copy kikan shaders to target folder
add_custom_target(copy_kikan_shaders ALL
        COMMAND  ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../kikan_shaders
        ${PROJECT_BINARY_DIR}/shaders
        COMMENT "Copying Kikan shaders into binary directory")
add_dependencies(FluidSim copy_kikan_shaders)

#copy shaders to target folder
add_custom_target(copy_shaders ALL
        COMMAND  ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../shaders
        ${PROJECT_BINARY_DIR}/shaders
        COMMENT "Copying shaders into binary directory")
add_dependencies(FluidSim copy_shaders)

#copy shaders to target folder
add_custom_target(copy_assets ALL
        COMMAND  ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../assets
        ${PROJECT_BINARY_DIR}/assets
        COMMENT "Copying assets into binary directory")
add_dependencies(FluidSim copy_assets)

#link Kikan
find_library(LIB_Kikan
        NAMES libKikan.a
        PATHS ${PROJECT_SOURCE_DIR}/../lib)
target_link_libraries(FluidSim ${LIB_Kikan})

#define GLEW_STATIC (necessary for glew)
add_compile_definitions(GLEW_STATIC)

#link GLFW
find_library(LIB_GLFW
        NAMES libglfw3.a
        PATHS ${PROJECT_SOURCE_DIR}/../lib)
target_link_libraries(FluidSim ${LIB_GLFW})

#link all necessary libraries for opengl
if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(FluidSim glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(FluidSim opengl32)
endif(WIN32)