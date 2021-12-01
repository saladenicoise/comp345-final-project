#pragma once

#include "../Cards/Cards.h"
#include <vector>

using namespace std;

class Player;
class Deck;
class GameEngine;
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
    virtual void issueOrder(Player *p, GameEngine *game, Deck* deck) = 0;

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
    void issueOrder(Player *p, GameEngine *game, Deck* deck);
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
    void issueOrder(Player *p, GameEngine *game, Deck* deck);


    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const BenevolentPlayerStrategy& strategy);

    // Assignment Operator
    BenevolentPlayerStrategy& operator =(const BenevolentPlayerStrategy& strategy);
};

class NeutralPlayerStrategy : public PlayerStrategy {
    public:
    NeutralPlayerStrategy(); // Default constructor
    
    NeutralPlayerStrategy(const NeutralPlayerStrategy& strategy); // Copy constructor
    ~NeutralPlayerStrategy(); // Destructor

    // Strategy Methods inherited
    vector<Territory*> toDefend(vector<Territory*> Map, Player &player);
    vector<Territory*> toAttack(vector<Territory*> Map, Player &player);
    void issueOrder(Player *p, GameEngine *game, Deck* deck);


    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const NeutralPlayerStrategy& strategy);

    // Assignment Operator
    NeutralPlayerStrategy& operator =(const NeutralPlayerStrategy& strategy);
};

class CheaterPlayerStrategy : public PlayerStrategy {
    public:
    CheaterPlayerStrategy(); // Default constructor
    
    CheaterPlayerStrategy(const CheaterPlayerStrategy& strategy); // Copy constructor
    ~CheaterPlayerStrategy(); // Destructor

    // Strategy Methods inherited
    vector<Territory*> toDefend(vector<Territory*> Map, Player &player);
    vector<Territory*> toAttack(vector<Territory*> Map, Player &player);
    void issueOrder(Player *p, GameEngine *game, Deck* deck);


    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const CheaterPlayerStrategy& strategy);

    // Assignment Operator
    CheaterPlayerStrategy& operator =(const CheaterPlayerStrategy& strategy);
};