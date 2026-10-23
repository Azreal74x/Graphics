#include <iostream>
#include <variant>      // for std::holds_alternative / std::get_if

#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "AbstractGame.hpp"
#include "Renderer.hpp"
#include "World.hpp"

AbstractGame::AbstractGame():_window(NULL),_renderer(NULL),_world(NULL), _fps(0)
{
    //ctor
}

AbstractGame::~AbstractGame()
{
    //dtor
    delete _window;
    delete _renderer;
    delete _world;
}

void AbstractGame::initialize() {
    std::cout << "Initializing engine..." << std::endl << std::endl;
    _initializeWindow();
    _printVersionInfo();
    _initializeGlew();
    _initializeRenderer();
    _initializeWorld();
    _initializeScene();
    std::cout << std::endl << "Engine initialized." << std::endl << std::endl;
}

///SETUP

void AbstractGame::_initializeWindow() {
    std::cout << "Initializing window..." << std::endl;

    // SFML 3: Build VideoMode from Vector2u; ContextSettings via fields
    sf::VideoMode mode{sf::Vector2u{800u, 600u}, 32};

    sf::ContextSettings settings{};
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = 0;
    // settings.sRgbCapable = true; // example if needed
    // settings.majorVersion / minorVersion are negotiated automatically

    _window = new sf::RenderWindow(
        mode,
        "My Game!",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );

    //_window->setVerticalSyncEnabled(true);
    std::cout << "Window initialized." << std::endl << std::endl;
}

void AbstractGame::_printVersionInfo() {
    std::cout << "Context info:" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version (string) : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);

    std::cout << "----------------------------------" << std::endl << std::endl;
}

void AbstractGame::_initializeGlew() {
    std::cout << "Initializing GLEW..." << std::endl;
    // Initialize the OpenGL extension wrangler
    GLint glewStatus = glewInit();
    std::cout << "Initialized GLEW, status (1 == OK, 0 == FAILED):" << (glewStatus == GLEW_OK) << std::endl << std::endl;
}

void AbstractGame::_initializeRenderer() {
    //setup our own renderer
    std::cout << "Initializing renderer..." << std::endl;
    _renderer = new Renderer();
    _renderer->setClearColor(0,0,0);
    std::cout << "Renderer done." << std::endl << std::endl;
}

void AbstractGame::_initializeWorld() {
    //setup the world
    std::cout << "Initializing world..." << std::endl;
    _world = new World();
    std::cout << "World initialized." << std::endl << std::endl;
}

///MAIN GAME LOOP

void AbstractGame::run()
{
    //setting to calculate fps
    sf::Clock renderClock;
    int frameCount = 0;
    float timeSinceLastFPSCalculation = 0;

    //settings to make sure the update loop runs at 60 fps
    sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    sf::Clock updateClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (_window->isOpen()) {
        timeSinceLastUpdate += updateClock.restart();

        if (timeSinceLastUpdate > timePerFrame)
        {
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

            while (timeSinceLastUpdate > timePerFrame) {
                timeSinceLastUpdate -= timePerFrame;
                _update(timePerFrame.asSeconds());
            }

            _render();
            _window->display();

            //fps count is updated once every 1 second
            frameCount++;
            timeSinceLastFPSCalculation += renderClock.restart().asSeconds();
            if (timeSinceLastFPSCalculation > 1) {
                _fps = frameCount/timeSinceLastFPSCalculation;
                timeSinceLastFPSCalculation -= 1;
                frameCount = 0;
            }
        }

        //empty the event queue
        _processEvents();
    }
}

void AbstractGame::_update(float pStep) {
    _world->update(pStep);
}

void AbstractGame::_render () {
    _renderer->render(_world);
}

void AbstractGame::_processEvents()
{
    bool exit = false;

    // SFML 3.0.2: pollEvent() returns std::optional<sf::Event>
    while (const std::optional<sf::Event> event = _window->pollEvent())
    {
        // Window closed
        if (event->is<sf::Event::Closed>())
        {
            exit = true;
        }

        // Key pressed
        else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Escape)
                exit = true;
        }

        // Window resized
        else if (const auto* resize = event->getIf<sf::Event::Resized>())
        {
            glViewport(
                0, 0,
                static_cast<GLsizei>(resize->size.x),
                static_cast<GLsizei>(resize->size.y)
            );
        }
    }

    if (exit)
        _window->close();
}