#include<array>
#include<cstdlib>
#include<iostream>
#include<limits>
#include<sstream>
#include<string>
#include<string_view>

#include "karten.h"
#include "logik.h"
#include "spieler.h"

using std::cin;
using std::cout;
using std::istream;
using std::ostream;
using std::string;
using std::string_view;

//cin aufräumen
void ignoreLine()
{
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//Terminal leeren
void clearScreen()
{
    std::system("clear");
}

static constexpr std::array<string_view, Karte::anzahlFarben> farbCodes {
    "\033[0;31;47m", "\033[0;30;47m"
};

static constexpr string_view resetFarbe {"\033[0m"};

static_assert(Karte::alleFarben.size() == Karte::anzahlFarben);
static_assert(farbCodes.size() == Karte::anzahlFarben);

istream& operator>>(istream& in, Karte::Rang& rang)
{
    string farbe {};
    char rangSymbol {};
    in >> rangSymbol;
    //Rang auslesen
    bool rangGefunden {false};

    for (auto& r : Karte::alleRaenge)
    {
        if (rangSymbol == Karte::rangSymbol[r])
        {
            rang = r;
            rangGefunden = true;
            break;
        }
    }
    if (!rangGefunden) in.setstate(std::ios_base::failbit);
    return in;
}

istream& operator>>(istream& in, Karte& karte)
{
    string input {};
    ignoreLine();
    std::getline(in >> std::ws, input);
    std::stringstream ss(input);
    std::vector<string> words {};
    bool farbeGefunden {false};
    bool rangGefunden {false};
    for(string w; ss>>w; ) words.push_back(w);
    if (words.size() == 0)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    //leere Karte wenn input x enthält
    if (words[0].find(Karte::keinRang) != string::npos)
    {
        karte = LEEREKARTE;
        return in;
    }
    //Farbe auslesen
    farbeGefunden = false;
    for (const auto& f: Karte::alleFarben)
    {
        std::size_t idx {words[0].find(Karte::farbNamen[f])};
        if (words[0].find(Karte::farbNamen[f]) == 0)
        {
            karte.farbe = f;
            farbeGefunden = true;
            break;
        }
    }
    if(!farbeGefunden)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    //Rang auslesen
    if (words.size() == 1)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    rangGefunden = true;
    for (auto& r : Karte::alleRaenge)
    {
        if (words[1].find(Karte::rangSymbol[r]) != string::npos)
        {
            karte.rang = r;
            rangGefunden = true;
            break;
        }
    }
    if (!rangGefunden) in.setstate(std::ios_base::failbit);
    return in;
}

ostream& operator<<(ostream& out, const Karte& karte)
{
    return out << farbCodes[karte.farbe] << Karte::rangSymbol[karte.rang]
        << resetFarbe;
}

//Drucke "nicht möglich"
void nichtMoeglich()
{
    cout << "Dieser Zug ist gerade leider nicht möglich\n";
}

//zeige Hand von Spieler
void zeigeHand(const Spieler& spieler)
{
    cout << "Deine Hand, Spieler " << spieler._id << ":\n";
    for (const auto& k: spieler._hand)
        cout << k << ' ';
    cout << "\n\n";
}

// zeige Auslage @a
void zeigeAuslage(const Auslage& a)
{
    for (const auto& k: a._karten)
        cout << k << ' ';
    cout << '\n';
}

// räume Terminal und zeige aktuelles Spiel
void zeigeTisch(const Spiel& spiel)
{
    static constexpr Karte rote3 {Karte::rang_3, Karte::rot};
    clearScreen();
    cout << "\nAblagestapel: " << spiel._stapel.letzteKarte();
    cout << "\n\n";
    for (const auto& t : spiel._teams)
    {
        cout << "Team " << t.id << ":\t" << rote3 << ": " << t.rote3;
        cout << "\tCanastas: ";
        for (const auto& a: t.auslagen)
        {
            if(a.istCanasta)
            {
                if(a.rein) cout << Karte{a.rang, Karte::rot};
                else cout << Karte{a.rang, Karte::schwarz};
            }
        }
        cout << '\n';
        for (const auto& a: t.auslagen)
        {
            if(!a.istCanasta)
            {
                zeigeAuslage(a);
            }
        }
        cout << "\n";
    }
}

//lass User Karte eingeben
Karte karteAuswaehlen(const Spieler& spieler)
{
    cout << "Welche Karte? ";
    while(true)
    {
        Karte eingabe {};
        cin >> eingabe;

        if (!cin)
        {
            ignoreLine();
            cout << "Ungültige Eingabe, versuchs noch mal: ";
            continue;
        }

        if(eingabe == LEEREKARTE) return LEEREKARTE;

        if (spieler._karteInHand(eingabe) < spieler._hand.size())
        {
            return eingabe;
        }
        cout << "Diese Karte hast du nicht, versuch's noch mal: ";
    }
}

Karte waehleJoker(const Spieler& spieler)
{
    cout << "Welchen Joker? ";
    while(true)
    {
        Karte::Rang eingabe {};
        cin >> eingabe;

        if (!cin)
        {
            ignoreLine();
            cout << "Ungültige Eingabe, versuchs noch mal: ";
            continue;
        }

        if(eingabe == Karte::keinRang) return LEEREKARTE;
        if(eingabe != Karte::rang_2 && eingabe != Karte::rang_joker)
        {
            cout << "Die gewählte Karte ist kein Joker, versuch's noch mal: ";
            continue;
        }

        if (spieler._hatRangAufHand(eingabe) < spieler._hand.size())
        {
            return Karte {spieler._hand[spieler._hatRangAufHand(eingabe)]};
        }
        cout << "Diese Karte hast du nicht, versuch's noch mal: ";
    }
}

//lass User Rang eingeben
Karte::Rang waehleRang()
{
    cout << "Welchen Kartenrang? ";
    Karte::Rang gewaehlterRang {};
    cin >> gewaehlterRang;
    return gewaehlterRang;
}

Karte::Rang waehleAuslage(Team& team)
{
    Karte::Rang gewaehlterRang {waehleRang()};
    while(true)
    {
        cin >> gewaehlterRang;
        //Abbruch
        if (gewaehlterRang == Karte::anzahlRaenge)
            return gewaehlterRang;
        //prüfe ob Auslage vorhanden ist
        for (auto& a: team.auslagen)
        {
            if (a.rang == gewaehlterRang)
                return a.rang;
        }
        cout << "Diese Reihe hat dein Team nicht, versuch's nochmal: ";
    }
}

//lass Spieler eine Aktion auswählen
Spieler::AktionAufnehmen waehleAufnehmen()
{
    static constexpr std::array<string_view, Spieler::anzahlAktionenAufnehmen>
    aufnehmAktionen {
        "Karte ziehen", "Stapel nehmen und auslegen",
        "Stapel nehmen und an Reihe anlegen"
    };
    static_assert(aufnehmAktionen.size() == Spieler::anzahlAktionenAufnehmen);
    for (const auto& aktion: Spieler::aufnehmAktionen)
    {
        cout << aktion + 1 << ":\t" << aufnehmAktionen[aktion] << '\n';
    }

    while(true)
    {
        cout << "Wähle eine Aktion: ";
        std::size_t input{};
        cin >> input;

        if(!cin)
        {
            ignoreLine();
            continue;
        }
        if(input <= Spieler::anzahlAktionenAufnehmen && input > 0)
            return static_cast<Spieler::AktionAufnehmen>(input - 1);
        else
            cout << "Keine gültige Aktion!\n";
    }
}

//lass Spieler eine Aktion auswählen
Spieler::Aktion waehleAktion()
{
    static constexpr std::array<string_view, Spieler::anzahlAktionen>
    aktionen {
        "neue Reihe auslegen", "an Reihe anlegen",
        "eine Karte ablegen"
    };
    static_assert(aktionen.size() == Spieler::anzahlAktionen);
    for (const auto& aktion: Spieler::aktionen)
    {
        cout << aktion + 1 << ":\t" << aktionen[aktion] << '\n';
    }

    while(true)
    {
        cout << "Wähle eine Aktion: ";
        std::size_t input{};
        cin >> input;

        if(!cin)
        {
            ignoreLine();
            continue;
        }
        if(input <= Spieler::anzahlAktionenAufnehmen && input > 0)
            return static_cast<Spieler::Aktion>(input - 1);
        else
            cout << "Keine gültige Aktion!\n";
    }
}

void zeigePunkte(const Spiel& s)
{
    clearScreen();
    for(const auto& t: s._teams)
    {
        cout << "Team " << t.id << ":\t";
        for (auto p: t.punkteProRunde)
            cout << p << '\t';
        cout << "|\t" << t.punkte << '\n';
    }
    cin.get();
}

//zeigt an dass Spieler "nummer" dran ist
void spielerBeginn(int nummer)
{
    clearScreen();
    cout << "Spieler " << nummer << ", du bist dran!\n";
    cin.get();
}

void hallo()
{
    clearScreen();
    cout << "Willkommen beim digitalen Canasta!\n\n";
}

void tschuess()
{
    clearScreen();
    cout << "Danke für's Spielen, kommt bald wieder!\n";
}

int waehleAnzahlTeams()
{
    cout << "Wie viele Teams wollen spielen? ";
    while(true)
    {
        int input {};
        cin >> input;
        if(!cin || input < 0)
        {
            cin.clear();
            ignoreLine();
            cout << "Das geht nicht, versuch's nochmal: ";
            continue;
        }
        return input;
    }
}

int waehleSpielerProTeam()
{
    cout << "Wie viele Spieler seid ihr pro Team? ";
    while(true)
    {
        int input {};
        cin >> input;
        if(!cin || input < 0)
        {
            cin.clear();
            ignoreLine();
            cout << "Das geht nicht, versuch's nochmal: ";
            continue;
        }
        return input;
    }
}

bool nochEineRunde()
{
    cout << "Noch eine Runde (j/n)? ";
    while(true)
    {
        char input{};
        cin >> input;
        if(!cin)
        {
            cin.clear();
            ignoreLine();
            cout << "Das geht nicht, versuch's nochmal: ";
            continue;
        }
        switch(input)
        {
            case 'j':
            case 'J':
            case 'y':
            case 'Y':
                ignoreLine();
                return true;
            case 'n':
            case 'N':
                ignoreLine();
                return false;
            default:
                cin.clear();
                ignoreLine();
                cout << "Das war keine gültige Eingabe, versuch's nochmal: ";
                continue;
        }
    }
}