#include "PlayerStrategies.h"
#include "../Player/Player.h"
#include "../Orders/Orders.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>
using std::cin;
using std::cout;
PlayerStrategy::PlayerStrategy() {

}

void PlayerStrategy::setStrategyName(string name) {
    strategy_name = name;
}

PlayerStrategy &PlayerStrategy::operator=(const PlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.strategy_name);
    }
    return *this;
}

PlayerStrategy::~PlayerStrategy() {

}

std::ostream &operator<<(ostream &os, const PlayerStrategy &strategy) {
    return os << strategy.strategy_name;
}

string PlayerStrategy::getStrategyName() const {
    return strategy_name;
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy &strategy) {
    this->setStrategyName(strategy.strategy_name);
}

//------
HumanPlayerStrategy::HumanPlayerStrategy() {
    setStrategyName("Human");
}

HumanPlayerStrategy::~HumanPlayerStrategy() {

}

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


HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy &strategy) {
    this->setStrategyName(strategy.getStrategyName());
}

std::ostream &operator<<(ostream &out, const HumanPlayerStrategy &strategy) {
    return out << strategy.getStrategyName();
}

HumanPlayerStrategy &HumanPlayerStrategy::operator=(const HumanPlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.getStrategyName());
    }
    return *this;
}

void HumanPlayerStrategy::issueOrder(Player *p, GameEngine *game, Deck* deck){
    int initialReinforcementPool = p->getReinforcementPool();
    vector<Player *> players = game->getPlayersList();

    do
    {
        
        if(p->getReinforcementPool() == 0) continue;
        cout<<"Player " << p->getPID() <<" How many armies of your " <<p->getReinforcementPool() <<" do you want to deploy?: ";
        int numtoDeploy;
        cin>>numtoDeploy;
        cout<<"Which territory do you want to deploy?: \n";
        for (int i=0; i<p->getDefendList().size(); i++)
        {
            cout<<i<<": "<<*p->getDefendList()[i]<<endl;
        }
        int numTerritory;
        cin>>numTerritory;
        if((numtoDeploy > 0) && (numtoDeploy <= p->getReinforcementPool()))
        {
            p->getDefendList()[numTerritory]->armyCount += numtoDeploy;
            p->setReinforcementPool(p->getReinforcementPool()-numtoDeploy);
            Deploy *deploy;
            deploy = new Deploy(p,p->getDefendList()[numTerritory],numtoDeploy);
            p->issueOrderObject(*deploy);
            cout<<"ReinforcementPool:"<<p->getReinforcementPool()<<endl;
            tempReinforcementPool = p->getReinforcementPool();
        }
    }while(p->getReinforcementPool()!=0);

    if (p->getReinforcementPool()==0)
    {
        p->setReinforcementPool(initialReinforcementPool);//sets it back to original reinforcementPool when we call execution in GameEngine
    }
    if (tempReinforcementPool==0) //makes sure that no other order is issued until player has deployed all its armies
    {
        srand (time(NULL));
        if(p->getHand()->getHandSize()!=0) //if the player has cards then one of the cards is selected and the card's play() method is called
        {
            cout<<"Do you want to issue an order from your cards? y/n: ";
            string answer;
            cin>>answer;
            if (answer=="y")
            {
                cout<<"\n"<<*p->getHand();
                cout<<"Which card would you like to select?: ";
                string card;
                cin>>card;
                if (card=="airlift")
                {
                    int index = p->getHand()->findCard(AIRLIFT);
                    p->getHand()->cardAtIndex(index).play(*p, *deck, *p->getHand());
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
                    p->issueOrderObject(*airlift);
                }
            	if (card=="bomb")
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
                    p->issueOrderObject(*bomb);
            	}
                if (card=="blockade")
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
                    p->issueOrderObject(*blockade);
            	}
                if (card=="diplomacy")
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
                    p->issueOrderObject(*negotiate);
            	}
                if (card=="reinforcement")
                {
                    int index = p->getHand()->findCard(REINFORCEMENT);
                    cout<<"Adding 5 reinforcement armies!\n";
                    p->setReinforcementPool(p->getReinforcementPool()+5);
                    p->getHand()->cardAtIndex(index).play(*p, *deck, *p->getHand());
            	}
            }
        }
        cout<<"Do you want to issue an advanced order? y/n: ";
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
            cout<<"How many armies do you want to advance (choose between 1-" << p->getDefendList()[source]->armyCount << ")?: ";
            cin>>army;
            Advance *advance = new Advance(p,targetP,p->getDefendList()[source],neighbours[target],army);
            p->issueOrderObject(*advance);
        }
    }
}

