//arielsaadon1446@gmail.com

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>

class Game;

class Player{
    private:
    std::string name;
    int coins;
    std::string role;
    bool active;
    bool canArrestNextTurn;
    bool hasExtraTurn;
    bool canTax;

    public:
    Player(std::string name,int coins,std::string role,bool active,bool canArrestNextTurn,bool hasExtraTurn,bool canTax);
    virtual ~Player();

    std::string getName()const;
    int getAmount()const;
    std::string getRole()const;
    bool isActive()const;
    bool getCanArrestStatus()const;
    void setCanArrestStatus(bool status);
    bool getExtraTurn()const;
    void setExtraTurn(bool status);
    bool getcanTax()const;
    void setCanTax(bool status);
    void setActive(bool status);

    virtual int getTaxAmount()const;
    
    virtual void incrementBy( int n);
    virtual void decrementBy( int n);

    virtual bool canBlock(const std::string &action)const;
    virtual void onTaxAction(Game &game);
    virtual void onArrested(Player &attacker, Game &game);
    virtual bool onCouped(Player &attacker, Game &game);
    virtual void startTurnEffect(Game &game);

    virtual bool canInvest()const; //baron
    virtual void invest(Game &game); //baron
    virtual void viewPlayerCoins(Player &target)const; //spy
    virtual bool canCancelBribe(Player &target)const;//judge
    virtual void cancelBribe(Player &target, Game &game);//judge
};
#endif