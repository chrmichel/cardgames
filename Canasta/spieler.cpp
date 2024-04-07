#include<algorithm>
#include<iostream>
#include<vector>

#include "einstellungen.h"
#include "io.h"
#include "karten.h"
#include "logik.h"
#include "spieler.h"
#include "stapel.h"

Spieler::Spieler(
    int nummer, Deck* deck, Stapel* stapel, Team* team, Spiel* s
): _id {nummer}, _deck {deck}, _stapel {stapel},
    _team {team}, _spiel {s}
{
    team->mitglieder.push_back(this);
}

//sortiere Handkarten
void Spieler::_sortiereHand()
{
    std::sort(_hand.begin(), _hand.end(), [](auto& k1, auto& k2){
        if (k1.rang == k2.rang) return k1.farbe < k2.farbe;
        return k1.rang < k2.rang;
    });
}

//zählt, wie oft der Spieler diesen Rang hat
int Spieler::_zaehleAufHand(Karte::Rang rang) const
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

//ziehe Karte vom Deck, falls rote 3 nochmal ziehen
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

//entfernt Karte aus Hand, gibt diese Karte zurück
Karte Spieler::karteAusHand(std::size_t index)
{
    Karte karte {_hand[index]};
    _hand.erase(_hand.begin()+index);
    return karte;
}

//true wenn Spieler den Stapel nehmen kann
bool Spieler::_kannStapelNehmen() const
{
    //kann nicht nehmen wenn nur eine Karte
    if (_stapel->karten.size() <= 1) return false;

    Karte letzteKarte {_stapel->letzteKarte()};
    if (!Einstellungen::jokerCanasta)
    {
        if (letzteKarte.rang == Karte::rang_2
            || letzteKarte.rang == Karte::rang_joker)
            return false;
    }
    //wenn nicht gesperrt, kann nehmen falls letzte Karte
    // zu offener Reihe passt
    if (!_stapel->gesperrt)
    {
        for (auto& a: _team->auslagen)
        {
            if (a.rang == letzteKarte.rang && !a.istCanasta)
                return true;
        }
    }
    //kann nehmen wenn Spieler zwei gleiche zum Auslegen hat
    if (_zaehleAufHand(letzteKarte.rang) >= 2) return true;
    return false;
}

//true, wenn der Spieler mit dem Stapel auslegen kann
bool Spieler::_kannVonStapelAuslegen() const
{
    //kann nicht nehmen wenn nur eine Karte
    if (_stapel->karten.size() <= 1) return false;
    if (!Einstellungen::jokerCanasta)
    {
        Karte k {_stapel->letzteKarte()};
        if (k.rang == Karte::rang_2 || k.rang == Karte::rang_joker)
            return false;
    }
    //kann nehmen wenn Spieler zwei gleiche zum Auslegen hat
    if (_zaehleAufHand(_stapel->letzteKarte().rang) >= 2) return true;
    return false;
}

//Spieler nimmt gesamten Stapel auf
void Spieler::nimmStapel()
{
    for(auto& k: _stapel->karten)
    {
        //falls rote 3 im Stapel, zum Team zählen
        if(k.rang == Karte::rang_3 && k.farbe==Karte::rot)
            _team->rote3++;
        //alles andere auf die Hand
        else
            _hand.push_back(k);
    }
    //stapel ist jetzt leer
    _stapel->karten.clear();
    _sortiereHand();
}

//prüft, ob der Spieler diese Karte hat, gibt Index der Karte zurück
std::size_t Spieler::_karteInHand(Karte karte) const
{
    for (std::size_t i {0}; i < _hand.size();++i)
        if (karte.rang == _hand[i].rang && karte.farbe == _hand[i].farbe)
            return i;
    
    return static_cast<std::size_t>(-1);
}
//prüft, ob der Spieler eine Karte mit diesem Rang hat, gibt Index der ersten Karte zurück
std::size_t Spieler::_hatRangAufHand(Karte::Rang rang) const
{
    for (std::size_t i {0}; i < _hand.size();++i)
        if (rang == _hand[i].rang)
            return i;
    
    return static_cast<std::size_t>(-1);
}

//legt eine Karte auf den Stapel ab, true falls Spieler
//keine Karten mehr hat
bool Spieler::ablegen()
{
    //wähle Karte aus
    Karte karte {karteAuswaehlen(*this)};

    std::size_t index {_karteInHand(karte)};
    _stapel->karten.push_back(karteAusHand(index));
    return (_hand.size() == 0);
}

//lege Karte an bestehende Reihe an
void Spieler::anbauen(Karte karte)
{
    Auslage* a_ptr {nullptr};
    Karte::Rang gewaehlterRang {};
    if(gewaehlterRang == Karte::anzahlRaenge) return;
    //falls Karte Joker ist fragen welche Auslage
    if (karte.rang == Karte::rang_2 || karte.rang == Karte::rang_joker)
        gewaehlterRang = waehleRang();
    //sonst automatisch passende suchen
    else
        gewaehlterRang = karte.rang;
    //finde passende Auslage, zeige darauf
    for (auto& a: _team->auslagen)
    {
        if (a.rang == gewaehlterRang)
        {
            a_ptr = &a;
            break;
        }
    }
    if(a_ptr == nullptr) return;
    if(a_ptr->anlegen(karte))
    {
        _team->hatCanasta = true;
    }
}

