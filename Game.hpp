//arielsaadon1446@gmail.com

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "Player.hpp"
#include "General.hpp"

class Game
{
private:
    std::vector<Player*> players;
    int turnIndex;
    bool gameOver = false;
    std::map<std::string,std::string> lastTargeted;//maps players name to last player who targeted them
    std::set<std::string> sanctionedPlayers; //set of players unders sections
    std::set<std::string> bribedPlayers;

public:

    Game(int turnIndex);
    ~Game();

    //player management
    void addPlayer(Player *Player);
    std::vector<Player*> getPlayers()const;
    Player* getCurrentPlayer()const;
    Player* getPlayerByName(const std::string &name)const;
    bool isPlayerActiveByName(const std::string &name)const;

    std::string turn()const;//return name of current player
    std::vector<std::string> getPlayersNames()const;//return names of active players
    std::string winner() const;//return winner if game over

    //game actions
    void gather(Player &p);
    void arrest(Player &p, Player &other);
    void tax(Player &p);
    void bribe(Player &p);
    void sanction(Player &p,Player &other);
    void coup(Player &p, Player &other);

    //game flow
    void nextTurn();
    bool isGameOver() const;
    void checkForCoup(Player &p);

    //actions helpers
    bool canPerformAction(Player &p, const std::string &action, Player *target = nullptr);
    bool isUnderSanction(const std::string &name)const;
    void addSanction(const std::string &name);
    void clearSanction(const std::string &name);
    void updateLastTargeted(const std::string &attackerName, const std::string target);
    std::string getLastTargeted(const std::string &name)const;

    //role specific
    void performInvestment(Player &p);//baron
    void viewPlayerCoins(Player &viewer , Player &target);//spy
    void cancelBribe(Player &judge, Player &target);//judge
    void blockCoup(Player &general); //general
    void blockArrest(Player &spy, Player &target);
    void blockTax(Player &governor, Player &target);//governor
    bool mustCoup(Player *p)const;
};

#endif