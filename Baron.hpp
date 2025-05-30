//arielsaadon1446@gmail.com

#ifndef BARON_HPP
#define BARON_HPP

#include "Player.hpp"
#include "Game.hpp"

class Baron:public Player{

public:
    Baron(const std::string &name, int coins = 0);
    ~Baron();

    bool canInvest()const override;
    void invest(Game &game)override;
};
#endif
