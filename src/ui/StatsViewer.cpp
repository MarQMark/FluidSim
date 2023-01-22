#include <sstream>
#include "FluidSim/StatsViewer.h"
#include "../imgui/imgui.h"

StatsViewer::StatsViewer() {
    _stats = new Stats();
}

StatsViewer::~StatsViewer() {
    delete _stats;
}

void StatsViewer::render() {
    ImGui::Begin("Stats");

    std::stringstream ss;
    ss << "FPS: ";
    ss << _stats->FPS;
    ImGui::Text(ss.str().c_str());
    ss.str(std::string());

    ss << "Particles: ";
    ss << _stats->PARTICLES;
    ImGui::Text(ss.str().c_str());
    ss.str(std::string());

    ss << "Lost Particles: ";
    ss << _stats->LOST_PARTICLES;
    ImGui::Text(ss.str().c_str());
    ss.str(std::string());

    ImGui::End();
}

Stats *StatsViewer::getStats() {
    return _stats;
}

