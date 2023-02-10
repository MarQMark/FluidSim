#include <iostream>
#include <fstream>
#include <sstream>
#include "Kikan/opengl/Shader.h"

namespace Kikan {
    Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
        std::string vertexSource = loadShaderSource(vertexPath);
        std::string fragmentSource = loadShaderSource(fragmentPath);

        int vs = compileShader(GL_VERTEX_SHADER, vertexSource);
        int fs = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        create_program(vs, fs);
    }

    Shader::~Shader() {
        glDeleteShader(_vs);
        glDeleteShader(_fs);
        glDeleteProgram(_id);
    }

    void Shader::create_program(GLuint vs, GLuint fs) {
        _id = glCreateProgram();

        glAttachShader(_id, vs);
        glAttachShader(_id, fs);
        glLinkProgram(_id);

        _vs = vs;
        _fs = fs;

        bind();
    }

    std::string Shader::loadShaderSource(const std::string &path) {
        std::stringstream ss;
        std::string line;
        std::ifstream source(path);

        while (std::getline(source, line))
            ss << line << std::endl;

        source.close();

        return ss.str();
    }

    int Shader::compileShader(GLenum type, const std::string &source) {
        int id = glCreateShader(type);
        const char *c_str = source.c_str();
        glShaderSource(id, 1, &c_str, nullptr);
        glCompileShader(id);

        GLint status;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);

        if (status != 1) {
            std::cout << "[SHADER COMPILE] "
                      << ((type == GL_VERTEX_SHADER) ? "VertexShader" : "FragmentShader")
                      << "  Status " << status << std::endl;

            GLsizei logLength = 0;
            GLchar log[1024];
            glGetShaderInfoLog(id, 1024, &logLength, log);
            std::cout << log << std::endl;
        }

        return id;
    }

    // TODO: FIX This it ain't working
    void Shader::changeVs(const std::string& src) {
        glDeleteShader(_vs);
        glDeleteProgram(_id);
        int vs = compileShader(GL_VERTEX_SHADER, src);
        create_program(vs, _fs);
    }

    // TODO: FIX This it ain't working
    void Shader::changeFs(const std::string& src) {
        glDeleteProgram(_id);
        glDeleteShader(_fs);
        int fs = compileShader(GL_FRAGMENT_SHADER, src);
        create_program(_vs, fs);
    }

    void Shader::bind() const {
        glUseProgram(_id);
    }

    void Shader::unbind() {
        glUseProgram(0);
    }

    int Shader::uniform_location(const std::string &name) {
        if (_uniforms.count(name))
            return _uniforms[name];

        GLint loc = glGetUniformLocation(_id, name.c_str());
        _uniforms[name] = loc;
        if (loc == -1)
            std::cout << "[ERROR] Uniform " << name << " not found" << std::endl;

        return loc;
    }

    void Shader::uniform1li(const std::string &name, int x) {
        GLint loc = uniform_location(name);
        if (loc != -1)
            glUniform1i(loc, x);
    }

    void Shader::uniform1lf(const std::string &name, float x) {
        GLint loc = uniform_location(name);
        if (loc != -1)
            glUniform1f(loc, x);
    }

    void Shader::uniformM4fv(const std::string &name, glm::mat4x4 mvp) {
        GLint loc = uniform_location(name);
        if (loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]);
    }

    void Shader::uniform2fv(const std::string &name, glm::vec2 v) {
        GLint loc = uniform_location(name);
        if (loc != -1)
            glUniform2fv(loc, 1, &v[0]);
    }

    void Shader::uniform3fv(const std::string &name, glm::vec3 v) {
        GLint loc = uniform_location(name);
        if (loc != -1)
            glUniform3fv(loc, 1, &v[0]);
    }

    void Shader::uniform4fv(const std::string &name, glm::vec4 v) {
        GLint loc = uniform_location(name);
        if (loc != -1)
            glUniform4fv(loc, 1, &v[0]);
    }

    void Shader::uniform1iv(const std::string &name, GLsizei count, const GLint *value) {
        GLint loc = uniform_location(name);
        if (loc != -1)
            glUniform1iv(loc, count, value);
    }
}
