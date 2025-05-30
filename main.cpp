//arielsaadon1446@gmail.com

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <limits> 
#include <map>
#include "Player.hpp"
#include "Game.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Governor.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Spy.hpp"
std::random_device global_rd;
std::mt19937 global_gen(global_rd()); 

void createPlayer(Game& game, const std::string& name, int initialCoins) {
    std::vector<std::string> roles = {"Baron", "General", "Governor", "Judge", "Merchant", "Spy"};
    std::uniform_int_distribution<> distrib(0, roles.size() - 1);
    std::string role = roles[distrib(global_gen)];
    Player* p = nullptr;

    if (role == "Baron") p = new Baron(name, initialCoins);
    else if (role == "General") p = new General(name, initialCoins);
    else if (role == "Governor") p = new Governor(name, initialCoins);
    else if (role == "Judge") p = new Judge(name, initialCoins);
    else if (role == "Merchant") p = new Merchant(name, initialCoins);
    else if (role == "Spy") p = new Spy(name, initialCoins);

    if (p) {
        try {
            game.addPlayer(p);
            std::cout << name << " joined as " << role << "." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "error adding player " << name << ": " << e.what() << std::endl;
            delete p;
        }
    }
}

// Function to display current status of all players
void displayPlayerInfo(Game& game) {
    std::cout << "\n--- PLAYERS STATUS---" << std::endl;
    for (Player* p : game.getPlayers()) {
        if (!p) continue;
        std::cout << p->getName() << " (" << p->getRole() << ") - coins: " << p->getAmount()
                  << (p->isActive() ? " [active]" : " [not active]")
                  << (game.isUnderSanction(p->getName()) ? " [under sanction]" : "")
                  << std::endl;
    }
    std::cout << "---------------------\n";
}

// Function to find a random target (excluding the attacker)
Player* findRandomTarget(Game& game, Player* attacker) {
    std::vector<Player*> activeTargets;
    for (Player* p : game.getPlayers()) {
        if (p && p->isActive() && p != attacker) {
            activeTargets.push_back(p);
        }
    }
    if (activeTargets.empty()) {
        return nullptr;
    }
    std::uniform_int_distribution<> distrib(0, activeTargets.size() - 1);
    return activeTargets[distrib(global_gen)];
}


int main() {
    Game game(0);
    const int NUM_PLAYERS = 3;
    const int MAX_TURNS = 15;

    std::cout << "make " << NUM_PLAYERS << " players starting with 2 coins " << std::endl;
    for (int i = 1; i <= NUM_PLAYERS; ++i) {
        createPlayer(game, "player" + std::to_string(i), 2);
    }

    std::cout << "\n starting the game\n" << std::endl;

    for (int turnNum = 1; turnNum <= MAX_TURNS && !game.isGameOver(); ++turnNum) {
        Player* currentPlayer = nullptr;
        try {
            currentPlayer = game.getCurrentPlayer();
        } catch (const std::runtime_error& e) {
            std::cerr << "cant accept current player" << e.what() << std::endl;
            break;
        }
        if (!currentPlayer) break;

        std::cout << " turn number " << turnNum << " turn of " << game.turn()
                  << " (" << currentPlayer->getRole() << ", coins: " << currentPlayer->getAmount() << ")" << std::endl;
        std::cout << "========================================" << std::endl;
        displayPlayerInfo(game);

        if (!currentPlayer->isActive()) {
            std::cout << currentPlayer->getName() << " not active, skiping" << std::endl;
            try { game.nextTurn(); } catch (const std::runtime_error& e) { std::cerr << e.what() << std::endl; break; }
            continue;
        }

        std::cout << "\n---  action of " << currentPlayer->getName() << " ---" << std::endl;
        try {
            if (game.mustCoup(currentPlayer)) {
                std::cout << currentPlayer->getName() << " must perform coup 10+ coins " << std::endl;
                Player* target = findRandomTarget(game, currentPlayer);
                if (target) {
                    game.coup(*currentPlayer, *target);
                    std::cout << currentPlayer->getName() << " blocked the coup " << target->getName() << "." << std::endl;
                
            }
        }
            else if (currentPlayer->getRole() == "Baron" && currentPlayer->canInvest()) { 
                game.performInvestment(*currentPlayer); 
                std::cout << currentPlayer->getName() << " investing " << std::endl; //
            }
            else if (currentPlayer->getRole() == "Spy" && currentPlayer->getAmount() >= 0) { 
                Player* target = findRandomTarget(game, currentPlayer);
                if (target) {
                    std::cout << currentPlayer->getName() << " view coins amount of " << target->getName()
                              << ": " << target->getAmount() << " מטבעות." << std::endl;
                }
            }
            else if (currentPlayer->getAmount() >= 7) { 
                Player* target = findRandomTarget(game, currentPlayer);
                if (target) {
                    game.coup(*currentPlayer, *target); 
                    std::cout << currentPlayer->getName() << " coup against " << target->getName() << "." << std::endl;
                }
            }
            else if (currentPlayer->getcanTax() && !game.isUnderSanction(currentPlayer->getName())) {
                game.tax(*currentPlayer);
                std::cout << currentPlayer->getName() << " tax amount (" << currentPlayer->getTaxAmount() << " coins)." << std::endl;
            }
            else {
                if (!game.isUnderSanction(currentPlayer->getName())) {
                    game.gather(*currentPlayer);
                    std::cout << currentPlayer->getName() << " gather coin." << std::endl;
                } else {
                    std::cout << currentPlayer->getName() << " under sanction, cant gather coin." << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cout << "action  of " << currentPlayer->getName() << " failed: " << e.what() << std::endl;
        
        }
        //next turn
        if (!game.isGameOver()) {
            try {
                game.nextTurn();
            } catch (const std::runtime_error& e) {
                std::cerr << "error " << e.what() << std::endl;
                break;
            }
        }
        }
    if (game.isGameOver()) {
        std::cout << "\n--- game over! ---" << std::endl;
        displayPlayerInfo(game);
        std::cout << "winner is : " << game.winner() << std::endl;
        }
     else {
        std::cout << "\n--- game ended without a winner ---" << std::endl;
        displayPlayerInfo(game);     }

    return 0;
}