//baue letzte Karte vom Stapel an, nimm restlichen Stapel
//true falls erfolgreich
bool Spieler::anbauenVonStapel()
{
    if(!_kannStapelNehmen()) return false;
    
    Karte oberste {_stapel->letzteKarte()};
    anbauen(oberste);
    _stapel->karten.pop_back();

    nimmStapel();
    _sortiereHand();   

    return true;
}

//baue Handkarte an
void Spieler::anbauenVonHand()
{
    Karte::Rang rang {waehleRang()};
    //wenn natürliche Karte, alle anbauen
    if (rang != Karte::rang_2 && rang != Karte::rang_joker)
    {
        if(!_team->hatAuslage(rang)) return;
        while(_hatRangAufHand(rang) < _hand.size())
            anbauen(karteAusHand(_hatRangAufHand(rang)));
    }
    //bei Jokern nur einen anbauen
    else 
        anbauen(karteAusHand(_hatRangAufHand(rang)));
}

bool Spieler::auslegenVonHand()
{
    Karte::Rang rang {waehleRang()};
    if (rang == Karte::rang_3 || rang == Karte::anzahlRaenge) 
        return false;
    if (!Einstellungen::jokerCanasta)
    {
        if (rang == Karte::rang_2 || rang == Karte::rang_joker)
            return false;
    }
    Auslage a {rang};
    int anzahl {_zaehleAufHand(rang)};
    if (anzahl >= 3)
    {
        std::vector<std::size_t> indices {};
        std::size_t handSize {_hand.size()};
        for(std::size_t i {handSize - 1}; i < handSize; --i)
        {
            if (_hand[i].rang == a.rang)
                indices.push_back(i);
        }
        for(auto& i: indices)
            a.anlegen(karteAusHand(i));
        _team->auslagen.push_back(a);
        return true;
    }
    if(anzahl == 2)
    {
        if(_zaehleAufHand(Karte::rang_joker) == 0) return false;
        Karte joker {waehleJoker(*this)};
        if(joker == LEEREKARTE) return false;
        std::vector<std::size_t> indices {_karteInHand(joker)};
        std::size_t handSize {_hand.size()};
        for(std::size_t i {handSize - 1}; i < handSize; --i)
        {
            if (_hand[i].rang == a.rang)
                indices.push_back(i);
        }
        for(auto& i: indices)
            a.anlegen(karteAusHand(i));
        _team->auslagen.push_back(a);
        return true;
    }
    return false;
}

bool Spieler::auslegenVonStapel()
{
    if(!_kannVonStapelAuslegen()) return false;
    Karte::Rang rang {_stapel->letzteKarte().rang};
    if (rang == Karte::rang_3) return false;
    if (!Einstellungen::jokerCanasta)
    {
        if (rang == Karte::rang_2 || rang == Karte::rang_joker)
            return false;
    }
    Auslage a {rang};
    int anzahl {_zaehleAufHand(rang)};
    if (anzahl >= 2)
    {
        a.anlegen(_stapel->letzteKarte());
        _stapel->karten.pop_back();
        std::vector<std::size_t> indices {};
        std::size_t handSize {_hand.size()};
        for(std::size_t i {handSize - 1}; i < handSize; --i)
        {
            if (_hand[i].rang == a.rang)
                indices.push_back(i);
        }
        for(auto& i: indices)
            a.anlegen(karteAusHand(i));
        _team->auslagen.push_back(a);
        nimmStapel();
        return true;
    }
    return false;
}

void Spieler::reset()
{
    _hand.clear();
}

//zählt Restwert auf der Hand
int Spieler::punkteZaehlen() const
{
    int punkte {0};
    for (const auto& k: _hand)
        punkte += k.wert();
    return punkte;
}

bool Spieler::_hatSchlussGemacht()
{
    if (_hand.size() == 0)
    {
        _team->hatSchlussGemacht = true;
        return true;
    }
    return false;
}

void Spieler::nehmen()
{
    zeigeTisch(*_spiel);
    zeigeHand(*this);
    bool ausgefuehrt {false};
    do
    {
        AktionAufnehmen a1 {waehleAufnehmen()};
        switch (a1)
        {
        case Ziehen: 
            zieheKarte();
            ausgefuehrt = true;
            zeigeTisch(*_spiel);
            zeigeHand(*this);
            break;
        case VonStapelAuslegen:
            if(auslegenVonStapel())
            {
                ausgefuehrt = true;
                zeigeTisch(*_spiel);
                zeigeHand(*this);
            }
            else nichtMoeglich();
            break;
        case VonStapelAnlegen: 
            if(anbauenVonStapel())
            {
                ausgefuehrt = true;
                zeigeTisch(*_spiel);
                zeigeHand(*this);
            }
            else nichtMoeglich();
            break;
        }
    } while(!ausgefuehrt);
}

bool Spieler::runde()
{
    nehmen();
    bool beendet {_hatSchlussGemacht()};
    do
    {
        zeigeTisch(*_spiel);
        zeigeHand(*this);
        Aktion a {waehleAktion()};
        switch(a)
        {
            case Auslegen:
                auslegenVonHand();
                beendet = _hatSchlussGemacht();
                break;
            case Anbauen:
                anbauenVonHand();
                beendet = _hatSchlussGemacht();
                break;
            case Ablegen:
                ablegen();
                beendet = true;
                break;
        }
    } while (!beendet);
    
    zeigeTisch(*_spiel);
    zeigeHand(*this);
    return _hatSchlussGemacht();
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
    punkteProRunde.push_back(rundenPunkte);
}

bool Team::hatAuslage(Karte::Rang rang)
{
    for (const auto& a: auslagen)
    {
        if (a.rang == rang) return true;
    }
    return false;
}