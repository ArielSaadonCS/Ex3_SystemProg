//arielsaadon1446@gmail.com

#include "Player.hpp"
#include "General.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <stdexcept>


Game::Game(int turnIndex):turnIndex(turnIndex){}

Game::~Game(){
    for(Player *p: players){
        if(p != nullptr){
            delete p;
            p = nullptr;
        }
    }
    players.clear();
}

void Game::addPlayer(Player *player){
    if(player==nullptr){
        throw std::invalid_argument("can't add null players");
    }

    for(Player *p: players){
        if(p->getName() == player->getName()){
            throw std::invalid_argument("player already exist");
        }
    }
        players.push_back(player);  
}

std::vector<Player*> Game::getPlayers()const{
    return players;
}

Player* Game::getCurrentPlayer()const{
    if(players.empty()){
        throw std::runtime_error("no players in the game");
    }
    return players[turnIndex];
}

Player* Game::getPlayerByName(const std::string &name)const{
    for(Player *p:players){
        if(p->getName()==name){
        return p;
    }
    }
    throw std::invalid_argument("palyer not found");
}

bool Game::isPlayerActiveByName(const std::string &name)const{
    try{
        Player *p = getPlayerByName(name);
        return p->isActive();
    }
    catch(std::invalid_argument){
        return false;
    }

}
std::string Game::turn()const{
    if(players.empty()){
        throw std::runtime_error("no players left in the game");
    }
    return players[turnIndex]->getName();
}

std::vector<std::string> Game::getPlayersNames()const{
    std::vector<std::string> activePlayerNames;
    for(Player *p: players){
        if(p->isActive()){
            activePlayerNames.push_back(p->getName());
        }
    }
    return activePlayerNames;
}
std::string Game::winner()const{
    if(!isGameOver()){
        throw std::runtime_error("game isn't over yet");
    }
    //find last active player
    for(Player *p : players){
        if(p->isActive()){
            return p->getName();
        }
    }
    throw std::runtime_error("no active players found and game is over");
}

void Game::gather(Player &p){
    if(isUnderSanction(p.getName())){
        throw std::runtime_error("player is under sanction and cannot gather");
    }
    p.incrementBy(1);
}

void Game::arrest(Player &p, Player &other){//check if targeting same player twice

    if(!p.getCanArrestStatus()){
        throw std::runtime_error("cannot arrest, you've been blocked by a spy");
    }
    if(getLastTargeted(p.getName()) == other.getName()){
        throw std::runtime_error("cannot arrest a player twice in a row");
    }
    if(p.getName() == other.getName()){
         throw std::runtime_error("cannot arrest yoursel");
    }
    if(!canPerformAction(p,"arrest",&other)){
        throw std::runtime_error("invalid arrest");
    }
    updateLastTargeted(p.getName(), other.getName()); //update who target who

    other.onArrested(p, *this);
}

void Game::tax(Player &p){
    if(isUnderSanction(p.getName())){
        throw std::runtime_error("player is under sanction and cannot tax");
    }
    if(p.getRole() == "Governor"){
        p.incrementBy(3);
    }
    else{
    p.incrementBy(2);
    }
}
void Game::bribe(Player &p){
    if(p.getAmount() < 4){
        throw std::invalid_argument("not enuogh coins for bribe");
    }
    p.decrementBy(4);
    bribedPlayers.insert(p.getName());
    p.setExtraTurn(true);
    //get another turn
}

void Game::sanction(Player &p,Player &other){
    if(p.getAmount() < 3){
        throw std::invalid_argument("not enuogh coins for sanction\n");
    }
    p.decrementBy(3);
    addSanction(other.getName());
   // other.onSanctioned(p, *this);

   if(other.getRole()=="Judge" && p.getAmount() > 0){
    p.decrementBy(1);
   }
    if(other.getRole()=="Baron" && p.getAmount() > 0){
        other.incrementBy(1);
   }
}

void Game::coup(Player &p, Player &other){
    if(p.getAmount() < 7){
        throw std::invalid_argument("not enuogh coins for coup\n");
    }

    p.decrementBy(7);

    bool coupBlocked = false;
    for(Player *player:players){
        if(player->isActive() && player->getRole() == "General" && player->getAmount() >= 5 && player != &p){
            coupBlocked = true;
            player->decrementBy(5);
            break;
        }
    }
    if(!coupBlocked){
        bool coupSuccess = other.onCouped(p,*this);
        if(coupSuccess){
            other.setActive(false);
        }
    }
}

