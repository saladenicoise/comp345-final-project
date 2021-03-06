#include "PlayerStrategies.h"
#include "../Player/Player.h"
#include "../Orders/Orders.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>
using std::cin;
using std::cout;
// Default constructor
PlayerStrategy::PlayerStrategy() {

}

// Mutator
void PlayerStrategy::setStrategyName(string name) {
    strategy_name = name;
}

//Assignment operator
PlayerStrategy &PlayerStrategy::operator=(const PlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.strategy_name);
    }
    return *this;
}

PlayerStrategy::~PlayerStrategy() {

}
// output overload
std::ostream &operator<<(ostream &os, const PlayerStrategy &strategy) {
    return os << strategy.strategy_name;
}
// accesor 
string PlayerStrategy::getStrategyName() const {
    return strategy_name;
}
// copy constructor
PlayerStrategy::PlayerStrategy(const PlayerStrategy &strategy) {
    this->setStrategyName(strategy.strategy_name);
}

//Human Class
HumanPlayerStrategy::HumanPlayerStrategy() {
    setStrategyName("Human");
}

//Default constructor
HumanPlayerStrategy::~HumanPlayerStrategy() {

}
// to defend method
vector<Territory*> HumanPlayerStrategy::toDefend(vector<Territory*> Map, Player& player) {
    vector<Territory*> toDefendTerritory = player.getTerritories();
    cout << "\nPlayer " <<player.getPID() << "'s list of territories that can be defended:" << endl;
	for (int i = 0; i < Map.size(); i++)
	{
        player.getDefendList().push_back(toDefendTerritory[i]);
		cout << *toDefendTerritory[i] << endl;
        
	}
    player.setDefendList(toDefendTerritory);
	return player.getDefendList();
}

// to attack method
vector<Territory*> HumanPlayerStrategy::toAttack(vector<Territory*> Map, Player &player) {
	vector<Territory*> toAttackTerritory = player.getNeighbour(Map);
	cout << "\nPlayer " <<player.getPID() << "'s list of territories that can be attacked:" << endl;
	for (int i = 0; i < toAttackTerritory.size(); i++)
	{
	    player.getAttackList().push_back(toAttackTerritory[i]);
		cout << *toAttackTerritory[i] << endl;
	}
    player.setAttackList(toAttackTerritory);
	return player.getAttackList();
}

// copy constructor
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy &strategy) {
    this->setStrategyName(strategy.getStrategyName());
}

// output overload
std::ostream &operator<<(ostream &out, const HumanPlayerStrategy &strategy) {
    return out << strategy.getStrategyName();
}

// assignment overload
HumanPlayerStrategy &HumanPlayerStrategy::operator=(const HumanPlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.getStrategyName());
    }
    return *this;
}

