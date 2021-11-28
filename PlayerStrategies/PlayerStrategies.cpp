#include "PlayerStrategies.h"
#include "../Player/Player.h"
#include "../Orders/Orders.h"
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

void HumanPlayerStrategy::issueOrder(Player *p, vector<Player*> players, Deck* deck){
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


