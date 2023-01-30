#include <string>
#include <sstream>
#include "FluidSim/ConstantsEditor.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"

ConstantsEditor::ConstantsEditor() {
    _constants = new Constants();
}

void ConstantsEditor::render() {

    ImGui::Begin("Constants");

    #pragma region float to string
    std::stringstream ss;
    ss << _constants->RADIUS;
    std::string buf1(ss.str());
    ss.str(std::string());
    ss << _constants->SIGMA;
    std::string buf2(ss.str());
    ss.str(std::string());
    ss << _constants->BETA;
    std::string buf3(ss.str());
    ss.str(std::string());
    ss << _constants->P0;
    std::string buf4(ss.str());
    ss.str(std::string());
    ss << _constants->STIFFNESS;
    std::string buf5(ss.str());
    ss.str(std::string());
    ss << _constants->STIFFNESS_NEAR;
    std::string buf6(ss.str());
    ss.str(std::string());
    ss << _constants->FRICTION;
    std::string buf9(ss.str());
    ss.str(std::string());
    ss << _constants->COLLISION_RADIUS;
    std::string buf10(ss.str());
    ss.str(std::string());
    ss << _constants->RENDER_RADIUS;
    std::string buf11(ss.str());
    ss.str(std::string());
    ss << _constants->RENDER_SMOOTHING;
    std::string buf12(ss.str());
    ss.str(std::string());
    #pragma endregion

    #pragma region UI Elements
    ImGui::Text("Radius");
    ImGui::InputText("##Radius", &buf1, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Sigma");
    ImGui::InputText("##Sigma", &buf2, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Beta");
    ImGui::InputText("##Beta", &buf3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("P0");
    ImGui::InputText("##P0", &buf4, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Stiffness");
    ImGui::InputText("##Stiffness", &buf5, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Stiffness near");
    ImGui::InputText("##Stiffness near", &buf6, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Friction");
    ImGui::InputText("##Friction", &buf9, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Collision Radius");
    ImGui::InputText("##Collision Radius", &buf10, ImGuiInputTextFlags_CharsDecimal);

    ImGui::Text("\n");
    ImGui::Separator();
    ImGui::Text("\n");

    ImGui::Text("Render Radius");
    ImGui::InputText("##Render Radius", &buf11, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Smoothing < 30!!!");
    ImGui::InputText("##Smoothing", &buf12, ImGuiInputTextFlags_CharsDecimal);
    #pragma endregion


    if(!buf1.empty())
        _constants->RADIUS = std::stof(buf1);
    if(!buf2.empty())
        _constants->SIGMA = std::stof(buf2);
    if(!buf3.empty())
        _constants->BETA = std::stof(buf3);
    if(!buf4.empty())
        _constants->P0 = std::stof(buf4);
    if(!buf5.empty())
        _constants->STIFFNESS = std::stof(buf5);
    if(!buf6.empty())
        _constants->STIFFNESS_NEAR = std::stof(buf6);
    if(!buf9.empty())
        _constants->FRICTION = std::stof(buf9);
    if(!buf10.empty())
        _constants->COLLISION_RADIUS = std::stoi(buf10);
    if(!buf11.empty())
        _constants->RENDER_RADIUS = std::stof(buf11);
    if(!buf12.empty())
        _constants->RENDER_SMOOTHING = std::stoi(buf12);


    ImGui::End();
}

Constants *ConstantsEditor::getConstants() {
    return _constants;
}

ConstantsEditor::~ConstantsEditor() {
    delete _constants;
}