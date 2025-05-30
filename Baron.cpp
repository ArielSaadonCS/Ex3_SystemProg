//arielsaadon1446@gmail.com

#include "Baron.hpp"

Baron:: Baron(const std::string &name, int coins):Player(name,coins,"Baron",true,true,false,true){}
Baron:: ~Baron(){}

bool Baron::canInvest()const{
    return getAmount() >= 3; 
}

void Baron::invest(Game &game){
    if(!canInvest()){
        throw std::runtime_error("not enough coins for investing");
    }
    incrementBy(3);
}