void HumanPlayerStrategy::issueOrder(Player *p, GameEngine *game, Deck* deck){
    int initialReinforcementPool = p->getReinforcementPool(); // get players armies
    vector<Player *> players = game->getPlayersList(); // get game players

    do
    {
        
        if(p->getReinforcementPool() == 0) continue;
        cout<<"Player " << p->getPID() <<" How many armies of your " <<p->getReinforcementPool() <<" do you want to deploy?: ";
        int numtoDeploy;
        cin>>numtoDeploy;
        cout<<"Which territory do you want to deploy?: \n";
        for (int i=0; i<p->getDefendList().size(); i++) // loop defense list
        {
            cout<<i<<": "<<*p->getDefendList()[i]<<endl; // show territories
        }
        int numTerritory;
        cin>>numTerritory;
        if((numtoDeploy > 0) && (numtoDeploy <= p->getReinforcementPool())) // deploy to terr
        {
            p->getDefendList()[numTerritory]->armyCount += numtoDeploy;
            p->setReinforcementPool(p->getReinforcementPool()-numtoDeploy);
            Deploy *deploy;
            deploy = new Deploy(p,p->getDefendList()[numTerritory],numtoDeploy);
            p->issueOrderObject(deploy);
            cout<<"ReinforcementPool:"<<p->getReinforcementPool()<<endl;
            tempReinforcementPool = p->getReinforcementPool();
        }
    }while(p->getReinforcementPool()!=0); // loop if you have armies

    if (p->getReinforcementPool()==0)
    {
        p->setReinforcementPool(initialReinforcementPool);//sets it back to original reinforcementPool when we call execution in GameEngine
    }
    if (tempReinforcementPool==0) //makes sure that no other order is issued until player has deployed all its armies
    {
        srand (time(NULL));
        if(p->getHand()->getHandSize()!=0) //if the player has cards then one of the cards is selected and the card's play() method is called
        {
            cout<<"Do you want to issue an order from your cards? y/n: "; // play cards
            string answer;
            cin>>answer;
            if (answer=="y")
            {
                cout<<"\n"<<*p->getHand();
                cout<<"Which card would you like to select?: ";
                string card;
                cin>>card;
                if (card=="airlift") // airlift card played
                {
                    int index = p->getHand()->findCard(AIRLIFT); // find card in hand
                    p->getHand()->cardAtIndex(index).play(*p, *deck, *p->getHand()); // play card
                    for (int i=0; i<p->getDefendList().size(); i++)
                    {
                        cout<<i<<": "<<*p->getDefendList()[i]<<endl;
                    }
                    cout<<"Which territory do you want to airlift?: ";
                    int source,target,army;
                    cin>>source;
                    cout<<"Which territory do you want to target?: ";
                    cin>>target;
                    cout<<"How many armies do you want to airlift (choose between 1-" << p->getDefendList()[source]->armyCount << ")?: ";
                    cin>>army;
                    Airlift *airlift = new Airlift(p,p->getDefendList()[source],p->getDefendList()[target],army);
                    p->issueOrderObject(airlift);
                }
            	if (card=="bomb") // bomb played
            	{
            	    vector<Territory*> bombTerritories;
                    int index = p->getHand()->findCard(BOMB);
                    p->getHand()->cardAtIndex(index).play(*p, *deck, *p->getHand());
                    for (int i=0; i<players.size(); i++)
                    {
                        if (p!=players[i])
                        {
                            for (int t=0; t<p->getAttackList().size(); t++)
                            {
                                if (std::find(players[i]->getTerritories().begin(), players[i]->getTerritories().end(), p->getAttackList()[t]) != players[i]->getTerritories().end()) 
                                {
                                    cout<<t<<": " <<*players[i]->getTerritories()[t]<<endl;
                                    bombTerritories.push_back(players[i]->getTerritories()[t]);
                                }
                            }
                        }
                    }
                    int numTerritory;
                    cin>>numTerritory;
                    Bomb *bomb = new Bomb(p,bombTerritories[numTerritory]);
                    p->issueOrderObject(bomb);
            	}
                if (card=="blockade") // blockade played
            	{
            	    int index = p->getHand()->findCard(BLOCKADE);
                    p->getHand()->cardAtIndex(index).play(*p, *deck, *p->getHand());
                    for (int i=0; i<p->getDefendList().size(); i++)
                    {
                        cout<<i<<": "<<*p->getDefendList()[i]<<endl;
                    }
                    cout<<"Which territory do you want to blockade?: ";
                    int numTerritory;
                    cin>>numTerritory;
                    Player* neutralPlayer;
                    for (int i=0; i<players.size(); i++)
                    {
                        if (players[i]->getNeutral()==1)
                        {
                            neutralPlayer = players[i];
                            break;
                        }
                    }
                    Blockade *blockade = new Blockade(p,p->getDefendList()[numTerritory],neutralPlayer);
                    p->issueOrderObject(blockade);
            	}
                if (card=="diplomacy") // negotiate
                {
                    int index = p->getHand()->findCard(DIPLOMACY);
                    for (int i=0; i<players.size(); i++)
                    {
                        cout<<i<<":"<<*players[i]<<endl;
                    }
                    cout<<"Which player do you want to negotiate with?: ";
                    int target;
                    cin>>target;
                    p->getHand()->cardAtIndex(index).play(*p, *deck, *p->getHand());
                    Negotiate *negotiate = new Negotiate(p,players[target]);
                    p->issueOrderObject(negotiate);
            	}
                if (card=="reinforcement") // reinforce armies
                {
                    int index = p->getHand()->findCard(REINFORCEMENT);
                    cout<<"Adding 5 reinforcement armies!\n";
                    p->setReinforcementPool(p->getReinforcementPool()+5);
                    p->getHand()->cardAtIndex(index).play(*p, *deck, *p->getHand());
            	}
            }
        }
        cout<<"Do you want to issue an advanced order? y/n: "; // advance to a territory
        string answer;
        cin>>answer;
        if (answer=="y")
        {
            for (int i=0; i<p->getDefendList().size(); i++)
            {
                cout<<i<<": "<<*p->getDefendList()[i]<<endl;
            }
            cout<<"Which territory do you want to advance?: ";
            int source,target,army;
            cin>>source;
            vector<Territory*> neighbours;
            for(int i = 0; i < p->getDefendList()[source]->edges.size(); i++)
            {
                if (p->getDefendList()[source]!=p->getDefendList()[source]->edges[i])
                {
                    cout<<i<<": "<< *p->getDefendList()[source]->edges[i]<<endl;
                    neighbours.push_back(p->getDefendList()[source]->edges[i]);
                }
            }
            cout<<"Which territory do you want to target the advance?: ";
            cin>>target;
            Player * targetP;
            for (int i=0; i<players.size(); i++)
            {
                cout<<"Territory size: " << players[i]->getTerritories().size()<< endl;;
                for (int t=0; t<players[i]->getTerritories().size(); t++)
                {
                    if (std::find(players[i]->getTerritories().begin(), players[i]->getTerritories().end(), neighbours[target]) != players[i]->getTerritories().end()) 
                    {
                        targetP = players[i];
                        break;
                    }
                }
            }
  		// Switch strategy if target is neutral
            if(targetP->getPlayerStrategy()->getStrategyName() == "Neutral"){
                cout << "Neutral Player Attacked!\nSwitching Neutral Player: " << targetP->getPID() <<" to cheater/aggressive" << endl;
                targetP->setStrategy(new CheaterPlayerStrategy());
            }
            cout<<"How many armies do you want to advance (choose between 1-" << p->getDefendList()[source]->armyCount << ")?: ";
            cin>>army;
            Advance *advance = new Advance(p,targetP,p->getDefendList()[source],neighbours[target],army);
            p->issueOrderObject(advance);
        }
    }
}

