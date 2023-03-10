#ifndef KIKAN_RENDERER_H
#define KIKAN_RENDERER_H

#include <map>
#include <string>
#include <cstring>
#include "glm/glm.hpp"
#include "Shader.h"
#include "Kikan/opengl/batching/AutoBatch.h"
#include "Kikan/opengl/batching/ManuelBatch.h"
#include "Kikan/opengl/vertices/DefaultVertex.h"
#include "Kikan/opengl/vertices/VertexRegistry.h"

namespace Kikan {
class Renderer {
    public:
        class Override{
        public:
            virtual void preRender(Renderer* renderer, double dt) = 0;
            virtual void postRender(Renderer* renderer, double dt) = 0;
        };

        Renderer(int width, int height) : _width(width), _height(height){
            setup_openGl();
        }

        ~Renderer(){
            glfwTerminate();
        }

        GLFWwindow* getWindow();
        void setWidth(int width);
        int getWidth();
        void setHeight(int height);
        int getHeight();

        glm::mat4x4 mvp;

        /*
        *  Uses Auto-batching with DefaultVertex.
        */
        void renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, float layer);

        /*
        *  Uses Auto-batching with DefaultVertex.
        */
        void renderQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color, float layer);

        /*
        *  Uses Auto-batching with DefaultVertex.
        *
        *  Uses Ear-Clipping-Algorithm to divide into Triangles.
        *  This means Polygons cannot intersect with themselves, cannot have holes and
        *  three or more vertices cannot form a line
        */
        void renderPolygon(std::vector<glm::vec2>& points, glm::vec4 color, float layer);

        /*
        * Uses Auto-batching with DefaultVertex.
        *
        * Texture ID of IVertex gets used for binding correct Texture.
        * In Vertex data textureID is always 0 since AutoBatching only allows one Texture per Batch
        *
        * Order of Points is:
        *      1 ------ 2
        *      |        |
        *      |        |
        *      4 ------ 3
        */
        void renderTexture2D(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, GLuint textureId, glm::vec4 color = glm::vec4(0), float layer = 0);

        /*
        *  Each Auto-Batch gets an unique ID: 4 Bytes Vertex Signature + 4 Bytes Texture ID
        */
        template <class T>
        void autoBatch(std::vector<IVertex*> vertices, std::vector<GLuint>* indices = nullptr);

        void addBatch(ManuelBatch* batch, unsigned int key);
        ManuelBatch* getBatch(unsigned int key);

        void render(double dt);
        void addPreRender(void (*func)(void* o, Renderer* renderer, double dt), void* o);
        void addPostRender(void (*func)(void* o, Renderer* renderer, double dt), void* o);
        void overrideRender(Override* ovr);

        Shader* shader(const std::string& name = "default");
        void shader(Shader* shader, const std::string& name = "default");

        static void queryErrors(const std::string& tag);
private:
        GLFWwindow *_window = nullptr;

        int _width;
        int _height;

        std::map<std::string, Shader*> _shaders;
        std::map<unsigned int, ManuelBatch*> _batches;

        std::map<uint64_t, AutoBatch*> _auto_batches;

        void (*preRender)(void* o, Renderer* renderer, double dt) = nullptr;
        void (*postRender)(void* o, Renderer* renderer, double dt) = nullptr;
        void* _o_pre_render = nullptr;
        void* _o_post_render = nullptr;
        Override* _override_render = nullptr;

        void setup_openGl();
        static uint64_t auto_batch_id(uint32_t signature, float textureID);
    };
}

#endif //KIKAN_RENDERER_H
