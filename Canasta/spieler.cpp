#include<algorithm>
#include<iostream>
#include<vector>

#include "einstellungen.h"
#include "karten.h"
#include "spieler.h"
#include "stapel.h"

Spieler::Spieler(int nummer, Deck& deck, Stapel& stapel, Team* team)
    : _id {nummer}, _deck {&deck}, _stapel {&stapel}, _team {team}
{
    team->mitglieder.push_back(this);
}

void Spieler::_sortiereHand()
{
    std::sort(_hand.begin(), _hand.end(), [](auto& k1, auto& k2){
        if (k1.rang == k2.rang) return k1.farbe < k2.farbe;
        return k1.rang < k2.rang;
    });
}

int Spieler::_zaehleAufHand(Karte::Rang rang)
{
    int anzahl {0};
    for(const auto& k: _hand)
    {
        if (k.rang == rang) anzahl++;
        if(k.rang == Karte::rang_2 && rang == Karte::rang_joker) anzahl++;
        if(k.rang == Karte::rang_joker && rang == Karte::rang_2) anzahl++;
    }
    return anzahl;
}

void Spieler::zieheKarte()
{
    Karte karte {_deck->verteileKarte()};
    if (karte.rang == Karte::rang_3 && karte.farbe == Karte::rot)
    {
        _team->rote3++;
        zieheKarte();
    }
    else
    {
        _hand.push_back(karte);
        _sortiereHand();
    }
}

Karte Spieler::karteAusHand(std::size_t index)
{
    Karte karte {_hand[index]};
    _hand.erase(_hand.begin()+index);
    return karte;
}

bool Spieler::_kannStapelNehmen()
{
    Karte letzteKarte {_stapel->letzteKarte()};
    if (!_stapel->gesperrt)
    {
        for (auto& a: _team->auslagen)
        {
            if (a.rang == letzteKarte.rang && !a.istCanasta)
                return true;
        }
    }
    int passendeAufHand {_zaehleAufHand(letzteKarte.rang)};
    if (passendeAufHand >= 2) return true;
    return false;
}

void Spieler::nimmStapel()
{
    for(auto& k: _stapel->karten)
    {
        if(k.rang == Karte::rang_3 && k.farbe==Karte::rot)
            _team->rote3++;
        else
            _hand.push_back(k);
    }
    _stapel->karten.clear();
}

std::size_t Spieler::_karteInHand(Karte karte)
{
    for (std::size_t i {0}; i < _hand.size();++i)
        if (karte.rang == _hand[i].rang && karte.farbe == _hand[i].farbe)
            return i;
    
    return static_cast<std::size_t>(-1);
}

void Spieler::ablegen(Karte karte)
{
    std::size_t index {_karteInHand(karte)};
    if (index < _hand.size())
    {
        _stapel->karten.push_back(karte);
        _hand.erase(_hand.begin()+index);
        return;
    }
}

bool Spieler::anbauen(Auslage& a, Karte karte)
{
    std::size_t index {_karteInHand(karte)};
    if (index < _hand.size())
    {
        if(a.anlegen(karte))
        {
        _hand.erase(_hand.begin()+index);
        return true;
        }
    }
}

Auslage Spieler::auslegenVonHand(Karte k1, Karte k2, Karte k3)
{
    Auslage ungueltig {Karte::anzahlRaenge};
    Karte::Rang rang {k1.rang};
    if (rang == Karte::rang_3) return ungueltig;
    if (!Einstellungen::jokerCanasta)
    {
        if (rang == Karte::rang_2 || rang == Karte::rang_joker)
            return ungueltig;
    }
    Auslage a {rang};
    if(a.anlegen(k2) && a.anlegen(k3))
        return a;

    return ungueltig;
}

Auslage Spieler::auslegenVonStapel(Karte k2, Karte k3)
{
    Auslage ungueltig {Karte::anzahlRaenge};
    Karte k1 {_stapel->letzteKarte()};
    Karte::Rang rang {k1.rang};
    if (rang == Karte::rang_3) return ungueltig;
    if (!Einstellungen::jokerCanasta)
    {
        if (rang == Karte::rang_2 || rang == Karte::rang_joker)
            return ungueltig;
    }
    Auslage a {rang};
    if(a.anlegen(k2) && a.anlegen(k3))
        return a;

    return ungueltig;
}

void Spieler::reset()
{
    _hand.clear();
}

int Spieler::punkteZaehlen()
{
    int punkte {0};
    for (auto& k: _hand)
        punkte += k.wert();
}

void Team::reset()
{
    rote3 = 0;
    ausgelegt = false;
    hatCanasta = false;
    auslagen.clear();
    for (auto& spieler: mitglieder)
        spieler->reset();
}

void Team::punkteBerechnen()
{
    int rote3Punkte {rote3 * Einstellungen::Rote3Punkte};
    if (rote3 == 4) rote3Punkte = Einstellungen::alleRoten3Punkte;

    int auslagenPunkte {0};
    for (auto& a: auslagen)
            auslagenPunkte += a.wert();
    
    int schlussPunkte {0};
    if (hatSchlussGemacht)
        schlussPunkte = Einstellungen::schlussPunkte;
    
    if(!hatCanasta)
    {
        rote3Punkte *= -1;
        auslagenPunkte *= -1;
    }

    int handMinus {0};
    for (auto& spieler: mitglieder)
        handMinus += spieler->punkteZaehlen();
    
    int rundenPunkte {
        rote3Punkte + auslagenPunkte + schlussPunkte - handMinus
    };
    punkte += rundenPunkte;

}