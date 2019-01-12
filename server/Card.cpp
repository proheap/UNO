#include "Card.h"

Card::Card(char color, char symbol) {
    _cardColor = color;
    _cardSymbol = symbol;
}

char Card::getColor() {
    return _cardColor;
}

char Card::getSymbol() {
    return _cardSymbol;
}

Card::~Card() {
}
