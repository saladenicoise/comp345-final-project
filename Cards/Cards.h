// This is the header file Cards.h. This is the inferface for the classes
// Deck, Hand and Card.
// Values of these classes of type Card

// To avoid redefinitions and make sure that the header files' content is
// only read once
#ifndef CARDS_H
#define CARDS_H

// Includes
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

class Card;
class Hand;
class Deck;
class Player;
class Order;
class OrderList;

// States the cards can be in
enum cardType{ BOMB = 1, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY };

// Class card
class Card
{

public:
    // Default Constructor
    Card();

    //  Parameterized Constructor that initializes the Card Type
    Card(cardType c);

    // Copy Constructor
    Card(const Card& card);

    // Overloaded assignment opperator, to allow chaining operations
    Card& operator=(const Card& c);

    // Destructor
    ~Card();

    // Functions
    // Accesor Method that gets the address of the card type
    cardType* getCardType();

    // Mutator Method
    void setCardType(cardType c);

    // play Function
    void play(Player& p, Deck& d, Hand& h);

    // Friend operator overloading insertion operator that outputs a Card object to the console. 
    friend std::ostream& operator<<(std::ostream& os, const Card& c);

private:
    // Data Members
    cardType* type;
};

// Deck class
class Deck
{

public:
    // Constructor that initializes the Deck
    Deck();

    // Parameterized Constructor with a specific deck size
    Deck(const int totalCards);
    
    // Copy Constructor
    Deck(const Deck& deck);

    // Overloaded assignment opperator, to allow chaining operations
    Deck& operator=(const Deck& d);
    
    // Destructor
    ~Deck();

    // Functions
    // shuffle function
    void shuffle();

    // Add and remove functions
    void remove();
    void add(const cardType type);

    // Draw function
    void draw(Hand& h);

    // Number of elements of this vector
    int getDeckSize() const;

    // Friend operator overloading insertion operator that outputs a Deck object to the console. 
    friend std::ostream& operator<<(std::ostream& os, const Deck& d);


private:
    // Data Members
    // A collection of cards in a vector
    std::vector<Card*> deck;
    int* size;
};

// Hand class
class Hand
{

public:
    // Constructor that initializes the Deck
    Hand();

    // Parameterized Constructor with a specific hand size
    Hand(const int h);
    
    // Copy Constructor
    Hand(const Hand& hand);

    // Overloaded assignment opperator, to allow chaining operations
    Hand& operator=(const Hand& h);
    
    // Destructor
    ~Hand();

    // Functions
    // Number of elements of this vector
    int getHandSize() const;

    // Add and remove functions
    void add(cardType const& type);
    void remove(int index);

    int findCard(Card c);

    Card cardAtIndex(int pos);

    // Friend operator overloading insertion operator that outputs a Deck object to the console. 
    friend std::ostream& operator<<(std::ostream& os, const Hand& h);

private:
    // Data members
    // A collection of cards in a vector
    std::vector<Card*> hand;
    int* size;
};

// Dummy class for Player
class Player
{
    public:
        Player(); //default constructor
        Player(string playerName, int pid, Hand* h, OrderList* olst);  
        Player(const Player& p); //copy constructor
        ~Player(); //deconstructor 
        void issueOrder(const string& type);
        Hand* h;
        
        // Overloaded assignment opperator, to allow chaining operations
        Player& operator=(const Player& p);

        // Friend operator overloading insertion operator that outputs a Deck object to the console. 
        friend std::ostream& operator<<(std::ostream& os, const Player& p);
        OrderList* olst;
    private:
        string playerName;
        int pid;
        
};

// Dummy class for Order
class Order 
{
    public:
        Order(); //default constructor
        Order(const std::string& s); //default constructor
        Order(const Order& orig); //copy constructor
        
        // Overloaded assignment opperator, to allow chaining operations
        Order& operator=(const Order& o);
        // Friend operator overloading insertion operator that outputs a Deck object to the console. 
        friend std::ostream& operator<<(std::ostream& os, const Order& o);

    private:
        string order;
};

// Dummy class for OrderList
class OrderList {
    public:
        //Constructors and Destructors
        OrderList();
        OrderList(vector<Order *> orderList);
        OrderList(const OrderList &orderList);
        ~OrderList();

        //Adds an order to our list
        void addOrder(Order* order);
  
        //Stream insertion operator
        friend std::ostream& operator<<(std::ostream& os, const OrderList& ol);
        //Assignment operation
        OrderList &operator=(const OrderList &orderList);

    private:
        vector<Order *> orderList;
};

// end marker for the above's #ifndef
#endif //CARDS_H
