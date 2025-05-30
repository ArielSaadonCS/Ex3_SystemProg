//arielsaadon1446@gmail.com

#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "Player.hpp"
#include "Game.hpp"

class Merchant: public Player{

public:
    Merchant(const std::string &name, int coins = 0);
    ~Merchant();

    void onArrested(Player &attacker, Game &game)override;
    void startTurnEffect(Game &game)override;
};
#endif
