#pragma once

#include<vector>

#include "karten.h"
#include "logik.h"
#include "spieler.h"
#include "stapel.h"

class Spieler;
class Spiel;

struct Team
{
    //Variablen
    int id {};
    int punkte {0};
    int rote3 {0};
    bool ausgelegt {false};
    bool hatCanasta {false};
    bool hatSchlussGemacht {false};
    std::vector<Auslage> auslagen {};
    std::vector<Spieler*> mitglieder {};
    std::vector<int> punkteProRunde {};
    //Funktionen
    void punkteBerechnen();
    void reset();
    bool hatAuslage(Karte::Rang);
};

class Spieler
{
public:
    enum AktionAufnehmen
    {
        Ziehen, VonStapelAuslegen, VonStapelAnlegen,
        anzahlAktionenAufnehmen
    };
    enum Aktion
    {
        Auslegen, Anbauen, Ablegen,
        anzahlAktionen
    };
    Spieler(int, Deck*, Stapel*, Team*, Spiel*);
    bool ablegen();
    void nehmen();
    void zieheKarte();
    void nimmStapel();
    bool auslegenVonHand();
    bool auslegenVonStapel();
    void anbauen(Karte);
    bool anbauenVonStapel();
    void anbauenVonHand();
    Karte karteAusHand(std::size_t);
    int punkteZaehlen() const;
    void reset();
    bool runde();
    static constexpr std::array aufnehmAktionen {
        Ziehen, VonStapelAuslegen, VonStapelAnlegen
    };
    static_assert(aufnehmAktionen.size() == anzahlAktionenAufnehmen);
    static constexpr std::array aktionen {
        Auslegen, Anbauen, Ablegen
    };
    static_assert(aktionen.size() == anzahlAktionen);

    friend void zeigeHand(const Spieler&);
    friend Karte karteAuswaehlen(const Spieler&);
    friend Karte waehleJoker(const Spieler&);
    friend AktionAufnehmen waehleAufnehmen();
    friend Aktion waehleAktion();

private:
    //Variablen
    int _id;
    Team* _team;
    Deck* _deck;
    Stapel* _stapel;
    Spiel* _spiel;
    std::vector<Karte> _hand {};
    //Hilfsfunktionen
    void _sortiereHand();
    bool _kannStapelNehmen() const;
    bool _kannVonStapelAuslegen() const;
    bool _hatSchlussGemacht();
    int _zaehleAufHand(Karte::Rang) const;
    std::size_t _karteInHand(Karte) const;
    std::size_t _hatRangAufHand(Karte::Rang) const;
};