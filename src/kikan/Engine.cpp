#include <iostream>
#include <sstream>
#include "Kikan/Engine.h"

namespace Kikan {
    bool Engine::shouldRun() const {
        return _should_run;
    }

    Scene *Engine::getScene(const std::string& name) {

        //if no scene exists create default scene
        if(_scenes.capacity() == 0){
            Scene* defaultScene = new Scene("default", _renderer, _input);
            _scenes.push_back(defaultScene);
            return defaultScene;
        }

        //look for specified scene
        for (Scene* scene : _scenes) {
            if (scene->name() == name)
                return scene;
        }

        //return default if no scene found
        return _scenes.at(0);
    }


    void Engine::addScene(const std::string& name) {
        //TODO implement function
    }

    void Engine::update_fps(){
        //get delta time
        _dt = ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - _last_time)).count();
        _last_time = std::chrono::high_resolution_clock::now();

        //if dt is higher than a day -> error -> set to 0
        if(_dt > 1e7)
            _dt = 1e-7;

        _frames_last_second++;

        // set window title
        if(_time_last_second >= 500.0)
        {
            time.fps = _frames_last_second * 2;
            _time_last_second = 0;
            _frames_last_second = 0;
        }

        _time_last_second += _dt;

        time.dt = _dt;
        time.tt += _dt;
    }

    void Engine::update() {
        update_fps();
        update_title();

        if(glfwWindowShouldClose(_renderer->getWindow())){
            _should_run = false;
            return;
        }

        if(preUpdate) preUpdate(this);

        // UPDATE
        _curr_scene->update(_dt);

        if(postUpdate) postUpdate(this);
        if(preRender) preRender(this);

        // RENDER
        _renderer->render(_dt);

        if(postRender != nullptr) postRender(this);
    }

    Renderer *Engine::getRenderer() {
        return _renderer;
    }

    void Engine::setCurrScene(const std::string& name) {
        _curr_scene = getScene(name);
    }

    Input *Engine::getInput() {
        return _input;
    }

    void Engine::setTitle(std::string &title) {
        _title = title;
    }

    std::string Engine::getTitle() {
        return _title;
    }

    void Engine::update_title() {
        if(_title.empty()){
            std::stringstream ss;
            ss << "KIKAN - FPS: " << time.fps;
            glfwSetWindowTitle(_renderer->getWindow(), ss.str().c_str());
        }
        else {
            glfwSetWindowTitle(_renderer->getWindow(), _title.c_str());
        }
    }

}

