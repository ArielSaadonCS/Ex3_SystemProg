//arielsaadon1446@gmail.com

#include "Governor.hpp"
#include <iostream>
#include <stdexcept>
#include "Game.hpp"
#include "Player.hpp"

Governor:: Governor(const std::string &name, int coins):Player(name,coins,"Governor",true,true,false,true){}
Governor:: ~Governor(){}

// void Governor::onTaxAction(){} //handle in game

bool Governor::canBlock(const std::string &action)const{
    return action == "tax";
}

int Governor::getTaxAmount()const{
    return 3;
}

