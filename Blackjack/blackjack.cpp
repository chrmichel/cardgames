#include "blackjack.h"
#include "Random.h"
#include<algorithm>
#include<array>
#include<cassert>
#include<iostream>

Deck::Deck()
{
    int count {0};
    for (auto rank: Card::allRanks)
    {
        for (auto suit: Card::allSuits)
        {
            m_cards[count++] = Card{rank, suit};
        }
    }
    assert(count == m_size && "Deck::Deck hat nicht genug Karten");
    shuffle();
}

void Deck::shuffle()
{
    std::shuffle(m_cards.begin(), m_cards.end(), Random::mt);
    m_currentIndex = 0;
}

Card Deck::dealCard()
{
    assert(m_currentIndex < m_size && "Deck::dealCard hat keine karten mehr");
    return m_cards[m_currentIndex++];
}

Player::Player(int number, Deck& deck)
{
    drawCard(deck.dealCard()); // 1 card for everyone
    if (number == 0)
        name = "Croupier ";
    else
        name = "Spieler " + std::to_string(number);
        drawCard(deck.dealCard()); //players get a 2nd card
}

void Player::drawCard(const Card card)
{
    //Card card {deck.dealCard()};
    if (card.rank == Card::rank_a)
    {
        m_aceCounter++;
    }
    m_hand.push_back(card);
    score += card.value();
    while (score > Settings::blackjack && m_aceCounter > 0)
    {
        m_aceCounter--;
        score -= 10;
    }
    if (score > Settings::blackjack)
        busted = true;
    if (score == Settings::blackjack)
        blackjack = true;
}

void Player::showHand()
{
    std::cout << name << " hat:";
    for (auto& card: m_hand)
    {
        std::cout << '\t' << card;
    }
    std::cout << "\t(" << score << ")";
    if (busted)
        std::cout << " ÜBERKAUFT    ";
    if (blackjack)
        std::cout << " BLACKJACK";
    std::cout << '\n';
}

bool Player::wantHit()
{
    std::cout << name << ", (z)iehen oder (p)assen? ";
    while(true)
    {
        char ch {getInput<char>()};
        switch (ch)
        {
        case 'z': return true;
        case 'p': return false;
        }
        std::cout << "Ungültiger Buchstabe, versuchs nochmal: ";
    }
}

void Player::reset(Deck& deck)
{
    m_hand = std::vector<Card> {};
    score = 0;
    busted = false;
    blackjack = false;
    drawCard(deck.dealCard());
    if (name != "Croupier ") drawCard(deck.dealCard());
}

Session::Session(int numPlayers): m_numPlayers {numPlayers}
{
    for (int i{0}; i < numPlayers; ++i)
    {
        m_players.push_back(Player{i+1, m_deck});
        m_bets_total.push_back(0.0);
        m_payouts_total.push_back(0.0);
    }
}

void Session::getBets()
{
    for (auto& player: m_players)
    {
        std::cout << player.name << ", nenne deinen Einsatz: ";
        m_bets.push_back(getInput<double>());
    }
}

void Session::showState()
{
    m_dealer.showHand();
    for (auto& player: m_players)
    {
        player.showHand();
    }
    std::cout << '\n';
}

void Session::playerTurn()
{
    for (auto& player: m_players)
    {
        showState();
        while(!player.busted && !player.blackjack && player.wantHit())
        {
            player.drawCard(m_deck.dealCard());
            player.showHand();
        }
    }
}

void Session::dealerTurn()
{
    while(m_dealer.score < Settings::dealerDrawsUntil)
    {
        m_dealer.drawCard(m_deck.dealCard());
    }
}

 double Session::evaluate(std::size_t index)
{
        //lose if busted
        if (m_players[index].busted)
            return 0.0;
        //if dealer and player have blackjack: tie, money back
        if (m_dealer.blackjack && m_players[index].blackjack)
            return m_bets[index];
        //if only dealer has blackjack: lose
        if (m_dealer.blackjack)
            return 0.0;
        //if player has blackjack: win higher amount + bet
        if (m_players[index].blackjack)
            return (Settings::winFactorBlackjack + 1) * m_bets[index];
        //if dealer busted: win
        if (m_dealer.busted)
            return m_bets[index] * 2;
        //no bust, player wins
        if (m_players[index].score > m_dealer.score)
            return m_bets[index] * 2;
        //tie with dealer
        if (m_players[index].score == m_dealer.score)
            return m_bets[index];
        //else lost
            return 0.0;
}

void Session::showResults()
{
    std::cout << "\nSpieler #\tEinsatz\tAuszahlung\n";
    for (int i{0}; i < m_numPlayers; ++i)
    {
        std::cout << i+1 << "\t\t" << m_bets[i] << '\t' << m_payouts[i] << '\n';
    }
}

void Session::play()
{
    getBets();
    playerTurn();
    dealerTurn();
    showState();
    for (int i{0}; i < m_numPlayers; ++i)
    {
        double payout {evaluate(i)};
        m_payouts.push_back(payout);

        m_bets_total[i] += m_bets[i];
        m_payouts_total[i] += m_payouts[i];
    }
    showResults();
}

bool Session::playAgain()
{
    std::cout << "\nNochmal spielen (j/n)? ";
    char ch {};
    while(true)
    {
        ch = getInput<char>();
        if (ch == 'n') return false;
        if (ch == 'j') break;
    }
    //reset bets and payout vectors
    m_bets = std::vector<double> {};
    m_payouts = std::vector<double> {};
    for (auto& player: m_players)
    {
        player.reset(m_deck);
    }
    m_dealer.reset(m_deck);
    return true;
}

void Session::summary()
{
    std::cout << "\nSpieler #\tEinsatz\tAuszahlung\n";
    for (int i{0}; i < m_numPlayers; ++i)
    {
        std::cout << i+1 << "\t\t" << m_bets_total[i] << '\t' << m_payouts_total[i] << '\n';
    }
}