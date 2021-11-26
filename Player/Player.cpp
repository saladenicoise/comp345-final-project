#include "Player.h"
#include <iostream>
#include <ostream> 
#include <string>

using namespace std;
Player::Player()
{
    this->h = new Hand();
    this->olst = new OrderList();
    this->isNeutral = 1;
} //default constructor 

Player::Player(string playerName, int isNeutral) {
    this->playerName = playerName;
    this->h = new Hand();
    this->olst = new OrderList();
    this->isNeutral = isNeutral;
}

Player::Player(string playerName)
{
    this->playerName = playerName;
    this->h = new Hand();
    this->olst = new OrderList();
    this->isNeutral = 1;
}

Player::Player(string playerName,int pid, vector<Territory*> t, Hand* h, OrderList* o, vector<Player*> noAttack, int isNeutral) // parameterized constructor for Player class
{
    this->playerName = playerName;
    this->pid = pid;
    this->t = t;
    this->h = h;
    this->olst = o;
    this->cannotAttack = noAttack;
    this->isNeutral = isNeutral;
}

Player::Player(const Player& p) //copy constructor
{
    this->playerName = p.playerName;
    this->pid = p.pid;
    for (auto c : p.t)
        this->t.push_back(new Territory(*c));
    this->h = p.h;
    this->olst = p.olst;
    for (auto a : p.cannotAttack)
        this->cannotAttack.push_back(new Player(*a));
}

Player::~Player() //destructor 
{
    playerName.clear();
    if (!t.empty()) {
    for (auto c : t) {
        delete c;
        c = nullptr;
        }
    }
    t.clear();
    delete this->h;
    delete this->olst;
    this->h = nullptr;
    this->olst = nullptr;
    if(!cannotAttack.empty()) {
        for(auto a : cannotAttack) {
            delete a;
            a = nullptr;
        }
    }
    cannotAttack.clear();
}

Player &Player::operator=(const Player &p) //assignment operator overloader to create deep copy 
{
    if(this == &p)
    {
        // Self-assignment guard
        return *this;
    }
    this->playerName = p.playerName;
    this->pid = p.pid;
    this->t = p.t;
    this->h = p.h;
    this->olst = p.olst;
    this->cannotAttack = p.cannotAttack;
    return *this;
}

std::ostream &operator<<(std::ostream& os,const Player& p) //overloads stream insertion
{
    return os << "Initializing Player" << ": "  << p.playerName<< "\n" << std::endl;
}

string Player::getPlayerName() //gets player's name 
{
    return playerName;
}

int Player::getPID() //gets player's id
{
    return pid;
}

Hand *Player::getHand() //returns hand object
{
    return h;
}

OrderList *Player::getOrderList() //returns order list object
{
	return olst;
}

vector<Player*> Player::getNotAttackablePlayers() { // get not attackable players
    return cannotAttack;
}

void Player::setNotAttackablePlayers(vector<Player*> cannotAttack) { //set not attackable players
    this->cannotAttack = cannotAttack;
}

vector<Territory*> Player::getTerritories(){ // get players territories
	return t;
}

void Player::setTerritories(vector<Territory*> newTerritoryList) {
    t = newTerritoryList; //Update the territory list
}

void Player::issueOrder(const string& order) //creates an order object and adds it to the list of orders. 
{
    Order *newOrder = new Order(order);
    this->olst->addOrder(newOrder);
}

int Player::getReinforcementPool() { // gets the number of armies in popl
    return reinforcementPool;
}

void Player::setReinforcementPool(int pool) { // set pool size
    this->reinforcementPool = pool;
}

void Player::setpID(int pID) { //sets players id
    this->pid = pID;
}

int Player::getNeutral() {
    return this->isNeutral;
}

int Player::getGetCard() {
    return this->getCard;
}

void Player::setGetCard(int getCard) {
    this->getCard = getCard;
}

