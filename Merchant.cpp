//arielsaadon1446@gmail.com

#include "Merchant.hpp"

Merchant::Merchant(const std::string &name, int coins):Player(name,coins,"Merchant",true,true,false,true){}
Merchant::~Merchant(){}

void Merchant::onArrested(Player &attacker, Game &game){
    if(getAmount() >= 2){
        decrementBy(2);
        attacker.incrementBy(0);
    }
    else if(getAmount() == 1){
        decrementBy(1);
        attacker.incrementBy(0);        
    }
}

void Merchant::startTurnEffect(Game &game){//get 3 coins if he have 3+ coins
    if(getAmount() >= 3){
        incrementBy(1);
    }
}