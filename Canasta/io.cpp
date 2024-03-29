#include<cstdlib>
#include<iostream>
#include<limits>

#include "karten.h"
#include "logik.h"
#include "spieler.h"

using std::cin;
using std::cout;
using std::istream;
using std::ostream;
using std::string;

void ignoreLine()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

istream& operator>>(istream& in, Karte& karte)
{
    string farbe {};
    char rang {};
    in >> farbe >> rang;
    //Farbe auslesen
    if (farbe.find("rot") != string::npos)
        karte.farbe = Karte::rot;
    else if (farbe.find("schwarz") != string::npos)
        karte.farbe = Karte::schwarz;
    else in.setstate(std::ios_base::failbit);
    //Rang auslesen
    bool rangGefunden {false};
    for (auto& r : Karte::alleRaenge)
    {
        if (rang == Karte::rangSymbol[r])
        {
            karte.rang = r;
            rangGefunden = true;
        }
    }
    if (!rangGefunden) in.setstate(std::ios_base::failbit);
    return in;
}

ostream& operator<<(ostream& out, const Karte& karte)
{
    return out << Karte::farbCodes[karte.farbe] << karte.rang
        << Karte::resetFarbe;
}

void zeigeHand(Spieler& spieler)
{
    for (auto& k: spieler._hand)
        cout << k << ' ';
}

void zeigeAuslage(Auslage& a)
{
    for (auto& k: a._karten)
        cout << k << ' ';
    cout << '\n';
}

void zeigeTisch(Spiel& spiel)
{
    static constexpr Karte rote3 {Karte::rang_3, Karte::rot};

    std::system("clear");
    
    for (std::size_t i{0}; i < spiel._teams.size(); ++i)
    {
        cout << "Team " << i+1 << ":\n";
        for(int j{0}; j < spiel._teams[i].rote3; ++j)
            cout << rote3;
        cout << '\n';
        for (auto& a: spiel._teams[i].auslagen)
            zeigeAuslage(a);

        cout << "\n\n";
    }
}