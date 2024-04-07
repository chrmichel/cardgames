#pragma once

#include<array>
#include<string_view>

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

    static constexpr char keinRang {'x'};

    static constexpr std::array rangAnzahl {
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3
    };

    static_assert(alleRaenge.size() == anzahlRaenge);
    static_assert(rangSymbol.size() == anzahlRaenge);
    static_assert(rangAnzahl.size() == anzahlRaenge);

    enum Farbe
    {
        rot, schwarz, anzahlFarben
    };

    static constexpr std::array alleFarben {
        rot, schwarz
    };

    static constexpr std::array<std::string_view, anzahlFarben> farbNamen {
        "rot", "schwarz"
    };

    static_assert(farbNamen.size() == anzahlFarben);
    
    Rang rang{};
    Farbe farbe{};

    int wert() const;
    bool operator==(const Karte) const;
    bool operator!=(const Karte) const;
};

constexpr Karte LEEREKARTE {Karte::anzahlRaenge, Karte::anzahlFarben};

class Deck
{
public:
    Deck();
    void mischen();
    Karte verteileKarte();

private:
    static constexpr std::size_t _anzahlKarten {110};
    std::array<Karte, _anzahlKarten> _karten {};
    std::size_t _zaehlIndex {0};
};