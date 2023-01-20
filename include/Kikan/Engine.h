#ifndef KIKAN_ENGINE_H
#define KIKAN_ENGINE_H

#include <chrono>
#include "vector"
#include "Kikan/ecs/Scene.h"
#include "Kikan/opengl/Renderer.h"
#include "Kikan/util/Time.h"

namespace Kikan {
    class Engine {
    public:
        Engine(int width = 1280, int height = 720){
            _renderer = new Renderer(width, height);
            _input = Input::create(_renderer->getWindow());
            setCurrScene();
        }

        ~Engine(){
            delete _input;
            delete _renderer;
        }

        bool shouldRun() const;

        void update();

        Scene* getScene(const std::string& = "default");
        void addScene(const std::string& name);
        void setCurrScene(const std::string& name = "default");

        Renderer* getRenderer();

        Input* getInput();

        Time time;

        void setTitle(std::string& title);
        std::string getTitle();

        // custom methods
        void (*preUpdate)(Engine*) = nullptr;
        void (*postUpdate)(Engine*) = nullptr;
        void (*preRender)(Engine*) = nullptr;
        void (*postRender)(Engine*) = nullptr;
    private:
        bool _should_run = true;

        std::chrono::high_resolution_clock::time_point _last_time;
        double _dt = 0;
        double _time_last_second = 0;
        unsigned int _frames_last_second = 0;

        std::string _title = std::string();

        Renderer* _renderer;
        std::vector<Scene*> _scenes;
        Scene* _curr_scene;

        Input* _input;

        void update_fps();
        void update_title();
    };
}

#endif //KIKAN_ENGINE_H
