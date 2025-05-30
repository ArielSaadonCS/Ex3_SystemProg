//arielsaadon1446@gmail.com

#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "Player.hpp"
#include "Game.hpp"

class Judge: public Player{

public:
    Judge(const std::string &name, int coins = 0);
    ~Judge();

    //void onSanctioned(Player &attacker, Game &game)override;
    bool canCancelBribe(Player &target)const override;
    void cancelBribe(Player &target, Game &game)override;
};
#endif
