//arielsaadon1446@gmail.com

#include "Spy.hpp"
#include "Player.hpp"

Spy:: Spy(const std::string &name, int coins):Player(name,coins,"Spy",true,true,false,true){}
Spy:: ~Spy(){}

bool Spy::canBlock(const std::string &action)const{
    return action == "arrest";
}

void Spy::viewPlayerCoins(Player &target)const{
    //neet to implement somehow
}