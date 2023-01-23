#include <iostream>
#include <sstream>
#include "FluidSim/ViewSpace.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"
#include "IconFontAwesome/IconsFontAwesome5.h"

ViewSpace::ViewSpace(Kikan::Texture2D *txt, MapFile* mapFile) : _txt(txt), _mf(mapFile){
    _controls = new Controls;
}

ViewSpace::~ViewSpace() {
    delete _controls;
}

void ViewSpace::render() {
    float padding = 10;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
    ImGui::Begin("Simulation", nullptr, ImGuiWindowFlags_MenuBar);
    _width = ImGui::GetWindowWidth();
    _height = ImGui::GetWindowHeight();

    if(ImGui::BeginMenuBar()) {
        _controls->PAUSE = _controls->PAUSE != ImGui::Button(_controls->PAUSE ? ICON_FA_PLAY : ICON_FA_PAUSE, ImVec2(20, 20));

        ImGui::Separator();
        if(ImGui::Button(ICON_FA_TRASH, ImVec2(20, 20)))
            _controls->RESET = true;

        ImGui::Separator();
        const char* modes[] ={ICON_FA_PEN" Spawn", ICON_FA_ERASER" Erase", ICON_FA_ARROW_DOWN" Force"};
        ImGui::Text("Mode");
        ImGui::SetNextItemWidth(100);
        ImGui::Combo("##Mode", &_controls->BRUSH_MODE, modes, 3);
        ImGui::Separator();

        switch (_controls->BRUSH_MODE) {
            case Controls::BMT::M_SPAWN: {
                std::stringstream ss;
                ss << _controls->PEN_SIZE;
                std::string buf1(ss.str());

                ImGui::Text("Size");
                ImGui::SetNextItemWidth(60);
                ImGui::InputText("##Size", &buf1, ImGuiInputTextFlags_CharsDecimal);
                ImGui::Separator();
                ImGui::Text("Single Press");
                ImGui::Checkbox("##checkbox", &_controls->SINGLE_SPAWN);

                _controls->PEN_SIZE = std::stof(buf1);
                break;
            }
            case Controls::BMT::M_ERASE:{
                std::stringstream ss;
                ss << _controls->ERASER_SIZE;
                std::string buf1(ss.str());

                ImGui::Text("Size");
                ImGui::SetNextItemWidth(60);
                ImGui::InputText("##Size", &buf1, ImGuiInputTextFlags_CharsDecimal);

                _controls->ERASER_SIZE = std::stof(buf1);
                break;
            }
            case Controls::BMT::M_FORCE:{
                std::stringstream ss;
                ss << _controls->FORCE;
                std::string buf1(ss.str());

                const char* forceModes[] ={ICON_FA_EXPAND_ARROWS_ALT" Push", ICON_FA_COMPRESS_ARROWS_ALT" Pull"};
                ImGui::Text("Type");
                ImGui::SetNextItemWidth(80);
                ImGui::Combo("##Force Mode", &_controls->FORCE_MODE, forceModes, 2);
                ImGui::Separator();

                ImGui::Text("Force");
                ImGui::SetNextItemWidth(80);
                ImGui::InputText("##Force", &buf1, ImGuiInputTextFlags_CharsDecimal);

                _controls->FORCE = std::stof(buf1);
                break;
            }
        }

        ImGui::EndMenuBar();
    }

    float middleX = ImGui::GetWindowPos().x + _width / 2.f;
    float middleY = ImGui::GetWindowPos().y + _height / 2.f + 20;
    ImVec2 min(middleX - (_width / 2.f) + padding, middleY - (_width / 2.f) + padding);
    ImVec2 max(middleX + (_width / 2.f) - padding, middleY + (_width / 2.f) - padding);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddImage(reinterpret_cast<ImTextureID>(_txt->get()),
                       min,
                       max,
                       ImVec2(0, 1),
                       ImVec2(1, 0));

    if(ImGui::IsWindowFocused())
        _zoom *= 1.f - 0.2f * ImGui::GetIO().MouseWheel;


    ImVec2 mousePos = ImGui::GetIO().MousePos;
    if(_controls->BRUSH_MODE == Controls::BMT::M_SPAWN){
        if (mousePos.x > min.x && mousePos.y > min.y + 20 && mousePos.x < max.x && mousePos.y < max.y - 20)
        {
            float f =  (_width / 2.f - padding) / ((float)_mf->getWidth() * _zoom);
            ImGui::GetWindowDrawList()->AddCircle(mousePos, _controls->PEN_SIZE  * f, ImColor(ImVec4(0.2f, .4f, .8f, .8f)), 64, 2.f);
        }
    }
    else if(_controls->BRUSH_MODE == Controls::BMT::M_ERASE){
        if (mousePos.x > min.x && mousePos.y > min.y + 20 && mousePos.x < max.x && mousePos.y < max.y - 20)
        {
            float f =  (_width / 2.f - padding) / ((float)_mf->getWidth() * _zoom);
            ImGui::GetWindowDrawList()->AddCircle(mousePos, _controls->ERASER_SIZE  * f, ImColor(ImVec4(1.f, .2f, .2f, .8f)), 64, 2.f);
        }
    }

    _controls->MOUSE_IN_SPACE = (mousePos.x > min.x && mousePos.y > min.y + 70 && mousePos.x < max.x && mousePos.y < max.y);

    float scale = (float)_mf->getWidth() / (max.x - min.x - padding);
    _controls->MOUSE_X = (mousePos.x - middleX) * scale * _zoom + (float)_mf->getWidth() / 2.f;
    _controls->MOUSE_Y = (mousePos.y - middleY) * scale * -_zoom + (float)_mf->getHeight() / 2.f;

    ImGui::End();
    ImGui::PopStyleVar();
}

float ViewSpace::getWidth() const {
    return _width;
}

float ViewSpace::getHeight() const {
    return _height;
}

float ViewSpace::getZoom() const {
    return _zoom;
}

Controls *ViewSpace::getControls() {
    return _controls;
}

void ViewSpace::setMapFile(MapFile *mapFile) {
    _mf = mapFile;
}
