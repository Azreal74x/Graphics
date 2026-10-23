#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include "DebugHud.hpp"
#include "../config.hpp"

DebugHud::DebugHud(sf::RenderWindow* aWindow)
    : _window(aWindow), _debugInfo(), _font(), _debugText(nullptr)
{
    assert(_window != nullptr);

    if (!_font.openFromFile(config::MGE_FONT_PATH + "arial.ttf")) {
        std::cerr << "Could not load font, exiting..." << std::endl;
        return;
    }

    _createDebugHud();
}

DebugHud::~DebugHud() = default;

void DebugHud::_createDebugHud() {
    // SFML 3: sf::Text must be constructed with a Font and character size
    _debugText = std::make_unique<sf::Text>(_font, "", 16);
    _debugText->setFillColor(sf::Color::White);
    _debugText->setPosition(sf::Vector2f{10.f, 10.f});
}

void DebugHud::setDebugInfo(const std::string& pInfo) {
    _debugInfo = pInfo;
    if (_debugText) {
        _debugText->setString(_debugInfo);
    }
}

void DebugHud::draw()
{
    if (!_debugText) return;

    // You are responsible for OpenGL state now in SFML 3
    glActiveTexture(GL_TEXTURE0);

    // Reset state so SFML can draw its text safely
    _window->resetGLStates();
    _window->draw(*_debugText);
}
