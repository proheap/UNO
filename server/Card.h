#pragma once

#include <string>

class Card {
private:
    char _cardColor;
    char _cardSymbol;
public:
    Card(char color, char symbol);
    ~Card();
    char getColor();
    char getSymbol();
};

