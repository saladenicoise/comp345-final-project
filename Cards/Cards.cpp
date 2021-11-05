// This file is the compilation unit.
// This file shows the implementation of the declared classes in Cards.h

// This file uses names declared in the Cards.h header file 
#include "Cards.h"
// For random_shuffle
#include <random>
#include <algorithm>


// Card class implementation
// Default Constructor, make an empty card
Card::Card() {}

// Parameterized Constructor
Card::Card(cardType c)
{
    this->type = new cardType(c);
}

// Copy Constructor
Card::Card(const Card& orig)
{
    this->type = new cardType(*orig.type);
}

// Destructor, destory object after main function ends
Card::~Card()
{
    delete type;
    type = nullptr;
}

// Accessor function that returns a pointer to the type of the card
cardType* Card::getCardType()
{
    return (this->type);
}

void Card::play(Player& p, Deck& d, Hand& h)
{
    // Find index of card in players' hand
    int index = h.findCard(*this);

    // Check if the player has said card
    if (index == -1)
    {
        std::cout << "Cannot play this card, you do not have it in your hand!" << std::endl;
    } else {
        cardType* ct = getCardType();
        
        std::cout << "Playing Card: " << *this << std::endl;
    
        // Remove card from the hand
        h.remove(index);
        // Player orders
        if (*ct == cardType::BOMB)
            p.issueOrder("BOMB");
        else if(*ct == cardType::AIRLIFT)
            p.issueOrder("AIRLIFT");
        else if(*ct == cardType::BLOCKADE)
            p.issueOrder("BLOCKADE");
        else if(*ct == cardType::DIPLOMACY)
            p.issueOrder("DIPLOMACY");
        else if(*ct == cardType::REINFORCEMENT)
            p.issueOrder("REINFORCEMENT");
        else
            throw std::logic_error("Error: Invalid Card Type");
        // Add card back to the deck
        d.add(*ct);
    }
}

// Overloaded assignment operator that create a deep copy of a card object.
Card& Card::operator=(const Card& orig)
{
    if(this == &orig)
    {
        // Self-assignment guard
        return *this;
    }
    // Perform copy
    this->type = orig.type;

    // Return the existing object so we can chain this operator
    return *this;
}

// Overloading stream insertion
std::ostream& operator<<(std::ostream& os, const Card& c)
{
    // Display card types to console
    if (*c.type == cardType::BOMB)
        return os << "Bomb";
    else if(*c.type == cardType::AIRLIFT)
        return os << "Airlift";
    else if(*c.type == cardType::BLOCKADE)
        return os << "Blockade";
    else if(*c.type == cardType::DIPLOMACY)
        return os << "Diplomacy";
    else if(*c.type == cardType::REINFORCEMENT)
        return os << "Reinforcement";
    else
        throw std::logic_error("Error: Invalid Card Type");

    return os;
}

// Deck class implementation
// Default Constructor to initialize the deck
Deck::Deck()
{
    // Need a minimum of one of each card in order to play
    this->deck.push_back(new Card(BOMB));
    this->deck.push_back(new Card(AIRLIFT));
    this->deck.push_back(new Card(DIPLOMACY));
    this->deck.push_back(new Card(BLOCKADE));
    this->deck.push_back(new Card(REINFORCEMENT));

    this->size = new int(5);
    
    // Shuffle to randomize
    this->shuffle();
}

// Parameterized constructor, creates a deck of a certain size filled with random cards
Deck::Deck(int totalCards)
{
    // Need a minimum of one of each card guaranteed
    this->deck.push_back(new Card(BOMB));
    this->deck.push_back(new Card(AIRLIFT));
    this->deck.push_back(new Card(DIPLOMACY));
    this->deck.push_back(new Card(BLOCKADE));
    this->deck.push_back(new Card(REINFORCEMENT));

    this->size = new int(totalCards+5);

    for (int i = 0; i < totalCards; i++)
    {
        // Randomize deck with the card types
        cardType ct = cardType (std::rand() % 5 + 1);
        // Put cards at the end of the vector
        deck.push_back(new Card(ct));
    }
    // Shuffle to randomize
    this->shuffle();
}

// Copy Constructor
Deck::Deck(const Deck& orig)
{
    // Preallocate enough memory to fit all the elements
    deck.reserve(orig.getDeckSize());

    // Loop through the deck, performing copy
    for (auto c : orig.deck)
        this->deck.push_back(new Card(*c));
}