//BenevolentPlayerStrategy class
// default constructor
BenevolentPlayerStrategy::BenevolentPlayerStrategy() {
    setStrategyName("Benevolent");
}
// destructor
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {

}
// to defend
vector<Territory*> BenevolentPlayerStrategy::toDefend(vector<Territory*> Map, Player& player) {
    vector<Territory*> toDefendTerritory = player.getTerritories(); // terr player owns
    cout << "\nPlayer " <<player.getPID() << "'s list of territories that can be defended:" << endl;
	for (int i = 0; i < Map.size(); i++)
	{
        player.getDefendList().push_back(toDefendTerritory[i]);
		cout << *toDefendTerritory[i] << endl;
        
	}
    player.setDefendList(toDefendTerritory);
	return player.getDefendList();
}

// to attack
vector<Territory*> BenevolentPlayerStrategy::toAttack(vector<Territory*> Map, Player &player) {
	vector<Territory*> toAttackTerritory = player.getNeighbour(Map); // neighbors of player
	cout << "\nPlayer " <<player.getPID() << "'s list of territories that can be attacked:" << endl;
	for (int i = 0; i < toAttackTerritory.size(); i++)
	{
	    player.getAttackList().push_back(toAttackTerritory[i]);
		cout << *toAttackTerritory[i] << endl;
	}
    player.setAttackList(toAttackTerritory);
	return player.getAttackList();
}

// copy constructor
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy &strategy) {
    this->setStrategyName(strategy.getStrategyName());
}
// output overload
std::ostream &operator<<(ostream &out, const BenevolentPlayerStrategy &strategy) {
    return out << strategy.getStrategyName();
}
// asignnment overload
BenevolentPlayerStrategy &BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.getStrategyName());
    }
    return *this;
}

