#include<vector>

#include "einstellungen.h"
#include "io.h"
#include "karten.h"
#include "logik.h"
#include "spieler.h"
#include "stapel.h"

Spiel::Spiel(int anzahlTeams, int spielerProTeam)
    : _anzahlTeams {anzahlTeams}
    , _spielerProTeam {spielerProTeam}
{
    _deck = Deck();
    _stapel = Stapel();
    _anzahlSpieler = anzahlTeams * spielerProTeam;

    //erstelle Teams
    for(int t{1}; t <= anzahlTeams; ++t)
        _teams.push_back(Team{t});

    // erstelle Spieler und ordne sie Teams zu
    for(int s{0}; s < _anzahlSpieler; ++s)
    {
        int teamIndex {s % anzahlTeams};
        Team* t_ptr {&_teams[0]};
        ;
        _spieler.push_back(
            Spieler(s+1, &_deck, &_stapel, (t_ptr + teamIndex), this)
        );
    }
}

void Spiel::runde(int nummer)
{
    //mische Deck
    _deck.mischen();
    //verteile Karten
    for(int k{1}; k <= Einstellungen::anzahlHandkarten; ++k)
    {
        for(auto& s: _spieler)
            s.zieheKarte();
    }
    do {
        _stapel.karten.push_back(_deck.verteileKarte());
    } while(
        _stapel.letzteKarte().rang == Karte::rang_2
        || _stapel.letzteKarte().rang == Karte::rang_3
        || _stapel.letzteKarte().rang == Karte::rang_joker
    );
    //berechne welcher Spieler die Runde beginnt
    std::size_t counter {
        static_cast<std::size_t>((nummer-1) % _anzahlSpieler)
    };
    //Runde spielen
    while(true)
    {
        spielerBeginn(counter+1);
        if(_spieler[counter].runde()) break;
        counter++;
        counter = counter % _anzahlSpieler;
    }
    //auswerten und reset für nächste Runde
    for (auto& t: _teams)
    {
        t.punkteBerechnen();
        if(t.punkte >= Einstellungen::siegPunkte) gewonnen = true;
        t.reset();
    }
}