//arielsaadon1446@gmail.com

#include "Judge.hpp"

Judge::Judge(const std::string &name, int coins):Player(name,coins,"Judge",true,true,false,true){}

Judge::~Judge(){}

bool Judge::canCancelBribe(Player &target)const{
    return target.getName() != this->getName(); //can cancel anyone bribe
}

void Judge::cancelBribe(Player &target, Game &game){
    if(!canCancelBribe(target)){
        throw std::runtime_error("cannot cancel this bribe");
    }
    //briber lose 4 coins and dont get extra turn, handled in Game
}