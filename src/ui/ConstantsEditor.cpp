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
    ss << _constants->MAX_PRESS;
    std::string buf7(ss.str());
    ss.str(std::string());
    ss << _constants->MAX_D;
    std::string buf8(ss.str());
    ss.str(std::string());
    ss << _constants->FRICTION;
    std::string buf9(ss.str());
    ss.str(std::string());
    ss << _constants->COLLISION_RADIUS;
    std::string buf10(ss.str());
    ss.str(std::string());
    ss << _constants->MAX_VEL;
    std::string buf11(ss.str());
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
    ImGui::Text("Max pressure");
    ImGui::InputText("##Max pressure", &buf7, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Max Displacement");
    ImGui::InputText("##Max Displacement", &buf8, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Friction");
    ImGui::InputText("##Friction", &buf9, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Collision Radius");
    ImGui::InputText("##Collision Radius", &buf10, ImGuiInputTextFlags_CharsDecimal);
    ImGui::Text("Max Velocity");
    ImGui::InputText("##Max Velocity", &buf11, ImGuiInputTextFlags_CharsDecimal);
    #pragma endregion


    _constants->RADIUS = std::stof(buf1);
    _constants->SIGMA = std::stof(buf2);
    _constants->BETA = std::stof(buf3);
    _constants->P0 = std::stof(buf4);
    _constants->STIFFNESS = std::stof(buf5);
    _constants->STIFFNESS_NEAR = std::stof(buf6);
    _constants->MAX_PRESS = std::stof(buf7);
    _constants->MAX_D = std::stof(buf8);
    _constants->FRICTION = std::stof(buf9);
    _constants->COLLISION_RADIUS = std::stoi(buf10);
    _constants->MAX_VEL = std::stof(buf11);


    ImGui::End();
}

Constants *ConstantsEditor::getConstants() {
    return _constants;
}

ConstantsEditor::~ConstantsEditor() {
    delete _constants;
}