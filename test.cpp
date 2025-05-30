#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Player.hpp"
#include "Game.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Governor.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Spy.hpp"
#include <string>
#include <vector>
#include <stdexcept>

Game* createTestGame(){
    return new Game(0);
}

TEST_CASE("Game basic logic and setup") {
    Game* game = createTestGame();
    
    SUBCASE("Add Player") {
    Player* p1 = new Player("p1", 2,"Governor",true,true,false,true); 
    Player* p2 = new Player("p2", 2,"Judge",true,true,false,true);
        game->addPlayer(p1);
        CHECK(game->getPlayers().size()==1);
        CHECK(game->getPlayersNames().size()==1);
        game->addPlayer(p2);
        CHECK(game->getPlayers().size()==2);
        CHECK(game->getPlayersNames().size()==2);
        
        Player* p1_again = new Player("p1", 2,"Governor",true,true,false,true);
        CHECK_THROWS_AS(game->addPlayer(p1_again), std::invalid_argument);
        delete p1_again;

        CHECK_THROWS_AS(game->addPlayer(nullptr), std::invalid_argument);
    }

    SUBCASE("Turn Management") {
        Game* turnGame = createTestGame();
        Player* p1_turn = new Player("p1", 2,"Governor",true,true,false,true); 
        Player* p2_turn = new Player("p2", 2,"Judge",true,true,false,true);
        Player* p3_turn = new Player("p3", 2,"Spy",true,true,false,true);
    
        turnGame->addPlayer(p1_turn);
        turnGame->addPlayer(p2_turn);
        turnGame->addPlayer(p3_turn);

        CHECK(turnGame->turn() == "p1");
        turnGame->nextTurn();
        CHECK(turnGame->turn() == "p2");
        turnGame->nextTurn();
        CHECK(turnGame->turn() == "p3");
        turnGame->nextTurn();
        CHECK(turnGame->turn() == "p1");

        p2_turn->setActive(false);
        while(turnGame->turn() != "p1") {
            turnGame->nextTurn();
        }
        turnGame->nextTurn(); 
        CHECK(turnGame->turn() == "p3"); 

        p3_turn->setExtraTurn(true);
        turnGame->nextTurn();
        CHECK(turnGame->turn() == "p3"); 
        CHECK(p3_turn->getExtraTurn() == false); 
        delete turnGame;
    }

    SUBCASE("Game Over and Winner") {
        Game* overGame = createTestGame();
        Player* p1_over = new Player("winner", 2,"Governor",true,true,false,true); 
        Player* p2_over = new Player("loser1", 2,"Judge",true,true,false,true);
        Player* p3_over = new Player("loser2", 2,"Spy",true,true,false,true);
    
        overGame->addPlayer(p1_over);
        overGame->addPlayer(p2_over);
        overGame->addPlayer(p3_over);
    
        CHECK(overGame->isGameOver()==false);
        CHECK_THROWS_AS(overGame->winner(), std::runtime_error);
        p2_over->setActive(false);
        p3_over->setActive(false);
        CHECK(overGame->isGameOver()==true);
        CHECK(overGame->winner() == "winner");
        delete overGame;
    }

    delete game;
}

