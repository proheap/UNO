#include "Player.h"

Player::Player(int sock) {
    _sock = sock;
    _room = nullptr;
    bool _uno = false;
    bool _roundPassed = false;
    std::cout << "Player was created " << _sock << std::endl;
}

int Player::getSock() {
    return _sock;
}

void Player::setRoom(void* room) {
    _room = room;
}

void* Player::getRoom() {
    return _room;
}

void Player::addCard(Card* card) {
    _hand.push_back(card);
}

void Player::clearHand() {
    while (!_hand.empty()) {
        delete _hand.back();
        _hand.back() = nullptr;
        _hand.pop_back();
    }
    _room = nullptr;
}

Card* Player::getCard(int i) {
    Card* pomKarta = _hand.at(i);
    _hand.erase(_hand.begin() + i);
    return pomKarta;
}

int Player::getCardsNumber() {
    return static_cast<int> (_hand.size());
}

bool Player::getUno() {
    return _uno;
}

void Player::setUno(bool uno) {
    _uno = uno;
}

void Player::setRoundPassed(bool roundPassed) {
    _roundPassed = roundPassed;
}

bool Player::getRoundPassed() {
    return _roundPassed;
}

Player::~Player() {
    std::cout << "Player was destoyed " << _sock << std::endl;
    while (!_hand.empty()) {
        delete _hand.back();
        _hand.back() = nullptr;
        _hand.pop_back();
    }
    _room = nullptr;
}