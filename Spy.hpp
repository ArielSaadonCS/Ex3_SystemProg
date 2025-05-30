//arielsaadon1446@gmail.com

#ifndef SPY_HPP
#define SPY_HPP

#include "Player.hpp"

class Spy: public Player{
public:
    Spy(const std::string &name, int coins = 0);
    ~Spy();

    bool canBlock(const std::string &action)const override;
    void viewPlayerCoins(Player &target)const override;
};
#endif