// issue order
void BenevolentPlayerStrategy::issueOrder(Player* p, GameEngine *game, Deck* deck)
{
        int index = 0;
    for (int i=0; i<p->getDefendList().size(); i++) // get players defend list
    {
        if (p->getDefendList()[i]->armyCount < p->getDefendList()[index]->armyCount)
        {
            index = i;
        }
    }
    Deploy *deploy = new Deploy(p,p->getDefendList()[index],p->getReinforcementPool());
    p->issueOrderObject(deploy); 

    int minIndex = 0;
    for (int i=0; i<p->getDefendList().size(); i++)
    {
        if (p->getDefendList()[i]->armyCount < p->getDefendList()[minIndex]->armyCount)
        {
            minIndex = i; 
        }
    }
    vector<Territory*> weakestTerritory;
    weakestTerritory.push_back(p->getDefendList()[minIndex]);
    vector<Territory*> neighbour = p->getNeighbour(weakestTerritory);
    int maxIndexNeighbour = 0;

    for (int i=0; i<p->getDefendList().size(); i++)
    {
        if (p->getDefendList()[i]->armyCount > p->getDefendList()[maxIndexNeighbour]->armyCount) // deploy to territories with min armies
        {
            maxIndexNeighbour = i;
        }
    }
    //makes sure target territory belongs to the player issuing the order
    bool result = false;
    if (std::find(p->getTerritories().begin(), p->getTerritories().end(), neighbour[maxIndexNeighbour]) != p->getTerritories().end()) 
    {
        result = true;
    }
    
    if (result == true)
    {
        int army = neighbour[maxIndexNeighbour]->armyCount/2;
        Advance *advance = new Advance(p,p,neighbour[maxIndexNeighbour],p->getDefendList()[minIndex],army);
        p->issueOrderObject(advance);
    }
}

//NeutralPlayerStrategy class, // change into aggressive if attacked
NeutralPlayerStrategy::NeutralPlayerStrategy() { // default constructor
    setStrategyName("Neutral");
}

NeutralPlayerStrategy::~NeutralPlayerStrategy() { // destructor

}

// to defend
vector<Territory*> NeutralPlayerStrategy::toDefend(vector<Territory*> Map, Player& player) {
    vector<Territory*> toDefendTerritory = player.getTerritories();
    cout << "\nPlayer " <<player.getPID() << "'s list of territories that can be defended:" << endl;
	for (int i = 0; i < Map.size(); i++)
	{
        player.getDefendList().push_back(toDefendTerritory[i]);
		cout << *toDefendTerritory[i] << endl;
        
	}
    player.setDefendList(toDefendTerritory);
	return player.getDefendList();
}

//to attack
vector<Territory*> NeutralPlayerStrategy::toAttack(vector<Territory*> Map, Player &player) {
 	vector<Territory*> toAttackTerritory = player.getNeighbour(Map);
	cout << "\nPlayer " <<player.getPID() << "'s list of territories that can be attacked:" << endl;
	for (int i = 0; i < toAttackTerritory.size(); i++)
	{
	    player.getAttackList().push_back(toAttackTerritory[i]);
		cout << *toAttackTerritory[i] << endl;
	}
    player.setAttackList(toAttackTerritory);
	return player.getAttackList();
}

// issue order
void NeutralPlayerStrategy::issueOrder(Player *p, GameEngine *game, Deck* deck) {
    // Distribute neutral players armies evenly among all of its territories
    int distributeArmies = p->getReinforcementPool()/p->getDefendList().size();
    for(int i = 0; i< p->getDefendList().size(); i++){
        p->getDefendList()[i]->armyCount += distributeArmies;
        p->setReinforcementPool(p->getReinforcementPool()-distributeArmies);
    }
    cout << "NeutralPlayerStrategy issueOrder, Ending turn.\n" << endl; // end turn, does nothing
}

// copy constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy &strategy) {
    this->setStrategyName(strategy.getStrategyName());
}

// os stream overlaod
std::ostream &operator<<(ostream &out, const NeutralPlayerStrategy &strategy) {
    return out << strategy.getStrategyName();
}

// assignment overload
NeutralPlayerStrategy &NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.getStrategyName());
    }
    return *this;
}

