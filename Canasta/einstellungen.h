#pragma once
namespace Einstellungen
{
    constexpr int anzahlHandkarten {13};
    inline constexpr int auslegenPunkte(int teamPunkte)
    {
        if(teamPunkte >= 3000) return 120;
        if(teamPunkte >= 1500) return 90;
        return 50;
    };
    constexpr int siegPunkte {5000};
    constexpr int canastaPunkte {300};
    constexpr int reinPunkte {500};
    constexpr int schlussPunkte {100};
    constexpr int Rote3Punkte {100};
    constexpr int alleRoten3Punkte {1000};
    constexpr int jokerCanastaPunkte {1000};
    constexpr bool jokerCanasta {true};
    constexpr int anzahlKartenCanasta {7};
}
