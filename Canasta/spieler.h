#pragma once

#include<vector>

#include "karten.h"
#include "stapel.h"


struct Team
{
    int id {};
    int punkte {0};
    int rote3 {0};
    bool ausgelegt {false};
    bool hatCanasta {false};
    bool hatSchlussGemacht {false};
    std::vector<Auslage> auslagen {};
    std::vector<Spieler*> mitglieder {};
    void punkteBerechnen();
    void reset();
};

class Spieler
{
private:
    int _id;
    Team* _team;
    Deck* _deck;
    Stapel* _stapel;
    std::vector<Karte> _hand {};
    void _sortiereHand();
    bool _kannStapelNehmen();
    int _zaehleAufHand(Karte::Rang);
    std::size_t _karteInHand(Karte);

public:
    Spieler(int, Deck&, Stapel&, Team* team);
    void ablegen(Karte);
    void zieheKarte();
    void nimmStapel();
    Auslage auslegenVonHand(Karte, Karte, Karte);
    Auslage auslegenVonStapel(Karte, Karte);
    bool anbauen(Auslage&, Karte);
    Karte karteAusHand(std::size_t);
    void reset();
    int punkteZaehlen();
    friend void zeigeHand(Spieler&);
};