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
    std::vector<Team> _teams {};
    Deck _deck {};
    Stapel _stapel {};

public:
    Spiel(int, int);
    friend void zeigeTisch(Spiel&);

};