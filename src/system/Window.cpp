/*!
 * \file src/Window.cpp
 * \brief windowing implementation
 *
 * \author xythobuz
 */

#include "global.h"
#include "Log.h"
#include "TextureManager.h"
#include "utils/strings.h"
#include "system/Window.h"

unsigned int Window::getWidth() {
    return mWidth;
}

unsigned int Window::getHeight() {
    return mHeight;
}

bool Window::getFullscreen() {
    return mFullscreen;
}

bool Window::getMousegrab() {
    return mMousegrab;
}

bool Window::getTextInput() {
    return mTextInput;
}

// ----------------------------------------------------------------------------

Shader Window::textShader;
Shader Window::imguiShader;
unsigned int Window::vertexArrayID = 0;

int Window::initializeGL() {
    getLog() << "GL Ven.: " << glGetString(GL_VENDOR) << Log::endl;
    getLog() << "GL Ren.: " << glGetString(GL_RENDERER) << Log::endl;
    getLog() << "GL Ver.: " << glGetString(GL_VERSION) << Log::endl;
    getLog() << "GLSL V.: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << Log::endl;

    glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

    // Set background to black
    glClearColor(BLACK[0] / 256.0f, BLACK[1] / 256.0f, BLACK[2] / 256.0f, BLACK[3] / 256.0f);

    // Set up Z buffer
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if closer to camera
    glDepthFunc(GL_LESS);

    // Set up culling
    //glEnable(GL_CULL_FACE); //! \todo Transparency?

    if (textShader.compile(textShaderVertex, textShaderFragment) < 0)
        return -1;
    if (textShader.addUniform("screen") < 0)
        return -2;
    if (textShader.addUniform("textureSampler") < 0)
        return -3;
    if (textShader.addUniform("colorVar") < 0)
        return -4;
    textShader.addBuffer(2);

    if (imguiShader.compile(imguiShaderVertex, imguiShaderFragment) < 0)
        return -5;
    if (imguiShader.addUniform("screen") < 0)
        return -6;
    if (imguiShader.addUniform("textureSampler") < 0)
        return -7;
    imguiShader.addBuffer(3);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void Window::shutdownGL() {
    glDeleteVertexArrays(1, &vertexArrayID);
}

void Window::resizeGL() {
    // new matrix?
}

void Window::drawTextGL(std::vector<glm::vec2>& vertices, std::vector<glm::vec2>& uvs,
                        glm::vec4 color, unsigned int texture) {
    assert(vertices.size() == uvs.size());
    assert((vertices.size() % 3) == 0);

    glBindBuffer(GL_ARRAY_BUFFER, textShader.getBuffer(0));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, textShader.getBuffer(1));
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    textShader.use();

    glUniform2f(textShader.getUniform(0), getWindow().getWidth(), getWindow().getHeight());

    getTextureManager().bindTextureId(texture, TextureManager::TextureStorage::SYSTEM, 0);
    glUniform1i(textShader.getUniform(1), 0);

    glUniform4fv(textShader.getUniform(2), 1, &color.r);

    glEnableVertexAttribArray(0); // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, textShader.getBuffer(0));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1); // UVs
    glBindBuffer(GL_ARRAY_BUFFER, textShader.getBuffer(1));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glEnable(GL_DEPTH_TEST);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

// ----------------------------------------------------------------------------

Shader::~Shader() {
    if (programID >= 0)
        glDeleteProgram(programID);

    if (!buffers.empty())
        glDeleteBuffers(buffers.size(), &buffers[0]);
}

int Shader::addUniform(const char* name) {
    assert(programID >= 0);
    int r = glGetUniformLocation(programID, name);
    if (r < 0) {
        getLog() << "Can't find GLSL Uniform \"" << name << "\"!" << Log::endl;
        return -1;
    }
    uniforms.push_back(r);
    return uniforms.size() - 1;
}

unsigned int Shader::getUniform(int n) {
    assert(n >= 0);
    assert(n < uniforms.size());
    return uniforms.at(n);
}

void Shader::addBuffer(int n) {
    int s = buffers.size();
    for (int i = 0; i < n; i++)
        buffers.push_back(0);
    glGenBuffers(n, &buffers[s]);
}

unsigned int Shader::getBuffer(int n) {
    assert(n >= 0);
    assert(n < buffers.size());
    return buffers.at(n);
}

void Shader::use() {
    assert(programID >= 0);
    glUseProgram(programID);
}

int Shader::compile(const char* vertex, const char* fragment) {
    assert(vertex != nullptr);
    assert(fragment != nullptr);

    GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint result = GL_FALSE;
    GLint logLength = 0;

    // Compile vertex shader
    glShaderSource(vertexID, 1, &vertex, nullptr);
    glCompileShader(vertexID);

    // Check vertex shader
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::vector<char> message(logLength + 1);
        glGetShaderInfoLog(vertexID, logLength, nullptr, &message[0]);
        if (result != GL_TRUE)
            getLog() << "Vertex Shader compilation error:" << Log::endl;
        getLog() << &message[0] << Log::endl;
        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);
        return -1;
    }

    // Compile fragment shader
    glShaderSource(fragmentID, 1, &fragment, nullptr);
    glCompileShader(fragmentID);

    // Check fragment shader
    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::vector<char> message(logLength + 1);
        glGetShaderInfoLog(fragmentID, logLength, nullptr, &message[0]);
        if (result != GL_TRUE)
            getLog() << "Fragment Shader compilation error:" << Log::endl;
        getLog() << &message[0] << Log::endl;
        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);
        return -2;
    }

    // Link both shaders
    programID = glCreateProgram();
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
    glLinkProgram(programID);

    // Check resulting program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::vector<char> message(logLength + 1);
        glGetProgramInfoLog(programID, logLength, nullptr, &message[0]);
        if (result != GL_TRUE)
            getLog() << "Shader link error:" << Log::endl;
        getLog() << &message[0] << Log::endl;
        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);
        glDeleteProgram(programID);
        return -3;
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
    return programID;
}

// ----------------------------------------------------------------------------
// *INDENT-OFF*

const char* Window::textShaderVertex = R"!?!(
#version 330 core

layout(location = 0) in vec2 vertexPosition_screen;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

uniform vec2 screen;

void main() {
    vec2 halfScreen = screen / 2;
    vec2 vertexPosition_homogenous = (vertexPosition_screen - halfScreen) / halfScreen;

    gl_Position = vec4(vertexPosition_homogenous.x, -vertexPosition_homogenous.y, 0, 1);
    UV = vertexUV;
}
)!?!";

const char* Window::textShaderFragment = R"!?!(
#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D textureSampler;
uniform vec4 colorVar;

void main() {
    color = texture(textureSampler, UV) * colorVar;
}
)!?!";

// --------------------------------------

const char* Window::imguiShaderVertex = R"!?!(
#version 330 core

layout(location = 0) in vec2 vertexPosition_screen;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexColor;

out vec2 UV;
out vec4 FragColor;

uniform vec2 screen;

void main() {
    vec2 halfScreen = screen / 2;
    vec2 vertexPosition_homogenous = (vertexPosition_screen - halfScreen) / halfScreen;

    gl_Position = vec4(vertexPosition_homogenous.x, -vertexPosition_homogenous.y, 0, 1);
    UV = vertexUV;
    FragColor = vertexColor;
}
)!?!";

const char* Window::imguiShaderFragment = R"!?!(
#version 330 core

in vec2 UV;
in vec4 FragColor;

out vec4 color;

uniform sampler2D textureSampler;

void main() {
    color = texture(textureSampler, UV) * FragColor;
}
)!?!";

// *INDENT-ON*

