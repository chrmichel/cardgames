#pragma once

#include<array>

struct Karte
{
    enum Rang
    {
        rang_3, rang_4, rang_5, rang_6, rang_7, rang_8, rang_9,
        rang_10, rang_J, rang_Q, rang_K, rang_A, rang_2, rang_joker,
        anzahlRaenge
    };

    static constexpr std::array alleRaenge {
        rang_3, rang_4, rang_5, rang_6, rang_7, rang_8, rang_9,
        rang_10, rang_J, rang_Q, rang_K, rang_A, rang_2, rang_joker
    };

    static constexpr std::array rangSymbol {
        '3', '4', '5', '6', '7', '8', '9', 'Z', 'J', 'Q', 'K', 'A', '2', '?'
    };

    static constexpr std::array rangAnzahl {
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3
    };

    static_assert(alleRaenge.size() == anzahlRaenge);
    static_assert(rangSymbol.size() == anzahlRaenge);
    static_assert(rangAnzahl.size() == anzahlRaenge);

    enum Farbe
    {
        rot, schwarz, resetFarbe, anzahlFarben
    };

    static constexpr std::array alleFarben {
        rot, schwarz, resetFarbe
    };

    static constexpr std::array<std::string_view, anzahlFarben> farbCodes {
        "\033[0;31m", "\033[0;30;47m", "\033[0m"
    };

    static_assert(alleFarben.size() == anzahlFarben);
    static_assert(farbCodes.size() == anzahlFarben);
    
    Rang rang{};
    Farbe farbe{};

    int wert();
};

class Deck
{
private:
    static constexpr std::size_t _anzahlKarten {110};
    std::array<Karte, _anzahlKarten> _karten {};
    std::size_t _zaehlIndex {0};

public:
    Deck();
    void mischen();
    Karte verteileKarte();
};