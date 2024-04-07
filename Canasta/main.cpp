#include<iostream>

#include "einstellungen.h"
#include "io.h"
#include "karten.h"
#include "logik.h"
#include "spieler.h"
#include "stapel.h"

int main()
{
    hallo();
    Spiel spiel {waehleAnzahlTeams(), waehleSpielerProTeam()};
    int counter {1};
    bool ende {false};
    do {
        spiel.runde(counter);
        counter++;
        zeigePunkte(spiel);
        if(spiel.gewonnen) ende = true;
        else ende = !nochEineRunde();
    } while (!ende);
    tschuess();
}