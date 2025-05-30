//arielsaadon1446@gmail.com

#include "Player.hpp"
#include <iostream>
#include <stdexcept>

Player::Player(std::string name,int coins,std::string role,bool active,bool canArrestNextTurn,bool hasExtraTurn, bool canTax)
:name(name),coins(coins),role(role),active(active),canArrestNextTurn(canArrestNextTurn),hasExtraTurn(hasExtraTurn),canTax(canTax){}
Player::~Player(){}

std::string Player::getName()const{
    return name;
}
std::string Player::getRole()const{
    return role;
}

int Player::getAmount()const{
    return coins;
}
bool Player::isActive()const{
    return active;
}
void Player::setActive(bool status){
    active =status;
}
bool Player::getCanArrestStatus()const{
    return canArrestNextTurn;
}
void Player::setCanArrestStatus(bool status){
    canArrestNextTurn = status;
}
bool Player::getExtraTurn()const{
    return hasExtraTurn;
}
void Player::setExtraTurn(bool status){
    hasExtraTurn = status;
}
bool Player::getcanTax()const{
    return canTax;
}
void Player::setCanTax(bool status){
    canTax = status;
}
void Player::incrementBy( int n){
    if(n <= 0){
        throw std::invalid_argument("n must be positive");
    }
    coins += n;
}
void Player::decrementBy( int n){
    if(n <= 0){
        throw std::invalid_argument("n must be positive");
    }
    if(coins < n){
        throw std::invalid_argument("not enough coins");
    }
    coins -= n;
}

int Player::getTaxAmount()const{
    return 2;
}

bool Player::canBlock(const std::string &action)const{
    return false;//base player cant block
}

void Player::onArrested(Player &attacker,Game &game){
    if(getAmount() >= 1){
        decrementBy(1);
        attacker.incrementBy(1);
    }
}
bool Player::onCouped(Player &attacker,Game &game){
    return true; //coup successed
}
void Player::startTurnEffect(Game &game){}
void Player::viewPlayerCoins(Player &target)const{
     throw std::runtime_error("this player cannot view other palyers coins");
}
bool Player::canInvest()const{
    return false;
}
void Player::invest(Game &game){
    throw std::runtime_error("this player cannot invest");

}
bool Player::canCancelBribe(Player &target)const{return false;}
void Player::cancelBribe(Player &target,Game &game){
    throw std::runtime_error("this player cannot cancel bribe");
}
void Player::onTaxAction(Game &game){}
