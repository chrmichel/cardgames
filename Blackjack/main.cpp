#include "blackjack.h"

#include<array>
#include<iostream>
#include<limits>
#include<vector>

int setup()
{
    std::cout << "Willkommen im Blackjack-Kasino! Wie viele Spieler seid ihr? ";
    return getInput<int>();
}

void outro()
{
    std::cout << "Danke fürs Spielen! Kommen Sie bald wieder!\n\n";
}

int main()
{
    int numPlayers {setup()};
    if (!numPlayers)
    {
        outro();
        return 0;
    }

    Session s {numPlayers};
    do
    {
        s.play();
    } while (s.playAgain());
    s.summary();
    outro();
    return 0;
    
}