//CheaterPlayerStrategy class
CheaterPlayerStrategy::CheaterPlayerStrategy() { // defualt constructor
    setStrategyName("Cheater");
}

CheaterPlayerStrategy::~CheaterPlayerStrategy() { // destructor

}
// to defend
vector<Territory*> CheaterPlayerStrategy::toDefend(vector<Territory*> Map, Player& player) {
    vector<Territory*> toDefendTerritory = player.getTerritories();
    cout << "\nPlayer " <<player.getPID() << "'s list of territories that can be defended:" << endl;
	for (int i = 0; i < Map.size(); i++)
	{
        player.getDefendList().push_back(toDefendTerritory[i]);
		cout << *toDefendTerritory[i] << endl;
        
	}
    player.setDefendList(toDefendTerritory);
	return player.getDefendList();
}
// to attack
vector<Territory*> CheaterPlayerStrategy::toAttack(vector<Territory*> Map, Player &player) {
 	vector<Territory*> toAttackTerritory = player.getNeighbour(Map);
	cout << "\nPlayer " <<player.getPID() << "'s list of territories that can be attacked:" << endl;
	for (int i = 0; i < toAttackTerritory.size(); i++)
	{
	    player.getAttackList().push_back(toAttackTerritory[i]);
		cout << *toAttackTerritory[i] << endl;
	}
    player.setAttackList(toAttackTerritory);
	return player.getAttackList();
}
// issue order
void CheaterPlayerStrategy::issueOrder(Player *p, GameEngine *game, Deck* deck) {
        vector<Territory*> enemyNeigh = p->getAttackList(); // get neighbors
        vector<Player*> players = game->getPlayersList(); // get game players
        Player* targetPlayer; // person to attack

        // loop through players attack list
        for(int k = 0;k < enemyNeigh.size();k++){
            Territory* attackTarget = p->getAttackList()[k]; // set target
            // loop through game players
            for (int i=0; i<players.size(); i++)
            {
                for (int t=0; t<players[i]->getTerritories().size(); t++) // search through players territories
                {
                    if (std::find(players[t]->getTerritories().begin(), players[t]->getTerritories().end(), enemyNeigh[k]) != players[t]->getTerritories().end()) // check if player has targeted territory
                    {
                        targetPlayer = players[i];
                        break;
                    }
                }
            }

            // Distribute cheaters armies evenly among neighbors
            int distributeArmies = p->getReinforcementPool()/enemyNeigh.size();
            p->getAttackList()[k]->armyCount = distributeArmies;
            p->setReinforcementPool(p->getReinforcementPool()-distributeArmies);
            
            //Give it to the cheater (sourcePlayer or p)
            attackTarget->setOwnerId(p->getPID());
            //Remove from loser's list
            vector<Territory *> loserTer = targetPlayer->getTerritories();
            vector<Territory *> newTer;
            
            for(int i = 0; i < loserTer.size(); i++) {
                if(loserTer[i] != attackTarget) {//Is the territory of the target player the one we just stole?
                    newTer.push_back(loserTer[i]); //Add  it to the list of new territories
                }
            }
            targetPlayer->setTerritories(newTer);
            //Add to winner's list
            vector<Territory *> winnerTer = p->getTerritories();
            winnerTer.push_back(attackTarget);
            p->setTerritories(winnerTer);
            p->setGetCard(1);

            // Switch strategy if target is neutral
            if(targetPlayer->getPlayerStrategy()->getStrategyName() == "Neutral"){
                cout << "Neutral Player Attacked!\nSwitching Neutral Player: " << targetPlayer->getPID() <<" to cheater/aggressive" << endl;
                targetPlayer->setStrategy(new CheaterPlayerStrategy());
            }
        }
        cout << "Cheater has taken control of its neighbors" << endl;
}
// Mutator
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy &strategy) {
    this->setStrategyName(strategy.getStrategyName());
}

// os stream overload
std::ostream &operator<<(ostream &out, const CheaterPlayerStrategy &strategy) {
    return out << strategy.getStrategyName();
}
// assignment overload
CheaterPlayerStrategy &CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.getStrategyName());
    }
    return *this;
}

