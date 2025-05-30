//arielsaadon1446@gmail.com

#include "General.hpp"

General::General(const std::string &name, int coins):Player(name,coins,"General",true,true,false,true){}
General::~General(){}

void General::onArrested(Player &attacker, Game &game){
//no coins lost or gained
}

bool General::canBlock(const std::string &action)const{
    return action == "coup" && getAmount() >= 5;
}

void General::blockCoup(Game &game){
    if(getAmount() >= 5){
        decrementBy(5);
    }
}