//BenevolentPlayerStrategy class

BenevolentPlayerStrategy::BenevolentPlayerStrategy() {
    setStrategyName("Aggressive");
}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {

}

vector<Territory*> BenevolentPlayerStrategy::toDefend(vector<Territory*> Map, Player& player) {
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

vector<Territory*> BenevolentPlayerStrategy::toAttack(vector<Territory*> Map, Player &player) {
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


BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy &strategy) {
    this->setStrategyName(strategy.getStrategyName());
}

std::ostream &operator<<(ostream &out, const BenevolentPlayerStrategy &strategy) {
    return out << strategy.getStrategyName();
}

BenevolentPlayerStrategy &BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.getStrategyName());
    }
    return *this;
}

void BenevolentPlayerStrategy::issueOrder(Player* p, GameEngine *game, Deck* deck)
{
        int index = 0;
    for (int i=0; i<p->getDefendList().size(); i++)
    {
        if (p->getDefendList()[i]->armyCount < p->getDefendList()[index]->armyCount)
        {
            index = i;
        }
    }
    Deploy *deploy = new Deploy(p,p->getDefendList()[index],p->getReinforcementPool());
    p->issueOrderObject(*deploy);

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
        if (p->getDefendList()[i]->armyCount > p->getDefendList()[maxIndexNeighbour]->armyCount)
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
        p->issueOrderObject(*advance);
    }
}

//NeutralPlayerStrategy class, // change into aggressive if attacked
NeutralPlayerStrategy::NeutralPlayerStrategy() {
    setStrategyName("Neutral");
}

NeutralPlayerStrategy::~NeutralPlayerStrategy() {

}

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

void NeutralPlayerStrategy::issueOrder(Player *p, GameEngine *game, Deck* deck) {
    cout << "NeutralPlayerStrategy issueOrder, Ending turn.\n" << endl;
}

NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy &strategy) {
    this->setStrategyName(strategy.getStrategyName());
}

std::ostream &operator<<(ostream &out, const NeutralPlayerStrategy &strategy) {
    return out << strategy.getStrategyName();
}

NeutralPlayerStrategy &NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.getStrategyName());
    }
    return *this;
}

//CheaterPlayerStrategy class
CheaterPlayerStrategy::CheaterPlayerStrategy() {
    setStrategyName("Cheater");
}

CheaterPlayerStrategy::~CheaterPlayerStrategy() {

}

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

void CheaterPlayerStrategy::issueOrder(Player *p, GameEngine *game, Deck* deck) {
    vector<Territory*> enemyNeighbors = p->getAttackList();
    p->setTerritories(enemyNeighbors);

}

CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy &strategy) {
    this->setStrategyName(strategy.getStrategyName());
}

std::ostream &operator<<(ostream &out, const CheaterPlayerStrategy &strategy) {
    return out << strategy.getStrategyName();
}

CheaterPlayerStrategy &CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.getStrategyName());
    }
    return *this;
}

//AggressivePlayerStrategy class
AggressivePlayerStrategy::AggressivePlayerStrategy() {
    setStrategyName("Aggressive");
}

AggressivePlayerStrategy::~AggressivePlayerStrategy() {

}

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

void AggressivePlayerStrategy::issueOrder(Player *p, GameEngine *game, Deck *deck) {

}


AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &strategy) {
    this->setStrategyName(strategy.getStrategyName());
}

std::ostream &operator<<(ostream &out, const AggressivePlayerStrategy &strategy) {
    return out << strategy.getStrategyName();
}

AggressivePlayerStrategy &AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy &strategy) {
    if(this != &strategy)
    {
        this->setStrategyName(strategy.getStrategyName());
    }
    return *this;
}