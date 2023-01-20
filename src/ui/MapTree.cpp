#include "FluidSim/MapTree.h"
#include "../imgui/imgui.h"

MapTree::MapTree(std::vector<MapFile*>* maps) : _maps(maps) {
}

void MapTree::render() {
    ImGui::Begin("Maps");
    for (auto* map : *_maps) {
        if(ImGui::TreeNode(map->getName().c_str())){

            ImTextureID my_tex_id = reinterpret_cast<void*>(map->getTexture()->get());
            float my_tex_w = ImGui::GetWindowHeight() / 5.f * ((float)map->getWidth() / (float)map->getHeight());
            float my_tex_h = ImGui::GetWindowHeight() / 5.f;
            {
                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImVec2 uv_min = ImVec2(0.0f, 1.0f);                 // Top-left
                ImVec2 uv_max = ImVec2(1.0f, 0.0f);                 // Lower-right
                ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
                ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
                ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
                /*if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    float region_sz = 32.0f;
                    float region_x = ImGui::GetIO().MousePos.x - pos.x - region_sz * 0.5f;
                    float region_y = ImGui::GetIO().MousePos.y - pos.y - region_sz * 0.5f;
                    float zoom = 4.0f;
                    if (region_x < 0.0f) { region_x = 0.0f; }
                    else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
                    if (region_y < 0.0f) { region_y = 0.0f; }
                    else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
                    ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                    ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                    ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                    ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                    ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
                    ImGui::EndTooltip();
                }*/
            }

            if(ImGui::Button("Load"))
                _loaded = map;

            ImGui::TreePop();
        }
    }

    ImGui::End();
}

MapFile *MapTree::getLoaded() {
    return _loaded;
}
