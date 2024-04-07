#pragma once

#include<vector>

#include "einstellungen.h"
#include "karten.h"
#include "spieler.h"
#include "stapel.h"

class Spiel
{
private:
    int _spielerProTeam {};
    int _anzahlTeams {};
    int _anzahlSpieler {};
    std::vector<Team> _teams {};
    std::vector<Spieler> _spieler {};
    Deck _deck {};
    Stapel _stapel {};

public:
    Spiel(int, int);
    friend void zeigeTisch(const Spiel&);
    friend void zeigePunkte(const Spiel&);
    void runde(int);
    bool gewonnen {false};
};