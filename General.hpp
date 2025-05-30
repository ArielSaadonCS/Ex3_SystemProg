//arielsaadon1446@gmail.com

#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "Player.hpp"
#include "Game.hpp"

class General:public Player{

public:
    General(const std::string &name, int coins = 0);
    ~General();

    void onArrested(Player &attacker, Game &game)override;
    bool canBlock(const std::string &action)const override;
    void blockCoup(Game &game);
};
#endif