TEST_CASE("specific Game Actions") {
    Game* game = createTestGame();

    Judge* judge = new Judge("p1_judge", 2); 
    Governor* governor = new Governor("p2_governor", 2);
    Spy* spy = new Spy("p3_spy", 2);
    General* general = new General("p4_general", 2);
    Baron* baron = new Baron("p5_baron", 2);
    
    game->addPlayer(judge);
    game->addPlayer(governor);
    game->addPlayer(spy);
    game->addPlayer(general);
    game->addPlayer(baron);

    SUBCASE("gather Action") {
        while(judge->getAmount() < 2) judge->incrementBy(1);
        while(judge->getAmount() > 2) judge->decrementBy(1);
        game->clearSanction(judge->getName());
        int initialCoins = judge->getAmount();
        game->gather(*judge);
        CHECK(judge->getAmount() == initialCoins + 1);
        
        game->addSanction(judge->getName());
        CHECK_THROWS_AS(game->gather(*judge), std::runtime_error); 
        game->clearSanction(judge->getName());
    }
        
    SUBCASE("tax Action governor") {
        while(governor->getAmount() < 2) governor->incrementBy(1);
        while(governor->getAmount() > 2) governor->decrementBy(1);
        game->clearSanction(governor->getName());
        governor->setCanTax(true);

        int initialCoins = governor->getAmount();
        game->tax(*governor);
        CHECK(governor->getAmount() == initialCoins + governor->getTaxAmount()); 
    }
    
    SUBCASE("tax regular") {
        while(judge->getAmount() < 2) judge->incrementBy(1);
        while(judge->getAmount() > 2) judge->decrementBy(1);
        game->clearSanction(judge->getName());
        judge->setCanTax(true);

        int initialCoins = judge->getAmount();
        game->tax(*judge); 
        CHECK(judge->getAmount() == initialCoins + judge->getTaxAmount()); 
    }

    SUBCASE("coup action") {
        while(judge->getAmount() < 7) judge->incrementBy(1);
        while(judge->getAmount() > 7) judge->decrementBy(1);
        general->setActive(true); 
        
        game->coup(*judge, *general);
        CHECK(judge->getAmount() == 0); 
        CHECK(general->isActive() == false);
        general->setActive(true); 
        while(general->getAmount() < 2) general->incrementBy(1);
    }

    SUBCASE("arrest action") {  
        while(baron->getAmount() < 2) baron->incrementBy(1);
        while(baron->getAmount() > 2) baron->decrementBy(1);
        while(spy->getAmount() < 2) spy->incrementBy(1);
        while(spy->getAmount() > 2) spy->decrementBy(1);
        baron->setCanArrestStatus(true);
        spy->setActive(true);
    
        int initialBaronCoins = baron->getAmount();
        int initialTargetRichCoins = spy->getAmount();
        game->arrest(*baron, *spy);
        CHECK(spy->getAmount() == initialTargetRichCoins - 1);
        CHECK(baron->getAmount() == initialBaronCoins + 1);

        baron->setCanArrestStatus(false);
        CHECK_THROWS_AS(game->arrest(*baron, *spy), std::runtime_error); 
        baron->setCanArrestStatus(true); 
    }

    SUBCASE("sanction action") {
        while(governor->getAmount() < 3) governor->incrementBy(1);
        while(governor->getAmount() > 3) governor->decrementBy(1);
        spy->setActive(true);
        game->clearSanction(spy->getName());

        int initialGovCoins = governor->getAmount();
        game->sanction(*governor, *spy);
        CHECK(governor->getAmount() == initialGovCoins - 3); 
        CHECK(game->isUnderSanction(spy->getName()) == true);
        game->clearSanction(spy->getName());
    }

    SUBCASE("bribe action") {
        while(baron->getAmount() < 4) baron->incrementBy(1);
        while(baron->getAmount() > 4) baron->decrementBy(1);
        
        int initialBaronCoins = baron->getAmount();
        game->bribe(*baron);
        CHECK(baron->getAmount() == initialBaronCoins - 4);
        baron->setExtraTurn(false);
    }

    SUBCASE("mustCoup") {
        while(game->getCurrentPlayer() != governor) { //need to be the current player
        game->nextTurn();
        }
        while(governor->getAmount() < 10) governor->incrementBy(1);
        CHECK(game->mustCoup(governor) == true);
        while(governor->getAmount() > 2) governor->decrementBy(1); 
        while(governor->getAmount() < 2) governor->incrementBy(1);
        CHECK(game->mustCoup(governor) == false);
    }
    
    SUBCASE("baron investment") {        
        while(baron->getAmount() < 3) baron->incrementBy(1);
        while(baron->getAmount() > 3) baron->decrementBy(1);
        
        int initialCoins = baron->getAmount();
        game->performInvestment(*baron);
        CHECK(baron->getAmount() == initialCoins + 3);
        
        while(baron->getAmount() > 1) baron->decrementBy(1);
        CHECK_THROWS_AS(game->performInvestment(*baron), std::runtime_error);
    }

    SUBCASE("judge cancelBribe") {
        while(spy->getAmount() < 4) spy->incrementBy(1);
        while(spy->getAmount() > 4) spy->decrementBy(1);
        spy->setExtraTurn(false);

        game->bribe(*spy); 
        REQUIRE(spy->getExtraTurn() == true);
    }

    delete game; 
}
