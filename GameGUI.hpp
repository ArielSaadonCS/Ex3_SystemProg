//arielsaadon1446@gmail.com

#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Player.hpp"
#include <vector>
#include <string>

struct ActionButton {
    sf::RectangleShape rect;
    sf::Text label;
    std::string actionName;
    bool requiresTarget;
};

class GameGUI {
public:
    GameGUI(Game* g);
    void run();

private:
    sf::RenderWindow window;
    sf::Font font;
    Game* game;

    std::vector<ActionButton> actionButtons;

    std::vector<std::pair<sf::FloatRect, Player*>> playerRects;

    std::vector<std::string> actionLog;

    bool selectingTarget = false;
    std::string selectedAction;

    void setupUI();
    void setupActionButtons(Player* current);
    void drawPlayers();
    void drawTurnInfo();
    void drawActionButtons();
    bool isClicked(const sf::RectangleShape& rect, const sf::Vector2f& mousePos);
    void logAction(const std::string& msg);
    void drawActionLog();
};

#endif //GAME_GUI_HPP