void Game::nextTurn(){
    Player *current = getCurrentPlayer();
    clearSanction(current->getName()); //sanction expire apter turn pass
    
    if(current->getExtraTurn()){
        current->setExtraTurn(false);
        return; //getting an extra turn for bribe
    }
    current->setCanTax(true);
    int originalIndex = turnIndex;
    current->setCanArrestStatus(true);
    do{
    turnIndex = (turnIndex+1)% players.size();
    if(turnIndex == originalIndex && !players[turnIndex]->isActive()){
        throw std::runtime_error("no active players left\n");
    }
    }while(!players[turnIndex]->isActive());
    
    players[turnIndex]->startTurnEffect(*this);

    checkForCoup(*players[turnIndex]); //check if player have 10+ coins
    
    gameOver = isGameOver();
}

bool Game::isGameOver() const{
    int activeCount = 0;
    for(Player *p: players){
        if(p->isActive()){
            activeCount++;
        }
    }
    return activeCount <= 1;
}

void Game::checkForCoup(Player &p){
    if(p.getAmount() >= 10){
        std::cout << "the player "<< p.getName() << " have 10+ coins and must make coup this turn\n";
        //the actual is in the main
    }
}

bool Game::canPerformAction(Player &p, const std::string &action, Player* target){
    if(!p.isActive()){return false;}

    if(isUnderSanction(p.getName()) && (action == "gather" || action == "tax")){
        return false;
    }
    
    if(target != nullptr && !target->isActive()){return false;}

    if(action == "bribe" && p.getAmount() < 4){return false;}
    if(action == "sanction" && p.getAmount() < 3){return false;}
    if(action == "coup" ){return p.getAmount() >= 7;}
    if(action == "arrest" && target != nullptr && getLastTargeted(p.getName()) == target->getName()){return false;}
    if(action == "arrest" && !p.getCanArrestStatus()){return false;}
    if(action == "tax" && !p.getcanTax()){return false;}
    return true;
}

bool Game::isUnderSanction(const std::string &name)const{
    return sanctionedPlayers.find(name) != sanctionedPlayers.end();
}

void Game::addSanction(const std::string &name){
    sanctionedPlayers.insert(name);
}

void Game::clearSanction(const std::string &name){
    sanctionedPlayers.erase(name);
}

void Game::updateLastTargeted(const std::string &attackerName, const std::string target){
    lastTargeted[attackerName] = target;
}

std::string Game::getLastTargeted(const std::string &name)const{
    auto it = lastTargeted.find(name);
    if(it != lastTargeted.end()){
        return it->second;
    }
    return "";
}

//role specific
void Game::performInvestment(Player &p){
    if(p.canInvest()){
        p.invest(*this);
    }
    else{
        throw std::runtime_error("player cannot invest\n");
    }
}

void Game::viewPlayerCoins(Player &viewer , Player &target){
    if(viewer.getRole()=="Spy"){
        viewer.viewPlayerCoins(target);
    }
    else{
        throw std::runtime_error("only spy can view others amount of coins\n");
    }
}

void Game::cancelBribe(Player &judge, Player &target){
    if(!judge.canCancelBribe(target)){
        throw std::runtime_error("cannot cancel this bribe\n");
    }
    if(bribedPlayers.find(target.getName())!= bribedPlayers.end()){
        bribedPlayers.erase(target.getName());
        judge.cancelBribe(target,*this);
        std::cout<<"bribe canceled for "<< target.getName() << std::endl;
    }
    else{
        throw std::runtime_error("this player did not bribe\n");
    }
}

void Game::blockCoup(Player &general){  
    if(general.getRole()=="General" && general.canBlock("coup")){
        General *g = dynamic_cast<General*>(&general); //specific method of general
        if(g){
            g->blockCoup(*this);
        }
    }
    else{
        throw std::runtime_error("cannot block coup\n");
     }
}

void Game::blockArrest(Player &spy, Player &target){
    if(spy.getRole() != "Spy"){
    throw std::runtime_error("only spy can block arrest\n");
    }
    if(!target.getCanArrestStatus()){ //someone already disable him for arrest next turn
    throw std::runtime_error("player already blocked for arresting next turn\n");
    }
    target.setCanArrestStatus(false);
}

void Game::blockTax(Player &governor, Player &target){
        if(governor.getRole() != "Governor"){
        throw std::runtime_error("only governor can block tax\n");
        }
        if(!target.getcanTax()){ //someone already disable him for arrest next turn
            throw std::runtime_error("player already blocked for taxing\n");
        }
        target.setCanTax(false);
}

bool Game::mustCoup(Player *p)const{
    return (p->getName() == getCurrentPlayer()->getName() && p->getAmount() >= 10);
}
#define GAME_HPP