// Implementation of AggressivePlayerStrategy's default constructor
AggressivePlayerStrategy::AggressivePlayerStrategy() {
    setStrategyName("Aggressive");
}

// Implementation of AggressivePlayerStrategy's destructor
AggressivePlayerStrategy::~AggressivePlayerStrategy() {

}

// Implementation of Aggressive Player Strategy of toDefend()
vector<Territory*> AggressivePlayerStrategy::toDefend(vector<Territory*> Map, Player& player) {
    vector<Territory*> toDefendTerritory = player.getTerritories();
    cout << "\nPlayer " <<player.getPID() << "'s list of territories that can be defended:" << endl;
    for (int i = 0; i < Map.size(); i++)
    {
        player.getDefendList().push_back(toDefendTerritory[i]);
        cout << *toDefendTerritory[i] << endl;

    }
    player.setDefendList(toDefendTerritory);
    return player.getDefendList();
}

// Implementation of Aggressive Player Strategy of toAttack()
vector<Territory*> AggressivePlayerStrategy::toAttack(vector<Territory*> Map, Player &player) {
    vector<Territory*> toAttackTerritory = player.getNeighbour(Map);
    cout << "\nPlayer " <<player.getPID() << "'s list of territories that can be attacked:" << endl;
    for (int i = 0; i < toAttackTerritory.size(); i++)
    {
        player.getAttackList().push_back(toAttackTerritory[i]);
        cout << *toAttackTerritory[i] << endl;
    }
    player.setAttackList(toAttackTerritory);
    return player.getAttackList();
}

// Implementation of AggressivePlayerStrategy of issueOrder()
void AggressivePlayerStrategy::issueOrder(Player *p, GameEngine *game, Deck *deck) {

    // Getting the strongest country index.
    int maxArmiesIndex = 0;
    for(int i = 1; i < p->getDefendList().size(); i++) {
        if(p->getDefendList()[i]->armyCount > p->getDefendList()[maxArmiesIndex]->armyCount) {
            maxArmiesIndex = i;
        }
    }

    // Deploying armies from the reinforcement pool in the strongest country
    Deploy *deploy = new Deploy(p,p->getDefendList()[maxArmiesIndex],p->getReinforcementPool());
    p->issueOrderObject(deploy);

    vector<Territory*> placeholder;
    vector<Territory*> neighbours = p->getNeighbour(placeholder);
    Territory* strongestCountry = p->getDefendList()[maxArmiesIndex];

    vector<Territory*> targets = p->getAttackList();
    for(int i = 0; i < targets.size(); i++) {
        bool foundAdjacentTarget = std::find(strongestCountry->edges.begin(), strongestCountry->edges.end(), targets[i]) != strongestCountry->edges.end();
        if(foundAdjacentTarget) {
            Player* targetPlayer;
            for(int j = 0; j < game->getPlayersList().size(); j++) {
                if(game->getPlayersList()[j]->getPID() == targets[i]->getPlayerIDOccupying()) {
                    targetPlayer = game->getPlayersList()[j];
                }
            }
            Advance *advance = new Advance(p,targetPlayer,p->getDefendList()[maxArmiesIndex],targets[i],p->getDefendList()[maxArmiesIndex]->armyCount);
            p->issueOrderObject(advance);
		
	    // Switch strategy if target is neutral
            if(targetPlayer->getPlayerStrategy()->getStrategyName() == "Neutral"){
                cout << "Neutral Player Attacked!\nSwitching Neutral Player: " << targetPlayer->getPID() <<" to cheater/aggressive" << endl;
                targetPlayer->setStrategy(new CheaterPlayerStrategy());
            }
        }
    }

    
}

// Implementation of AggressivePlayerStrategy of copy constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &strategy) {
    this->setStrategyName(strategy.getStrategyName());
}

// Implementation of AggressivePlayerStrategy's overloaded stream insertion operator
std::ostream &operator<<(ostream &out, const AggressivePlayerStrategy &strategy) {
    return out << strategy.getStrategyName();
}

// Implementation of AggressivePlayerStrategy's overloaded assignment operator
AggressivePlayerStrategy &AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.getStrategyName());
    }
    return *this;
}
