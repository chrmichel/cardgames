#include<iostream>
#include<vector>

#include "einstellungen.h"
#include "karten.h"
#include "stapel.h"

Auslage::Auslage(Karte::Rang _rang):
    rang {_rang}
{
}

bool Auslage::anlegen(Karte karte)
{
    //wenn karte gleichen Rang hat wie Auslage
    if (karte.rang == rang)
    {
        _karten.push_back(karte);
    }
    //ansonsten wenn karte ein Joker ist
    else if ((karte.rang == Karte::rang_2 || karte.rang == Karte::rang_joker)
        && _anzahlJoker < Einstellungen::anzahlKartenCanasta / 2)
    {
        _karten.push_back(karte);
        _anzahlJoker++;
        _rein = false;
    }

    if (_karten.size() >= Einstellungen::anzahlKartenCanasta)
    {
        istCanasta = true;
        return true;
    }
    return false;
}

int Auslage::wert()
{
    int summe {0};
    if(istCanasta && _rein) 
    {
        if(Einstellungen::jokerCanasta)
            summe += Einstellungen::jokerCanastaPunkte;
        else
            summe += Einstellungen::reinPunkte;
    }
    else if (istCanasta)
        summe += Einstellungen::canastaPunkte;
    
    for (auto& karte : _karten)
        summe += karte.wert();

    return summe;
}

Karte Stapel::letzteKarte()
{
    return karten[karten.size()-1];
}

void Stapel::reset()
{
    gesperrt = false;
    karten.clear();
}