// Destructor, destory object after main function ends
Deck::~Deck()
{
    for(auto c : deck)
    {
        delete c;
        c = nullptr;
    }
    deck.clear();
}

// Draw function to get a card from the deck to the hand of the player
void Deck::draw(Hand* h)
{
    // Check if deck has cards
    if (deck.size() > 0)
    {   
        // Draw the card at the back of the vector
        Card drawnCard = *this->deck.back();
        
        // Get the type of the card
        cardType* t = drawnCard.getCardType();

        // Output message 
        std::cout << "Drawing card: " << drawnCard << std::endl;

        // Remove card from the deck
        Deck::remove();
    
        // Add the card to the hand of the player
        h->add(*t);
    } else {
        std::cout << "Cannot draw, deck is empty!" << std::endl;
    }
}

// Add function to add card to the end of the vector
void Deck::add(const cardType type)
{
    deck.push_back(new Card(type));
    shuffle();
}

// Remove function to remove a card from the back of the deck and free memory
void Deck::remove()
{
    Card* p = deck.back();
    deck.pop_back();
    delete p;
    p = nullptr;
}

// Randomly shuffles the deck of cards
void Deck::shuffle()
{
    // Initialize the starting point seed
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(deck.begin(), deck.end(), g);
}

// Get the number of cards remaining in the deck
int Deck::getDeckSize() const
{
   return deck.size();
}

// Overloading Assignment Operator
Deck& Deck::operator=(const Deck& orig)
{
  // Self-assignment guard, check if they dont point to the same address
  if (this != &orig)
  {
    // Preallocate enough memory for the elements
    deck.reserve(orig.getDeckSize());

    // Loop through the deck, performing copy
    for (auto c : orig.deck)
        this->deck.push_back(new Card(*c));
  }
  return *this;
}

// Overloading stream insertion
std::ostream& operator<<(std::ostream& os, const Deck& d)
{
    // Print the contents of the deck
    std::vector<Card*> v = (d.deck);
    for(Card* c : v)
    {
        std::cout << *c << " " << std::endl;
    }
    return os;
}

// Hand class implementation
// Default Constructor, create empty hand
Hand::Hand()
{
    this->hand = std::vector<Card*> {};
    this->size = 0;
}

// Copy Constructor
Hand::Hand(const Hand& orig)
{
    // Preallocate enough memory to fit all the elements
    hand.reserve(orig.getHandSize());

    // Loop through the deck, performing copy
    for (auto c : orig.hand)
        this->hand.push_back(new Card(*c));
}

// Destructor, destory object after main function ends
Hand::~Hand()
{
  if (!hand.empty())
  {
    for (auto c : hand)
    {
        delete c;
        c = nullptr;
    }
  }
    hand.clear();
}

// Add a card to the hand
void Hand::add(const cardType& type)
{
    hand.push_back(new Card(type));
}

// Function to remove card from the cand at a specific index
void Hand::remove(int index)
{
    hand.erase(hand.begin() + index);
}

// Get the number of cards remaining in the deck
int Hand::getHandSize() const
{
   return hand.size();
}

// Find the first instance of the card in the hand 
int Hand::findCard(Card c)
{   
    int index = -1;
    int i = 0;  // Counter
    for(auto it = hand.begin(); it != hand.end(); it++, i++)
    {
        // Found first element and break
        if (*(*hand.at(i)).getCardType() == *c.getCardType())
        {
            index = i;
            break;
        }
    }
    return index;
}

// Get the card at a specific index
Card Hand::cardAtIndex(int index)
{   
    // If index your looking for is out of range of the number of cards your have throw error
    if (index > hand.size())
        throw std::logic_error("Error: Invalid Index");
    else
        return *this->hand.at(index);
}

// Overloading Assignment Operator
Hand& Hand::operator=(const Hand& orig)
{

    // Preallocate enough memory for the elements
    hand.reserve(orig.getHandSize());
    
    // Self-assignment guard, check if they dont point to the same address
    if (this != &orig)
    {
        // Perform the copy
        for (auto c : orig.hand)
        this->hand.push_back(new Card(*c));
    }
    return *this;
}

// Overloading stream insertion
std::ostream& operator<<(std::ostream& os, const Hand& h)
{
    // Print the contents of the deck
    std::vector<Card*> v = (h.hand);
    for(Card* card : v)
    {
        std::cout << *card << " " << std::endl;
    }
    return os;
}
