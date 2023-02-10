#include "FluidSim/MapTree.h"
#include "../imgui/imgui.h"

MapTree::MapTree(std::vector<MapFile*>* maps) : _maps(maps) {
}

void MapTree::render() {
    ImGui::Begin("Maps");

    for (auto* map : *_maps) {
        if(ImGui::TreeNode(map->getName().c_str())){

            auto my_tex_id = reinterpret_cast<void*>(map->getIcon()->get());
            float my_tex_w = ImGui::GetWindowHeight() / 5.f * ((float)map->getWidth() / (float)map->getHeight());
            float my_tex_h = ImGui::GetWindowHeight() / 5.f;

            ImVec2 size = ImVec2(my_tex_w, my_tex_h);               // Size of the image we want to make visible
            ImVec2 uv0 = ImVec2(0.0f, 1.0f);                            // UV coordinates for lower-left
            ImVec2 uv1 = ImVec2(1.0f, 0.0f);    // UV coordinates for (32,32) in our texture
            ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
            if (ImGui::ImageButton("map", my_tex_id, size, uv0, uv1, bg_col, tint_col))
                _loaded = map;

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Size: %i x %i", map->getWidth(), map->getHeight());
                ImGui::Text("Click to Load");
                ImGui::EndTooltip();
            }

            ImGui::TreePop();
        }
    }

    ImGui::End();
}

MapFile *MapTree::getLoaded() {
    return _loaded;
}
