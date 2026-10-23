#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ShaderUtil.h"

/**
 * Basic demo of rendering a triangle in OpenGL through the new programmable pipeline.
 * In a real life example, this code should include error checking and refactor into classes/functions.
 * In addition VAO's are missing.
 */

int main() {
    //Open SFML Window == Valid OpenGL Context
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    //settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::Window window(
        sf::VideoMode({800, 600}), //800 x 600 x 32 bits pp
        "OpenGL Window", //title
        sf::Style::Default,
        sf::State::Windowed, //Default, Fullscreen, Titlebar, etc
        settings
    );

    window.setVerticalSyncEnabled(true);

    //initialize glew to load all available opengl functions/extensions
    GLenum glewResult = glewInit();
    if (glewResult != GLEW_OK) {
        std::cout << "Could not initialize GLEW, byeeee!" << std::endl;
        return -1;
    }

    GLuint programID = ShaderUtil::createProgram("./../vertexshader.vs", "./../fragmentshader.fs");

    //declare the data to upload
    const GLfloat vertices[] = {
        //1 triangles, 3 vertices per triangle, 4 floats per vertex
        -0.5f, -0.5f, 0, //bottom left
        0.5f, -0.5f, 0, //bottom right
        -0.5f, 0.5f, 0, //top left

        0.5f, 0.5f, 0, //top right
        0.5f, -0.5f, 0, //bottom right
        -0.5f, 0.5f, 0, //top left

        //second plane positions
        -0.5f, -0.5f, 0, //bottom left
        0.5f, -0.5f, 0, //bottom right
        -0.5f, 0.5f, 0, //top left

        0.5f, 0.5f, 0, //top right
        0.5f, -0.5f, 0, //bottom right
        -0.5f, 0.5f, 0, //top left
    };
    // why colors? why not UVs?
    const GLfloat colors[]{
        //1 triangle, 3 vertices per triangle, 1 color per vertex, 4 "floats" per color RGBA
        1, 0, 0, //bottom left
        0, 1, 0, //bottom right
        1, 0, 1, //top left

        0, 0, 1, //top right
        0, 1, 0, //bottom right
        1, 0, 1, //top left

        //second plane colors
        1, 0, 0, //bottom left
        0, 1, 0, //bottom right
        1, 0, 1, //top left

        0, 0, 1, //top right
        0, 1, 0, //bottom right
        1, 0, 1 //top left

    };

    // Create VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //create a handle to the buffer
    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    //bind our buffer to the GL_ARRAY_BUFFER endpoint, since none was bound yet,
    //a new array buffer for vertex position data will be created
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    //stream all our data to the array buffer endpoint to which our vertexPositionsBufferId is connected
    //note that vertexPositionsBufferId is not mentioned, instead the ARRAY_BUFFER is set as the data "sink"
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //disconnect the funnel
    //glBindBuffer(GL_ARRAY_BUFFER, 0);


    //create a handle to the buffer
    GLuint colorBufferId;
    glGenBuffers(1, &colorBufferId);
    //bind our buffer to the GL_ARRAY_BUFFER endpoint, since none was bound yet,
    //a new array buffer for vertex color data will be created
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
    //stream all our data to the array buffer endpoint to which our vertexColorsBufferId is connected
    //note that vertexColorsBufferId is not mentioned, instead the ARRAY_BUFFER is set as the data "sink"
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    //disconnect the funnel
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLint vertexIndex = glGetAttribLocation(programID, "vertex");
    GLint colorIndex = glGetAttribLocation(programID, "color");

    //make sure data is streamed to the shader for this index
    glEnableVertexAttribArray(vertexIndex);
    //bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    //tell OpenGL in which format to expect this data
    glVertexAttribPointer(vertexIndex, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    //make sure data is streamed to the shader for this index
    glEnableVertexAttribArray(colorIndex);
    //bind the color buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
    //tell OpenGL in which format to expect this data
    glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    //disconnect VAO
    glBindVertexArray(0);

    glClearColor(0, 0, 0, 1);

    while (window.isOpen()) {
        glClear(GL_COLOR_BUFFER_BIT);

        //tell the GPU to use this program
        glUseProgram(programID);

        GLint offsetIndex = glGetUniformLocation(programID, "offset");
        float xOffset = 0.5f * cos(clock() / 1000.0f);
        float yOffset = 0.5f * sin(clock() / 1000.0f);
        //float xOffset = 0.0f;
        //float yOffset = 0.0f;
        glUniform2f(offsetIndex, xOffset, yOffset);

        //make columns uniform
        GLint columnsUniform = glGetUniformLocation(programID, "columns");
        glUniform1f(columnsUniform, 10.0f);

        //make rows uniform
        GLint rowsUniform = glGetUniformLocation(programID, "rows");
        glUniform1f(rowsUniform, 20.0f);

        GLint checkerTypeUniform = glGetUniformLocation(programID, "checkerType");
        glUniform1i(checkerTypeUniform, 0.0f);

        //static float to keep increasing the angle
        static float angle = 0.0f;
        static float rotationSpeed = 0.01f;
        angle += rotationSpeed;;

        //create the uniform to use it in the vertex shader
        GLint rotationUniform = glGetUniformLocation(programID, "rotation");
        glUniform1f(rotationUniform, angle);

        //make columns uniform
        GLint mousePosUniform = glGetUniformLocation(programID, "mousePos");
        //(window) gets pixel coords, so raw screen space (need to be normalized for OpenGL)
        sf::Vector2 mousePos = sf::Mouse::getPosition(window);
        glUniform2f(mousePosUniform, mousePos.x, mousePos.y);

        GLint screenSizeUniform = glGetUniformLocation(programID, "screenSize");
        sf::Vector2 screenSize = window.getSize();
        glUniform2f(screenSizeUniform, screenSize.x, screenSize.y);

        // Bind the VAO and draw the triangle
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //set info and draw second checkerboard
        glUniform2f(offsetIndex, -xOffset, -yOffset);
        glUniform1i(checkerTypeUniform, 1.0f);
        glDrawArrays(GL_TRIANGLES, 6, 6);

        glBindVertexArray(0);
        glDisableVertexAttribArray(vertexIndex);
        glDisableVertexAttribArray(colorIndex);

        //display it
        window.display();

        //empty the event queue
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            // Window closed
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Window resized
            else if (const auto *resized = event->getIf<sf::Event::Resized>()) {
                glViewport(
                    0, 0,
                    static_cast<GLsizei>(resized->size.x),
                    static_cast<GLsizei>(resized->size.y)
                );
            }

            // Key pressed
            else if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Up) {
                    rotationSpeed += 0.001f;
                } else if (key->code == sf::Keyboard::Key::Down) {
                    rotationSpeed -= 0.001f;
                    if (rotationSpeed < 0.001f)
                        rotationSpeed = 0.001f;
                }
            }
        }
    }

    return 0;
}
