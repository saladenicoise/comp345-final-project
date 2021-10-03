// Driver file for Cards 

// This is the application file includes the main funtion,
// which demonstrates the classes Deck, Hand and Card

// This file uses identifiers such as Deck and hand, which are declared in the Cards.h file. 
#include "Cards.h"
using namespace std;

int main( )
{
    // Create a deck of 30 rand cards + 1 of each type, so 35
    Deck d1(30);

    Hand h1 = Hand();

    std::cout << "Deck contains:\n" << d1 << "Deck size: " << d1.getDeckSize()<< std::endl;
    std::cout << "Drawing from the Deck:\n" << std::endl;
        
    for (int i = 0; i < 5; i++) {
        d1.draw(h1);
    }
    
    std::cout << "Cards remaining in the Deck: " << d1.getDeckSize() << std::endl;


    std::cout << "\nPlayers hand contains: \n" << h1 <<std::endl;
    

    return 0;
}