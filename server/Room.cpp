#include "Room.h"

Room::Room(Player* hrac, char *id) {
    _rivals.push_back(hrac);
    _id[0] = id[2];
    _id[1] = id[3];
    std::cout << "Room was created " << _id[0] << _id[1] << std::endl;
    _state = 0;
    _direction = true;
    _whosTurn = 0;
}

void Room::getId(char *id) {
    id[0] = _id[0];
    id[1] = _id[1];
}

void Room::addPlayer(Player* player) {
    _rivals.push_back(player);
    std::cout << "Player connected to " << _id[0] << _id[1] << std::endl;
}

std::vector<Player*>* Room::getPlayers() {
    return &_rivals;
}

int Room::getState() {
    return _state;
}

void Room::initPile() {
    _pile.push_back(new Card('b', '0'));
    _pile.push_back(new Card('b', '1'));
    _pile.push_back(new Card('b', '2'));
    _pile.push_back(new Card('b', '3'));
    _pile.push_back(new Card('b', '4'));
    _pile.push_back(new Card('b', '5'));
    _pile.push_back(new Card('b', '6'));
    _pile.push_back(new Card('b', '7'));
    _pile.push_back(new Card('b', '8'));
    _pile.push_back(new Card('b', '9'));
    _pile.push_back(new Card('b', 't'));
    _pile.push_back(new Card('b', 't'));
    _pile.push_back(new Card('b', 's'));
    _pile.push_back(new Card('b', 's'));
    _pile.push_back(new Card('b', 'd'));
    _pile.push_back(new Card('b', 'd'));
    _pile.push_back(new Card('b', '1'));
    _pile.push_back(new Card('b', '2'));
    _pile.push_back(new Card('b', '3'));
    _pile.push_back(new Card('b', '4'));
    _pile.push_back(new Card('b', '5'));
    _pile.push_back(new Card('b', '6'));
    _pile.push_back(new Card('b', '7'));
    _pile.push_back(new Card('b', '8'));
    _pile.push_back(new Card('b', '9'));

    _pile.push_back(new Card('r', '0'));
    _pile.push_back(new Card('r', '1'));
    _pile.push_back(new Card('r', '2'));
    _pile.push_back(new Card('r', '3'));
    _pile.push_back(new Card('r', '4'));
    _pile.push_back(new Card('r', '5'));
    _pile.push_back(new Card('r', '6'));
    _pile.push_back(new Card('r', '7'));
    _pile.push_back(new Card('r', '8'));
    _pile.push_back(new Card('r', '9'));
    _pile.push_back(new Card('r', 't'));
    _pile.push_back(new Card('r', 't'));
    _pile.push_back(new Card('r', 's'));
    _pile.push_back(new Card('r', 's'));
    _pile.push_back(new Card('r', 'd'));
    _pile.push_back(new Card('r', 'd'));
    _pile.push_back(new Card('r', '1'));
    _pile.push_back(new Card('r', '2'));
    _pile.push_back(new Card('r', '3'));
    _pile.push_back(new Card('r', '4'));
    _pile.push_back(new Card('r', '5'));
    _pile.push_back(new Card('r', '6'));
    _pile.push_back(new Card('r', '7'));
    _pile.push_back(new Card('r', '8'));
    _pile.push_back(new Card('r', '9'));

    _pile.push_back(new Card('y', '0'));
    _pile.push_back(new Card('y', '1'));
    _pile.push_back(new Card('y', '2'));
    _pile.push_back(new Card('y', '3'));
    _pile.push_back(new Card('y', '4'));
    _pile.push_back(new Card('y', '5'));
    _pile.push_back(new Card('y', '6'));
    _pile.push_back(new Card('y', '7'));
    _pile.push_back(new Card('y', '8'));
    _pile.push_back(new Card('y', '9'));
    _pile.push_back(new Card('y', 't'));
    _pile.push_back(new Card('y', 't'));
    _pile.push_back(new Card('y', 's'));
    _pile.push_back(new Card('y', 's'));
    _pile.push_back(new Card('y', 'd'));
    _pile.push_back(new Card('y', 'd'));
    _pile.push_back(new Card('y', '1'));
    _pile.push_back(new Card('y', '2'));
    _pile.push_back(new Card('y', '3'));
    _pile.push_back(new Card('y', '4'));
    _pile.push_back(new Card('y', '5'));
    _pile.push_back(new Card('y', '6'));
    _pile.push_back(new Card('y', '7'));
    _pile.push_back(new Card('y', '8'));
    _pile.push_back(new Card('y', '9'));

    _pile.push_back(new Card('g', '0'));
    _pile.push_back(new Card('g', '1'));
    _pile.push_back(new Card('g', '2'));
    _pile.push_back(new Card('g', '3'));
    _pile.push_back(new Card('g', '4'));
    _pile.push_back(new Card('g', '5'));
    _pile.push_back(new Card('g', '6'));
    _pile.push_back(new Card('g', '7'));
    _pile.push_back(new Card('g', '8'));
    _pile.push_back(new Card('g', '9'));
    _pile.push_back(new Card('g', 't'));
    _pile.push_back(new Card('g', 't'));
    _pile.push_back(new Card('g', 's'));
    _pile.push_back(new Card('g', 's'));
    _pile.push_back(new Card('g', 'd'));
    _pile.push_back(new Card('g', 'd'));
    _pile.push_back(new Card('g', '1'));
    _pile.push_back(new Card('g', '2'));
    _pile.push_back(new Card('g', '3'));
    _pile.push_back(new Card('g', '4'));
    _pile.push_back(new Card('g', '5'));
    _pile.push_back(new Card('g', '6'));
    _pile.push_back(new Card('g', '7'));
    _pile.push_back(new Card('g', '8'));
    _pile.push_back(new Card('g', '9'));

    _pile.push_back(new Card('s', 'c'));
    _pile.push_back(new Card('s', 'c'));
    _pile.push_back(new Card('s', 'c'));
    _pile.push_back(new Card('s', 'c'));
    _pile.push_back(new Card('s', 'f'));
    _pile.push_back(new Card('s', 'f'));
    _pile.push_back(new Card('s', 'f'));
    _pile.push_back(new Card('s', 'f'));

    shuffle(_pile.begin(), _pile.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
}

void Room::pileToPile() {
    if (_pile.empty()) {
        while (!_discardedCards.empty()) {
            _pile.push_back(_discardedCards.back());
            _pile.pop_back();
        }
        shuffle(_pile.begin(), _pile.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    }
}

Card* Room::getCard() {
    Card* pomKarta = _pile.back();
    _pile.pop_back();
    return pomKarta;
}

void Room::setDiscard(Card* card) {
    _discardedCards.push_back(card);
}

void Room::setState(int state) {
    _state = state;
}

bool Room::getDirection() {
    return _direction;
}

void Room::setDirection(bool direction) {
    _direction = direction;
}

int Room::getTurn() {
    return _whosTurn;
}

void Room::setTurn(int whosTurn) {
    _whosTurn = whosTurn;
}

void Room::deletePlayer(Player* player) {
    Player* tmpPlayer = player;
    int tmpInt = 0;
    while (_rivals.at(tmpInt) != tmpPlayer) {
        tmpInt++;
    }
    _rivals.at(tmpInt) = nullptr;
    _rivals.erase(_rivals.begin() + tmpInt);
}

bool Room::isPlayerHere(Player* player) {
    Player* tmpPlayer = player;
    for (int i; i < _rivals.size(); i++) {
        if (_rivals.at(i) == tmpPlayer) {
            return true;
        }
    }
    return false;
}

Room::~Room() {
    std::cout << "Room was destroyed " << _id[0] << _id[1] << std::endl;
    while (!_rivals.empty()) {
        _rivals.back()->clearHand();
        send(_rivals.back()->getSock(), "43000000", 8, 0);
        _rivals.back() = nullptr;
        _rivals.pop_back();
    }
    while (!_pile.empty()) {
        delete _pile.back();
        _pile.back() = nullptr;
        _pile.pop_back();
    }
    while (!_discardedCards.empty()) {
        delete _discardedCards.back();
        _discardedCards.back() = nullptr;
        _discardedCards.pop_back();
    }
}