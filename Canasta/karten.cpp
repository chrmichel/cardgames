#include<algorithm>
#include<array>
#include<cassert>
#include<iostream>
#include<vector>

#include "karten.h"
#include "Random.h"

using std::array;
using std::vector;

// gibt Wert der Karte zurück
int Karte::wert() const
{
    switch (rang)
    {
        case '?': return 50;
        case 'K':
        case 'Q':
        case 'J':
        case 'Z':
        case '9':
        case '8':
        case '7': return 10;
        case '6':
        case '5':
        case '4': return 5;
        case '2':
        case 'A': return 20;
        case '3': if (farbe == Karte::rot) return 100;
    }
    return 0;
}

//vergleiche zwei Karten
bool Karte::operator==(const Karte k) const
{
    return (rang == k.rang && farbe == k.farbe);
}
bool Karte::operator!=(const Karte k) const
{
    return (rang != k.rang || farbe != k.farbe);
}

// mischt das Deck
void Deck::mischen()
{
    std::shuffle(_karten.begin(), _karten.end(), Random::mt);
    _zaehlIndex = 0;
}

Deck::Deck()
{
    std::size_t counter {0};
    for (auto& r: Karte::alleRaenge)
    {
        for (auto& c: Karte::alleFarben)
        {
            for (int i{0}; i < Karte::rangAnzahl[r]; ++i)
            {
                _karten[counter++] = Karte{r, c};
            }
        }
    }
    mischen();
}

//teile Karte aus
Karte Deck::verteileKarte()
{
    assert(_zaehlIndex < _anzahlKarten &&
        "Deck::dealCard hat keine Karten mehr im Deck");
    return _karten[_zaehlIndex++];
}