//arielsaadon1446@gmail.com

#include "GameGUI.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <stdexcept> 
#include "Game.hpp"
#include "Player.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Governor.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Spy.hpp"

void createPlayerForGUI(Game* game, const std::string& name, int initialCoins, std::mt19937& gen) {
    if (!game) return;

    std::vector<std::string> roles = {"Baron", "General", "Governor", "Judge", "Merchant", "Spy"};
    std::uniform_int_distribution<> distrib(0, roles.size() - 1);
    std::string role = roles[distrib(gen)];
    Player* p = nullptr;

    if (role == "Baron") p = new Baron(name, initialCoins);
    else if (role == "General") p = new General(name, initialCoins);
    else if (role == "Governor") p = new Governor(name, initialCoins);
    else if (role == "Judge") p = new Judge(name, initialCoins);
    else if (role == "Merchant") p = new Merchant(name, initialCoins);
    else if (role == "Spy") p = new Spy(name, initialCoins);

    if (p) {
        try {
            game->addPlayer(p); 
        } catch (const std::exception& e) {
            std::cerr << "error" << name << " to GUI: " << e.what() << std::endl;
            delete p; 
        }
    }
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    Game* gameInstance = new Game(0); 

    int numPlayers = 5;

    for (int i = 1; i <= numPlayers; ++i) {
        createPlayerForGUI(gameInstance, "player" + std::to_string(i), 2, gen);
    }

    if (gameInstance->getPlayers().size() < 2) {
        delete gameInstance; 
        return 1; 
    }

    GameGUI gui(gameInstance); 
    gui.run(); 

    // 4. ניקוי זיכרון בסיום
    // חשוב מאוד: אם Game מקצה זיכרון דינמי לשחקנים (Player*),
    // הדסטרקטור של Game (~Game()) צריך לדאוג לשחרר את הזיכרון הזה.
    // אם הדסטרקטור של Game לא עושה זאת, תהיה דליפת זיכרון של השחקנים.
    std::cout << "game over" << std::endl;
    delete gameInstance; // ישחרר את אובייקט המשחק. (וגם את השחקנים אם ~Game מטפל בזה)

    return 0;
}

GameGUI::GameGUI(Game* g)
    : window(sf::VideoMode(800, 600), "Power Struggle GUI"), game(g) {
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font!" << std::endl;
        exit(1);
    }
}


void GameGUI::logAction(const std::string& msg) {
    if (actionLog.size() > 8) actionLog.erase(actionLog.begin());
    actionLog.push_back(msg);
}

void GameGUI::drawActionLog() {
    float y = 100;
    sf::Text title("Action Log:", font, 18);
    title.setFillColor(sf::Color::White);
    title.setPosition(600, y);
    window.draw(title);
    y += 25;

    for (const auto& line : actionLog) {
        sf::Text txt(line, font, 14);
        txt.setFillColor(sf::Color::White);
        txt.setPosition(600, y);
        window.draw(txt);
        y += 20;
    }
}

void GameGUI::setupActionButtons(Player* current) {
    actionButtons.clear();
    
    float x = 50;
    float y = 500;
    int buttonsPerRow = 5;
    int count = 0;

    auto makeButton = [&](const std::string& name, bool needsTarget, sf::Color color) {
        if (count == buttonsPerRow) {
            x = 50;
            y += 60;
            count = 0;
        }

        ActionButton btn;
        btn.rect.setSize(sf::Vector2f(140, 40));
        btn.rect.setPosition(x, y);
        btn.rect.setFillColor(color);

        btn.label.setFont(font);
        btn.label.setCharacterSize(16);
        btn.label.setFillColor(sf::Color::White);
        btn.label.setString(name);
        btn.label.setPosition(x + 10, y + 8);

        btn.actionName = name;
        btn.requiresTarget = needsTarget;

        actionButtons.push_back(btn);
        x += 160;
        count++;
    };

    if (game->mustCoup(current)) {
    makeButton("coup", true, sf::Color(255, 0, 0)); 
    return; 
    }

    bool isUnderSanction = game->isUnderSanction(current->getName());

    if (!isUnderSanction && game->canPerformAction(*current, "gather")) {
        makeButton("gather", false, sf::Color(70, 130, 180));
    }
    if (!isUnderSanction && game->canPerformAction(*current, "tax")) {
        makeButton("tax", false, sf::Color(34, 139, 34));
    }

    if (game->canPerformAction(*current, "bribe")) makeButton("bribe", false, sf::Color(255, 165, 0));
    if (game->canPerformAction(*current, "coup")) makeButton("coup", true, sf::Color(178, 34, 34));
    if (game->canPerformAction(*current, "arrest")) makeButton("arrest", true, sf::Color(100, 149, 237));
    if (game->canPerformAction(*current, "sanction")) makeButton("sanction", true, sf::Color(128, 0, 128));

    if (current->getRole() == "Baron" && current->canInvest()) {
        makeButton("invest", false, sf::Color(139, 69, 19));
    }
    if (current->getRole() == "Spy") {
        makeButton("viewcoins", true, sf::Color::Cyan);
        makeButton("blockarrest", true, sf::Color(200, 200, 50));
    }
    if (current->getRole() == "Judge") {
        makeButton("cancelbribe", true, sf::Color(200, 100, 200));
    }
    if (current->getRole() == "General" && current->canBlock("coup")) {
        makeButton("blockcoup", false, sf::Color(255, 100, 100));
    }
    if (current->getRole() == "Governor") {
        makeButton("blocktax", true, sf::Color(255, 100, 255));
    }

    makeButton("endturn", false, sf::Color(105, 105, 105));
}

