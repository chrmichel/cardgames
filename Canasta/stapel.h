#pragma once

#include "karten.h"

class Auslage
{
public:
    //Variablen
    bool rein {true};
    bool istCanasta {false};
    Karte::Rang rang {};
    //Konstruktor
    Auslage(Karte::Rang);
    //Funktionen
    int wert();
    bool anlegen(Karte);
    std::size_t anzahl();
    //Ausgabe
    friend void zeigeAuslage(const Auslage&);

private:
    int _anzahlJoker {0};
    std::vector<Karte> _karten {};
};

struct Stapel
{
    bool gesperrt {false};
    std::vector<Karte> karten {};
    Karte letzteKarte() const;
    void reset();
};