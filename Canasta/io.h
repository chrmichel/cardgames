#pragma once

#include<iostream>

#include "karten.h"
#include "logik.h"
#include "spieler.h"
#include "stapel.h"

//Hilfsfunktionen
void ignoreLine();
void clearScreen();
void nichtMoeglich();
void spielerBeginn(int);
//Operator overloads
std::istream& operator>>(std::istream&, Karte::Rang&);
std::istream& operator>>(std::istream&, Karte&);
std::ostream& operator<<(std::ostream&, const Karte&);
//Ausgaben
void zeigeHand(const Spieler&);
void zeigeAuslage(const Auslage&);
void zeigeTisch(const Spiel&);
void zeigePunkte(const Spiel&);
void hallo();
void tschuess();
//Eingaben
Karte karteAuswaehlen(const Spieler&);
Karte waehleJoker(const Spieler&);
Spieler::AktionAufnehmen waehleAufnehmen();
Spieler::Aktion waehleAktion();
Karte::Rang waehleRang();
int waehleAnzahlTeams();
int waehleSpielerProTeam();
bool nochEineRunde();