void GameGUI::drawPlayers() {
    playerRects.clear();
    int y = 50;

    for (Player* p : game->getPlayers()) {
        sf::Text info;
        info.setFont(font);
        info.setCharacterSize(18);
        info.setFillColor(p->isActive() ? sf::Color::Green : sf::Color::Red);
        info.setString(p->getName() + " (" + p->getRole() + ") - Coins: " + std::to_string(p->getAmount()));
        info.setPosition(50, y);

        window.draw(info);
        playerRects.push_back({ info.getGlobalBounds(), p });

        y += 30;
    }

    if (selectingTarget) {
        sf::Text selectMsg;
        selectMsg.setFont(font);
        selectMsg.setCharacterSize(20);
        selectMsg.setFillColor(sf::Color::Yellow);
        selectMsg.setString("Select a target for: " + selectedAction);
        selectMsg.setPosition(50, y + 10);
        window.draw(selectMsg);
    }
}

void GameGUI::drawTurnInfo() {
    sf::Text turnText;
    turnText.setFont(font);
    turnText.setCharacterSize(24);
    turnText.setFillColor(sf::Color::White);
    turnText.setString("Current Turn: " + game->turn());
    turnText.setPosition(400, 20);
    window.draw(turnText);
}

void GameGUI::drawActionButtons() {
    for (const auto& btn : actionButtons) {
        window.draw(btn.rect);
        window.draw(btn.label);
    }
}

bool GameGUI::isClicked(const sf::RectangleShape& rect, const sf::Vector2f& mousePos) {
    return rect.getGlobalBounds().contains(mousePos);
}

void GameGUI::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                Player* current = nullptr;
                try {
                    current = game->getCurrentPlayer();
                } catch (...) {
                    continue;
                }

                if (!selectingTarget) {
                    for (const auto& btn : actionButtons) {
                        if (isClicked(btn.rect, mousePos)) {
                            try {
                                if (btn.actionName == "endturn") {
                                    game->nextTurn();
                                    logAction(current->getName() + " ended their turn");
                                } else if (btn.requiresTarget) {
                                    selectingTarget = true;
                                    selectedAction = btn.actionName;
                                } else {
                                    if (btn.actionName == "gather") {
                                        game->gather(*current);
                                        logAction(current->getName() + " gathered 1 coin");
                                        game->nextTurn();
                                    }
                                    else if (btn.actionName == "tax") {
                                        game->tax(*current);
                                        logAction(current->getName() + " collected tax");
                                        game->nextTurn();
                                    }
                                    else if (btn.actionName == "bribe") {
                                        game->bribe(*current);
                                        logAction(current->getName() + " paid a bribe");
                                    }
                                    else if (btn.actionName == "invest") {
                                        game->performInvestment(*current);
                                        logAction(current->getName() + " invested (Baron)");
                                        game->nextTurn();
                                    }
                                    else if (btn.actionName == "blockcoup") {
                                        game->blockCoup(*current);
                                        logAction(current->getName() + " blocked a coup");
                                    }
                                }
                            } catch (const std::exception& e) {
                                std::cerr << "Action failed: " << e.what() << std::endl;
                                logAction("Action failed: " + std::string(e.what()));
                            }
                            break;
                        }
                    }
                } else {
                    for (const auto& [rect, target] : playerRects) {
                        if (rect.contains(mousePos)) {
                            try {
                                std::string action = selectedAction;

                                if (action == "coup") {
                                    game->coup(*current, *target);
                                    logAction(current->getName() + " performed a coup on " + target->getName());
                                }
                                else if (action == "arrest") {
                                    game->arrest(*current, *target);
                                    logAction(current->getName() + " arrested " + target->getName());
                                }
                                else if (action == "sanction") {
                                    game->sanction(*current, *target);
                                    logAction(current->getName() + " sanctioned " + target->getName());
                                }
                                else if (action == "viewcoins") {
                                    int coins = target->getAmount();
                                    logAction(current->getName() + " viewed " + target->getName() + "'s coins: " + std::to_string(coins));
                                }
                                else if (action == "cancelbribe") {
                                    game->cancelBribe(*current, *target);
                                    logAction(current->getName() + " canceled bribe on " + target->getName());
                                }
                                else if (action == "blockarrest") {
                                    game->blockArrest(*current, *target);
                                    logAction(current->getName() + " blocked arrest on " + target->getName());
                                }
                                else if (action == "blocktax") {
                                    game->blockTax(*current, *target);
                                    logAction(current->getName() + " blocked tax of " + target->getName());
                                }

                                selectingTarget = false;
                                selectedAction.clear();

                                if (action != "viewcoins" && action != "cancelbribe" && action != "blockarrest")
                                    game->nextTurn();

                            } catch (const std::exception& e) {
                                std::cerr << "Targeted action failed: " << e.what() << std::endl;
                                logAction("Targeted action failed: " + std::string(e.what()));
                                selectingTarget = false;
                                selectedAction.clear();
                            }
                            break;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(40, 40, 40));
        drawTurnInfo();
        drawPlayers();
        Player* current = nullptr;
        try {
            current = game->getCurrentPlayer();
            setupActionButtons(current);
        } catch (...) {}
        drawActionButtons();
        drawActionLog();

        if (game->mustCoup(current)) {
        sf::Text warning;
         warning.setFont(font);
         warning.setCharacterSize(20);
         warning.setFillColor(sf::Color::Red);
         warning.setString("You have 10 or more coins you must perform a Coup!");
         warning.setPosition(50, 460);
         window.draw(warning);
        }       
        window.display();

        if (game->isGameOver()) {
            std::cout << "Winner: " << game->winner() << std::endl;
            logAction("Winner: " + game->winner());
            window.close();
        }
    }
}
