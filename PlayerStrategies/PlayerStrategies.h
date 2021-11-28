#pragma once

#include "../Cards/Cards.h"
#include <vector>

using namespace std;

class Player;
class Deck;
class Territory;

// Abstract strategy for other strats to inherit
class PlayerStrategy {
private:
    string strategy_name;
public:
    PlayerStrategy(); // Default constructor
    
    PlayerStrategy(const PlayerStrategy& strategy); // Copy constructor

    virtual ~PlayerStrategy(); // Destructor

    // Strategy Methods to inherit
    virtual vector<Territory*> toDefend(vector<Territory*> Map, Player &player) = 0;
    virtual vector<Territory*> toAttack(vector<Territory*> Map, Player &player) = 0;
    virtual void issueOrder(Player *p, vector<Player*> players, Deck* deck) = 0;

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const PlayerStrategy& strategy);

    // Assignment Operator
    PlayerStrategy& operator =(const PlayerStrategy& strategy);
    
    void setStrategyName(string name); // Mutator
    string getStrategyName() const; // Accesor
};

class HumanPlayerStrategy : public PlayerStrategy {
    public:
    HumanPlayerStrategy(); // Default constructor
    
    HumanPlayerStrategy(const HumanPlayerStrategy& strategy); // Copy constructor
    ~HumanPlayerStrategy(); // Destructor

    // Strategy Methods inherited
    vector<Territory*> toDefend(vector<Territory*> Map, Player &player);
    vector<Territory*> toAttack(vector<Territory*> Map, Player &player);
    void issueOrder(Player *p, vector<Player*> players, Deck* deck);
    int tempReinforcementPool;

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const HumanPlayerStrategy& strategy);

    // Assignment Operator
    HumanPlayerStrategy& operator =(const HumanPlayerStrategy& strategy);
};

class BenevolentPlayerStrategy : public PlayerStrategy {
    public:
    BenevolentPlayerStrategy(); // Default constructor
    
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy& strategy); // Copy constructor
    ~BenevolentPlayerStrategy(); // Destructor

    // Strategy Methods inherited
    vector<Territory*> toDefend(vector<Territory*> Map, Player &player);
    vector<Territory*> toAttack(vector<Territory*> Map, Player &player);
    void issueOrder(Player *p, vector<Player*> players, Deck* deck);


    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const BenevolentPlayerStrategy& strategy);

    // Assignment Operator
    BenevolentPlayerStrategy& operator =(const BenevolentPlayerStrategy& strategy);
};