vector<Territory*> Player::getNeighbour(vector<Territory*> Map) //finds the neighbouring teritories of a player
{
    vector<Territory*> neighbouring_terrritories; //for each territory, find the neighbours of the territory
    for (int i = 0; i<t.size(); i++)
    {
      for(int j = 0; j < t[i]->edges.size(); j++){
          bool contains = false;
          for(Territory* terr : neighbouring_terrritories){
              if (terr = t[i]->edges[j]){
                  contains = true;
              }
          }
          if(!contains){
              neighbouring_terrritories.push_back(t[i]->edges[j]);
          }
      }
    }
    return neighbouring_terrritories;   //returns a player's list of territories that can be attacked
}

vector<Territory*> Player::toAttack(vector<Territory*> Map, Player& p)
{
	vector<Territory*> toAttackTerritory = getNeighbour(Map);
	cout << "The list of territories that are be attacked:" << endl;
	for (int i = 0; i < toAttackTerritory.size(); i++)
	{
	    attackList.push_back(toAttackTerritory[i]);
		cout << *toAttackTerritory[i] << endl;
	}
	return attackList;
}

vector<Territory*> Player::toDefend(vector<Territory*> toDefendTerritory, Player& p)
{
    std::string str ("Defend: ");
	cout << "The list of territories that are be defended:" << endl;
	for (int i = 0; i < toDefendTerritory.size(); i++)
	{
	    defendList.push_back(toDefendTerritory[i]);
		cout << *toDefendTerritory[i] << endl;
      //  p.issueOrder(str.append(toDefendTerritory[i]->countryName));
	}
	return defendList;
}

void Player::issueOrderObject(const Order) //creates an order object and adds it to the list of orders. 
{
    Order *newOrder = new Order();
    this->olst->addOrder(newOrder);
}



void Player::deployArmies(Player* p) 	
{
    int initialReinforcementPool = getReinforcementPool();
    do
    {
        if(getReinforcementPool() == 0) continue;
        cout<<"How many armies do you want to deploy?: ";
        int numtoDeploy;
        cin>>numtoDeploy;
        cout<<"Which territory do you want to deploy?: \n";
        for (int i=0; i<defendList.size(); i++)
        {
            cout<<i<<": "<<*defendList[i]<<endl;
        }
        int numTerritory;
        cin>>numTerritory;
        if((numtoDeploy > 0) && (numtoDeploy <= p->getReinforcementPool()))
        {
            defendList[numTerritory]->armyCount += numtoDeploy;
            p->setReinforcementPool(p->getReinforcementPool()-numtoDeploy);
            Deploy *deploy;
            deploy = new Deploy(p,defendList[numTerritory],numtoDeploy);
            p->issueOrderObject(*deploy);
            cout<<"ReinforcementPool:"<<p->getReinforcementPool()<<endl;
            tempReinforcementPool = getReinforcementPool();
        }
    }while(getReinforcementPool()!=0);

    if (getReinforcementPool()==0)
    {
        p->setReinforcementPool(initialReinforcementPool);//sets it back to original reinforcementPool when we call execution in GameEngine
    }
}

