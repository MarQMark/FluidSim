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
        simulation/SpriteRenderSystem.cpp
        simulation/ParticleRenderSystem.cpp

        #Kikan files
        kikan/glew/glew.c
        kikan/Engine.cpp
        kikan/ecs/Scene.cpp
        kikan/ecs/Entity.cpp
        kikan/opengl/Renderer.cpp
        kikan/ecs/systems/PhysicsSystem.cpp
        kikan/ecs/TypeRegistry.cpp
        kikan/opengl/Shader.cpp
        kikan/opengl/batching/Batch.cpp
        kikan/opengl/buffers/VertexBuffer.cpp
        kikan/opengl/batching/AutoBatch.cpp
        kikan/opengl/batching/ManuelBatch.cpp
        kikan/opengl/VertexRegistry.cpp
        kikan/opengl/buffers/IndexBuffer.cpp
        kikan/opengl/buffers/Texture2D.cpp
        kikan/ecs/systems/SpriteRenderSystem.cpp
        kikan/opengl/Camera.cpp
        kikan/input/Input.cpp)
add_executable(FluidSim ${SOURCE_FILES})

target_include_directories(FluidSim
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw/include)

target_link_directories(${PROJECT_NAME} PRIVATE external/glfw/src)

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


#define GLEW_STATIC (necessary for glew)
add_compile_definitions(GLEW_STATIC)

#link all necessary libraries for opengl
if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(FluidSim glut GL dl X11 pthread glfw)
endif(UNIX)

if (WIN32)
    target_link_libraries(FluidSim opengl32 glfw)
endif(WIN32)