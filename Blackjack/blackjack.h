#pragma once

#include<array>
#include<cassert>
#include<iostream>
#include<limits>
#include<string_view>
#include<vector>

struct Card
{
    enum Rank
    {
        rank_a, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8,
        rank_9, rank_10, rank_j, rank_q, rank_k, num_ranks
    };

    static constexpr std::array allRanks
    {
        rank_a, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8,
        rank_9, rank_10, rank_j, rank_q, rank_k
    };
    static_assert(allRanks.size() == num_ranks);

    static constexpr std::array rankValues
    {
        11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10
    };
    static_assert(rankValues.size() == num_ranks);

    static constexpr std::array rankLetters 
    {
        'A', '2', '3', '4', '5', '6', '7',
        '8', '9', 'Z', 'J', 'Q', 'K'
    };
    static_assert(rankLetters.size() == num_ranks);

    enum Suit
    {
        suit_herz, suit_karo, suit_pik, suit_kreuz, num_suits
    };

    static constexpr std::array allSuits
    {
        suit_herz, suit_karo, suit_pik, suit_kreuz
    };
    static_assert(allSuits.size() == num_suits);

    static constexpr std::array<std::string_view, num_suits> suitNames
    {
        " Herz", " Karo", "  Pik", "Kreuz"
    };
    static_assert(suitNames.size() == num_suits);

    Rank rank {};
    Suit suit {};

    inline friend std::ostream& operator<< (std::ostream& out, const Card& c)
    {
        return out << Card::suitNames[c.suit] << '-'
            << Card::rankLetters[c.rank];
    }

    inline friend std::istream& operator>> (std::istream& in, Card& card)
    {
        //not yet defined
        return in;
    }

    inline int value() const
    {
        return rankValues[rank];
    }
};

class Deck
{
private:
    static constexpr std::size_t m_size {Card::num_ranks * Card::num_suits};
    std::size_t m_currentIndex {0};
    std::array<Card, m_size> m_cards {};

public:
    Deck();
    void shuffle();
    Card dealCard();
};

class Player
{
private:
    std::vector<Card> m_hand {};
    int m_aceCounter {0};
public:
    std::string name {};
    int score {0};
    bool busted {false};
    bool blackjack {false};
    Player(int number, Deck& deck);
    void drawCard(const Card);
    void showHand();
    bool wantHit();
    void reset(Deck& deck);
};

namespace Settings
{
    constexpr int blackjack {21};
    constexpr int dealerDrawsUntil {17};
    constexpr double winFactorBlackjack {1.5};
}

class Session
{
private:
    int m_numPlayers {};
    std::vector<Player> m_players {};
    std::vector<double> m_bets {};
    std::vector<double> m_payouts {};
    std::vector<double> m_bets_total {};
    std::vector<double> m_payouts_total {};
    Deck m_deck {Deck {}};
    Player m_dealer {Player {0, m_deck}};
public:
    Session(int numPlayers);
    void getBets();
    void showState();
    void playerTurn();
    void dealerTurn();
    double evaluate(std::size_t);
    void showResults();
    void play();
    void summary();
    bool playAgain();
};

template <typename T>
inline T getInput ()
{
    while(true)
    {
        T input {};
        std::cin >> input;

        if (!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        return input;
    }
}