void Player::selectOrder(Player* p, vector<Player*> targetPlayer, Deck* deck)
{
    srand (time(NULL));
    if(h->getHandSize()!=0) //if the player has cards then one of the cards is selected and the card's play() method is called
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
                for (int i=0; i<defendList.size(); i++)
                {
                    cout<<i<<": "<<*defendList[i]<<endl;
                }
                cout<<"Which territory do you want to airlift?: ";
                int source,target,army;
                cin>>source;
                cout<<"Which territory do you want to target?: ";
                cin>>target;
                cout<<"How many armies do you want to airlift (choose between 1-" << defendList[source]->armyCount << ")?: ";
                cin>>army;
                Airlift *airlift = new Airlift(p,defendList[source],defendList[target],army);
                p->issueOrderObject(*airlift);
            }
        	if (card=="bomb")
        	{
        	    vector<Territory*> bombTerritories;
                int index = p->getHand()->findCard(BOMB);
                p->getHand()->cardAtIndex(index).play(*p, *deck, *p->getHand());
                for (int i=0; i<targetPlayer.size(); i++)
                {
                    if (p!=targetPlayer[i])
                    {
                        for (int t=0; t<attackList.size(); t++)
                        {
                            if (std::find(targetPlayer[i]->getTerritories().begin(), targetPlayer[i]->getTerritories().end(), attackList[t]) != targetPlayer[i]->getTerritories().end()) 
                            {
                                cout<<t<<": " <<*targetPlayer[i]->getTerritories()[t]<<endl;
                                bombTerritories.push_back(targetPlayer[i]->getTerritories()[t]);
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
                for (int i=0; i<defendList.size(); i++)
                {
                    cout<<i<<": "<<*defendList[i]<<endl;
                }
                cout<<"Which territory do you want to blockade?: ";
                int numTerritory;
                cin>>numTerritory;
                Player* neutralPlayer;
                for (int i=0; i<targetPlayer.size(); i++)
                {
                    if (targetPlayer[i]->getNeutral()==1)
                    {
                        neutralPlayer = targetPlayer[i];
                        break;
                    }
                }
                Blockade *blockade = new Blockade(p,defendList[numTerritory],neutralPlayer);
                p->issueOrderObject(*blockade);
        	}
            if (card=="diplomacy")
            {
                int index = p->getHand()->findCard(DIPLOMACY);
                for (int i=0; i<targetPlayer.size(); i++)
                {
                    cout<<i<<":"<<*targetPlayer[i]<<endl;
                }
                cout<<"Which player do you want to negotiate with?: ";
                int target;
                cin>>target;
                p->getHand()->cardAtIndex(index).play(*p, *deck, *p->getHand());
                Negotiate *negotiate = new Negotiate(p,targetPlayer[target]);
                p->issueOrderObject(*negotiate);
        	}
        }
    }
    cout<<"Do you want to issue an advanced order? y/n: ";
    string answer;
    cin>>answer;
    if (answer=="y")
    {
        for (int i=0; i<defendList.size(); i++)
        {
            cout<<i<<": "<<*defendList[i]<<endl;
        }
        cout<<"Which territory do you want to advance?: ";
        int source,target,army;
        cin>>source;
        vector<Territory*> neighbours;
        for(int i = 0; i < defendList[source]->edges.size(); i++)
        {
            if (defendList[source]!=defendList[source]->edges[i])
            {
                cout<<i<<": "<< *defendList[source]->edges[i]<<endl;
                neighbours.push_back(defendList[source]->edges[i]);
            }
        }
        cout<<"Which territory do you want to target the advance?: ";
        cin>>target;
        Player * targetP;
        for (int i=0; i<targetPlayer.size(); i++)
        {
            cout<<"Territory size: " << targetPlayer[i]->getTerritories().size()<< endl;;
            for (int t=0; t<targetPlayer[i]->getTerritories().size(); t++)
            {
                if (std::find(targetPlayer[i]->getTerritories().begin(), targetPlayer[i]->getTerritories().end(), neighbours[target]) != targetPlayer[i]->getTerritories().end()) 
                {
                    targetP = targetPlayer[i];
                    break;
                }
            }
        }
        cout<<"How many armies do you want to advance (choose between 1-" << defendList[source]->armyCount << ")?: ";
        cin>>army;
        Advance *advance = new Advance(p,targetP,defendList[source],neighbours[target],army);
        p->issueOrderObject(*advance);
    }
}
void Player::issuingOrder(Player* p, Player* targetPlayer, Deck* deck)
{
    deployArmies(p);

    if (tempReinforcementPool==0) //makes sure that no other order is issued until player has deployed all its armies
    {
        selectOrder(p,targetPlayer, deck);
    }
}

