#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

class DebugHud {
public:
	explicit DebugHud(sf::RenderWindow* aWindow);
	~DebugHud();

	void setDebugInfo(const std::string& pInfo);
	void draw();

private:
	void _createDebugHud();

	sf::RenderWindow* _window;
	std::string _debugInfo;
	sf::Font _font;
	std::unique_ptr<sf::Text> _debugText; // SFML 3: construct with font
};
