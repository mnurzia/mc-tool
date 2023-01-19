cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(biomeplus C CXX)

add_subdirectory(ext/lib/fmt)


################################################################################
# Common utils
################################################################################

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################

set(PROJECT_NAME biomeplus)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "src/app/App.hpp"
    "src/app/components/fontmanager/FontManager.hpp"
    "src/app/components/functioneditor/FunctionEditor.hpp"
    "src/app/components/functioneditor/LangDef.hpp"
    "src/app/components/hexeditor/HexEditor.hpp"
    "src/app/components/javarng/JavaRng.hpp"
    "src/app/components/logview/LogView.hpp"
    "src/app/components/logview/Sink.hpp"
    "src/app/components/menubar/MenuBar.hpp"
    "src/app/components/nbtview/NBTView.hpp"
    "src/app/components/nbtview/schema/Annotator.hpp"
    "src/app/components/newdialogs/NewWorldView.hpp"
    "src/app/components/PersistentWindowComponent.hpp"
    "src/app/components/threadstatus/ThreadStatus.hpp"
    "src/app/components/UIComponent.hpp"
    "src/app/components/uuidgen/UUIDGen.hpp"
    "src/app/components/WindowComponent.hpp"
    "src/app/components/workspace/Workspace.hpp"
    "src/app/components/worldview/Fragment.hpp"
    "src/app/components/worldview/FragmentManager.hpp"
    "src/app/components/worldview/FragmentShaders.hpp"
    "src/app/components/worldview/FragmentTask.hpp"
    "src/app/components/worldview/WorldView.hpp"
    "src/app/dispatchqueue/DispatchQueue.hpp"
    "src/app/dispatchqueue/Task.hpp"
    "src/app/dispatchqueue/TaskGroup.hpp"
    "src/app/Icons.hpp"
    "src/app/ImGuiEx.hpp"
    "src/app/resource/container/Container.hpp"
    "src/app/resource/handle/Handle.hpp"
    "src/app/resource/Manager.hpp"
    "src/app/resource/provider/Provider.hpp"
    "src/nbt/Deserializer.hpp"
    "src/nbt/Loader.hpp"
    "src/nbt/SerialBase.hpp"
    "src/nbt/Serializer.hpp"
    "src/nbt/Tag.hpp"
    "src/nbt/TagByte.hpp"
    "src/nbt/TagByteArray.hpp"
    "src/nbt/TagCompound.hpp"
    "src/nbt/TagDouble.hpp"
    "src/nbt/TagFloat.hpp"
    "src/nbt/TagInt.hpp"
    "src/nbt/TagIntArray.hpp"
    "src/nbt/TagList.hpp"
    "src/nbt/TagLong.hpp"
    "src/nbt/TagLongArray.hpp"
    "src/nbt/TagRoot.hpp"
    "src/nbt/TagShort.hpp"
    "src/nbt/TagString.hpp"
    "src/util/Color.hpp"
    "src/util/UUID.hpp"
    "src/worldgen/_BiomeData.hpp"
    "src/worldgen/Biome.hpp"
    "src/worldgen/Feature.hpp"
    "src/worldgen/Generator.hpp"
    "src/worldgen/JavaRandom.hpp"
    "src/worldgen/Layer.hpp"
    "src/worldgen/Map.hpp"
    "src/worldgen/OceanGen.hpp"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "ext/src/glad/glad.c"
    "ext/include/imgui/backends/imgui_impl_glfw.cpp"
    "ext/include/imgui/backends/imgui_impl_opengl3.cpp"
    "ext/include/imgui/imgui.cpp"
    "ext/include/imgui/imgui_demo.cpp"
    "ext/include/imgui/imgui_draw.cpp"
    "ext/include/imgui/imgui_widgets.cpp"
    "ext/include/imgui/imgui_tables.cpp"
    "ext/include/imgui/misc/cpp/imgui_stdlib.cpp"
    "ext/src/imgui_textedit/imgui_textedit.cpp"
    "ext/src/spdlog/async.cpp"
    "ext/src/spdlog/cfg.cpp"
    "ext/src/spdlog/spdlog.cpp"
    "ext/src/spdlog/color_sinks.cpp"
    "ext/src/spdlog/file_sinks.cpp"
    "ext/src/spdlog/stdout_sinks.cpp"
    "src/app/App.cpp"
    "src/app/components/fontmanager/FontManager.cpp"
    "src/app/components/functioneditor/FunctionEditor.cpp"
    "src/app/components/functioneditor/LangDef.cpp"
    "src/app/components/hexeditor/HexEditor.cpp"
    "src/app/components/javarng/JavaRng.cpp"
    "src/app/components/logview/LogView.cpp"
    "src/app/components/logview/Sink.cpp"
    "src/app/components/menubar/MenuBar.cpp"
    "src/app/components/nbtview/NBTView.cpp"
    "src/app/components/nbtview/schema/Annotator.cpp"
    "src/app/components/newdialogs/NewWorldView.cpp"
    "src/app/components/PersistentWindowComponent.cpp"
    "src/app/components/threadstatus/ThreadStatus.cpp"
    "src/app/components/UIComponent.cpp"
    "src/app/components/uuidgen/UUIDGen.cpp"
    "src/app/components/WindowComponent.cpp"
    "src/app/components/workspace/Workspace.cpp"
    "src/app/components/worldview/Fragment.cpp"
    "src/app/components/worldview/FragmentManager.cpp"
    "src/app/components/worldview/FragmentShaders.cpp"
    "src/app/components/worldview/FragmentTask.cpp"
    "src/app/components/worldview/WorldView.cpp"
    "src/app/dispatchqueue/DispatchQueue.cpp"
    "src/app/dispatchqueue/Task.cpp"
    "src/app/dispatchqueue/TaskGroup.cpp"
    "src/app/ImGuiEx.cpp"
    "src/app/resource/container/Container.cpp"
    "src/app/resource/handle/Handle.cpp"
    "src/app/resource/Manager.cpp"
    "src/app/resource/provider/Provider.cpp"
    "src/Main.cpp"
    "src/nbt/Deserializer.cpp"
    "src/nbt/Loader.cpp"
    "src/nbt/SerialBase.cpp"
    "src/nbt/Serializer.cpp"
    "src/nbt/Tag.cpp"
    "src/nbt/TagByte.cpp"
    "src/nbt/TagByteArray.cpp"
    "src/nbt/TagCompound.cpp"
    "src/nbt/TagDouble.cpp"
    "src/nbt/TagFloat.cpp"
    "src/nbt/TagInt.cpp"
    "src/nbt/TagIntArray.cpp"
    "src/nbt/TagList.cpp"
    "src/nbt/TagLong.cpp"
    "src/nbt/TagLongArray.cpp"
    "src/nbt/TagRoot.cpp"
    "src/nbt/TagShort.cpp"
    "src/nbt/TagString.cpp"
    "src/util/Color.cpp"
    "src/util/UUID.cpp"
    "src/worldgen/Biome.cpp"
    "src/worldgen/Feature.cpp"
    "src/worldgen/Generator.cpp"
    "src/worldgen/Layer.cpp"
    "src/worldgen/Map.cpp"
    "src/worldgen/OceanGen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

add_executable(${PROJECT_NAME} ${ALL_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "ext/include")

target_include_directories(${PROJECT_NAME} PUBLIC "/opt/homebrew/include")
target_include_directories(${PROJECT_NAME} PUBLIC "/opt/homebrew/include/freetype2")
target_include_directories(${PROJECT_NAME} PUBLIC "/opt/homebrew/Cellar/zlib/1.2.13/include/")

target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_COMPILED_LIB SPDLOG_FMT_EXTERNAL)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_compile_options(${PROJECT_NAME} PUBLIC -g)

target_link_directories(${PROJECT_NAME} PUBLIC "/opt/homebrew/lib")

target_link_libraries(${PROJECT_NAME} PRIVATE "glfw.3" "fmt" "freetype" "z" )


