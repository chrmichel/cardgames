#pragma once

#include "karten.h"

class Auslage
{
private:
    bool _rein {true};
    int _anzahlJoker {0};
    std::vector<Karte> _karten {};
public:
    bool istCanasta {false};
    Karte::Rang rang {};
    Auslage(Karte::Rang);
    int wert();
    bool anlegen(Karte);
    friend void zeigeAuslage(Auslage&);
};

struct Stapel
{
    bool gesperrt {false};
    std::vector<Karte> karten {};
    Karte letzteKarte();
    void reset();
};