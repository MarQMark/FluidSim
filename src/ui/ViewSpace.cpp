#include <iostream>
#include "FluidSim/ViewSpace.h"
#include "../imgui/imgui.h"

ViewSpace::ViewSpace(Kikan::Texture2D *txt) : _txt(txt){
}

void ViewSpace::render() {
    float padding = 10;

    ImGui::Begin("Simulation");
    _width = ImGui::GetWindowWidth();
    _height = ImGui::GetWindowHeight();

    float middleX = ImGui::GetWindowPos().x + _width / 2.f;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddImage(reinterpret_cast<ImTextureID>(_txt->get()),
                       ImVec2(middleX - (_height / 2.f) + padding, ImGui::GetWindowPos().y + padding + 20),
                       ImVec2(middleX + (_height / 2.f) - padding, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - padding),
                       ImVec2(0, 1),
                       ImVec2(1, 0));
    ImGui::End();
}

float ViewSpace::getWidth() const {
    return _width;
}

float ViewSpace::getHeight() const {
    return _height;
}

