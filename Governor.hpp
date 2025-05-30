//arielsaadon1446@gmail.com

#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "Player.hpp"

class Governor: public Player{

public:
    Governor(const std::string &name, int coins = 0);
    ~Governor();

    bool canBlock(const std::string &action)const override;
    int getTaxAmount()const override;
};
#endif
