#include "Kikan/opengl/buffers/Texture2D.h"

namespace Kikan {

    Texture2D::Texture2D(GLsizei width, GLsizei height, float data[], GLenum filter) : _width(width), _height(height){
        gen(filter);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_FLOAT, data);
        unbind();
    }
    Texture2D::Texture2D(GLsizei width, GLsizei height, unsigned char data[], GLenum filter) : _width(width), _height(height){
        gen(filter);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        unbind();
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &_id);
    }

    void Texture2D::bind(GLuint slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, _id);
    }

    void Texture2D::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint Texture2D::get() const {
        return _id;
    }

    void Texture2D::gen(GLenum filter) {
        glGenTextures(1, &_id);
        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void Texture2D::set(GLsizei width, GLsizei height, float *data) {
        _width = width;
        _height = height;

        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_FLOAT, data);
        unbind();
    }

    void Texture2D::set(GLsizei width, GLsizei height, unsigned char *data) {
        _width = width;
        _height = height;

        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        unbind();
    }
}