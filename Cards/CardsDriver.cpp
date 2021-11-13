// Driver file for Cards 

// This is the application file includes the main funtion,
// which demonstrates the classes Deck, Hand and Card

// This file uses identifiers such as Deck and hand, which are declared in the Cards.h file. 
#include "Cards.h"

using namespace std;

int main( )
{
    // Create a deck of 30 rand cards + 1 of each type, so 35
    Deck* d1 = new Deck();

    // Initialize default player to add cards to
    Player* p1 = new Player();

    std::cout << "Deck size: " << d1->getDeckSize()<< std::endl;

    // P1 draws cards
    std::cout << "\nPlayer 1 draws from the Deck:\n" << std::endl;

    for (int i = 0; i < 5; i++) {
        d1->draw(p1->getHand());
    }
    
    // Show that drawing is removing cards from the deck
    std::cout << "Cards remaining in the Deck: " << d1->getDeckSize() << std::endl;
    
    // P1's hand content
    std::cout << "\nPlayer 1's hand contains: \n" << *p1->getHand() <<std::endl;
    
    // P1 plays cards
    // Cards at index 0 removed and was added back to the deck
    p1->getHand()->cardAtIndex(0).play(*p1, *d1, *p1->getHand());
    p1->getHand()->cardAtIndex(1).play(*p1, *d1, *p1->getHand());
    
    // Show that playing the cards is removing cards from the hand and adding it back to the deck,
    // the deck gets re-shuffled after each add to keep it random
    std::cout << "Cards remaining in the Deck: " << d1->getDeckSize() << std::endl;
    std::cout << "\nPlayer 1's hand now contains: \n" << *p1->getHand() <<std::endl;

    // Show that the cards have been added to the players orderlist
    std::cout << "Player 1's order list: \n" << *p1->getOrderList() <<std::endl;

    return 0;
}
