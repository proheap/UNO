#include "Server.h"

char randomColor() {
    int randNumber = rand() % 4;
    switch (randNumber) {
        case 0:
            return 'r';
        case 1:
            return 'g';
        case 2:
            return 'y';
        case 3:
            return 'b';
    }
}

Server::Server(int port) {
    int sock_option = 1;
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(port);
    _port = port;
    //nastavy typ socketu
    if ((_mainSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("_mainSocket = socket error");
        exit(EXIT_FAILURE);
    }
    //nastavy viac pripojeni cez socket
    if (setsockopt(_mainSocket, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_option, sizeof (sock_option)) < 0) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }
    //priradi adresu
    if (bind(_mainSocket, (struct sockaddr *) &_address, sizeof (_address)) < 0) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    //caka na spojenie
    std::cout << "Listening on port " << PORT << std::endl;
    if (listen(_mainSocket, 3) < 0) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }
}

void Server::recvCycle() {
    int i, maxsd, sd, activity, newSocket, valRead, id, error_code;
    char buffer[9];
    struct sockaddr_in pomAddress;
    int address_length = sizeof (pomAddress);
    fd_set readfds;
    Room* tmpRoom;
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(_mainSocket, &readfds);
        maxsd = _mainSocket;
        for (i = 0; i < _players.size(); i++) {
            sd = _players.at(i)->getSock();
            FD_SET(sd, &readfds);
            if (sd > maxsd) {
                maxsd = sd;
            }
        }
        activity = select(maxsd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            perror("select error");
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(_mainSocket, &readfds)) {
            if ((newSocket = accept(_mainSocket, (struct sockaddr *) &pomAddress, (socklen_t*) & address_length)) < 0) {
                perror("accept error");
                exit(EXIT_FAILURE);
            }
            std::cout << "New connection" << newSocket << inet_ntoa(pomAddress.sin_addr) << ntohs(pomAddress.sin_port) << std::endl;
            _players.push_back(new Player(newSocket));
        }
        int hraci_size = _players.size();
        for (int i = 0; i < hraci_size; i++) {
            sd = _players.at(i)->getSock();
            if (FD_ISSET(sd, &readfds)) {
                //Niekto sa odpojil
                if ((valRead = read(sd, buffer, 8)) == 0) {
                    getpeername(sd, (struct sockaddr*) &pomAddress, (socklen_t*) & address_length);
                    std::cout << "Host disconnected" << inet_ntoa(pomAddress.sin_addr) << ntohs(pomAddress.sin_port) << std::endl;
                    std::unique_lock<std::mutex> lck(_mutex);
                    int spravy_size = _messages.size();
                    for (int j = 0; j < spravy_size; j++) {
                        if (_messages.at(j)->getPlayer()->getSock() == sd) {
                            delete (_messages.at(j));
                            _messages.erase(_messages.begin() + j);
                            j--;
                            spravy_size--;
                        }
                    }
                    if (_players.at(i)->getRoom() != nullptr) {
                        tmpRoom = (Room*) _players.at(i)->getRoom();
                        tmpRoom->deletePlayer(_players.at(i));
                        deleteRoom(tmpRoom);
                        delete tmpRoom;
                        tmpRoom = nullptr;
                    }
                    delete (_players.at(i));
                    _players.erase(_players.begin() + i);
                    close(sd);
                    i--;
                    hraci_size--;
                    lck.unlock();
                    //prisla sprava
                } else {
                    buffer[valRead] = '\0';
                    std::unique_lock<std::mutex> lck(_mutex);
                    _messages.push_back(new Message(_players.at(i), buffer));
                    _newMessage.notify_all();
                    lck.unlock();
                }
            }
        }

    }
}

void Server::sendCycle() {
    char tmpColor;
    char tmpSymbol;
    char buffer[9] = "00000000";
    char buffer2[9] = "00000000";
    Card *tmpCard;
    Card *tmpCard2;
    Player *tmpPlayer;
    int i, j, tmpInt, tmpInt2;
    char tmpChar[2];
    Room* tmpRoom;
    bool unoNotPressed;
    while (1) {
        std::unique_lock<std::mutex> lck(_mutex);
        while (_messages.empty()) {
            _newMessage.wait(lck);
        }
        strcpy(buffer, _messages.front()->getMessage().c_str());
        std::cout << "Received message: " << buffer << " " << _messages.front()->getPlayer()->getSock() << std::endl;
        switch (buffer[0]) {
            case '1':
            {
                switch (buffer[1]) {
                        //create game odpoved
                    case '1':
                    {
                        tmpChar[0] = buffer[2];
                        tmpChar[1] = buffer[3];
                        int exists = this->findRoom(tmpChar);
                        if (exists == -1 && _rooms.size() < 5) {
                            //printf("send\n");
                            _rooms.push_back(new Room(_messages.front()->getPlayer(), buffer));
                            _rooms.back()->initPile();
                            _messages.front()->getPlayer()->setRoom(_rooms.back());
                            std::cout << "Sended message: " << "110ttttt" << " " << _messages.front()->getPlayer()->getSock() << std::endl;
                            send(_messages.front()->getPlayer()->getSock(), "110ttttt", 8, 0);
                        } else {
                            std::cout << "Sended message: " << "111fffff" << " " << _messages.front()->getPlayer()->getSock() << std::endl;
                            send(_messages.front()->getPlayer()->getSock(), "111fffff", 8, 0);
                        }
                        break;
                    }
                        //join game odpoved
                    case '2':
                    {
                        tmpChar[0] = buffer[2];
                        tmpChar[1] = buffer[3];
                        int at = this->findRoom(tmpChar);
                        if (at == -1) {
                            std::cout << "Sended message: " << "121fffff" << " " << _messages.front()->getPlayer()->getSock() << std::endl;
                            send(_messages.front()->getPlayer()->getSock(), "121fffff", 8, 0);
                        } else if (_rooms.at(at)->getState() == 1) {
                            std::cout << "Sended message: " << "121fffff" << " " << _messages.front()->getPlayer()->getSock() << std::endl;
                            send(_messages.front()->getPlayer()->getSock(), "121fffff", 8, 0);
                        } else if (_rooms.at(at)->isPlayerHere(_messages.front()->getPlayer())) {
                            std::cout << "Sended message: " << "121fffff" << " " << _messages.front()->getPlayer()->getSock() << std::endl;
                            send(_messages.front()->getPlayer()->getSock(), "121fffff", 8, 0);
                        } else {
                            _rooms.at(at)->addPlayer(_messages.front()->getPlayer());
                            buffer[0] = '1';
                            buffer[1] = '2';
                            buffer[2] = '0';
                            buffer[3] = _rooms.back()->getPlayers()->size() + '0';
                            std::cout << "Sended message: " << buffer << " " << _messages.front()->getPlayer()->getSock() << std::endl;
                            send(_messages.front()->getPlayer()->getSock(), buffer, 8, 0);
                            buffer[0] = '2';
                            buffer[1] = '1';
                            buffer[2] = _rooms.back()->getPlayers()->size() + '0';
                            _messages.front()->getPlayer()->setRoom(_rooms.at(at));
                            for (int i = 0; i < _rooms.back()->getPlayers()->size() - 1; i++) {
                                std::cout << "Sended message: " << buffer << " " << _rooms.back()->getPlayers()->at(i)->getSock() << std::endl;
                                send(_rooms.back()->getPlayers()->at(i)->getSock(), buffer, 8, 0);
                            }
                        }
                        break;
                    }
                }
                break;
            }
                //start hry odpoved
            case '2':
            {
                tmpColor = randomColor();
                buffer[0] = 3;
                buffer[1] = 1;
                tmpRoom = (Room*) _messages.front()->getPlayer()->getRoom();
                tmpRoom->setState(1);
                tmpCard2 = tmpRoom->getCard();
                tmpRoom->setDiscard(tmpCard2);
                for (i = 0; i < tmpRoom->getPlayers()->size(); i++) {
                    if (i != 0) {
                        //hra zacina
                        std::cout << "Sended message: " << "22222222" << " " << tmpRoom->getPlayers()->at(i)->getSock() << std::endl;
                        send(tmpRoom->getPlayers()->at(i)->getSock(), "22222222", 8, 0);
                    }
                    //rozdavanie kariet
                    for (j = 0; j < 7; j++) {
                        tmpCard = tmpRoom->getCard();
                        tmpRoom->getPlayers()->at(i)->addCard(tmpCard);
                        buffer[0] = '3';
                        buffer[1] = '1';
                        buffer[2] = tmpCard->getColor();
                        buffer[3] = tmpCard->getSymbol();
                        std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->at(i)->getSock() << std::endl;
                        send(tmpRoom->getPlayers()->at(i)->getSock(), buffer, 8, 0);
                    }
                    //karta na discard
                    buffer[0] = '3';
                    buffer[1] = '2';
                    buffer[2] = tmpCard2->getColor();
                    if (tmpCard2->getSymbol() == 'f' || tmpCard2->getSymbol() == 'c') {
                        buffer[2] = tmpColor;
                    }
                    buffer[3] = tmpCard2->getSymbol();
                    std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->at(i)->getSock() << std::endl;
                    send(tmpRoom->getPlayers()->at(i)->getSock(), buffer, 8, 0);
                }
                //hrac je na rade
                buffer[0] = '3';
                buffer[1] = '3';
                std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->front()->getSock() << std::endl;
                send(tmpRoom->getPlayers()->front()->getSock(), buffer, 8, 0);
                tmpRoom->setDirection(true);
                tmpRoom->setTurn(0);
                break;
            }
            case '3':
            {
                switch (buffer[1]) {
                        //uno odpoved
                    case '7':
                    {
                        tmpRoom = (Room*) _messages.front()->getPlayer()->getRoom();
                        for (i = 0; i < tmpRoom->getPlayers()->size(); i++) {
                            if (tmpRoom->getPlayers()->at(i)->getUno()) {
                                for (j = 0; j < 3; j++) {
                                    tmpCard = tmpRoom->getCard();
                                    tmpRoom->getPlayers()->at(i)->addCard(tmpCard);
                                    buffer[0] = '3';
                                    buffer[1] = '1';
                                    buffer[2] = tmpCard->getColor();
                                    buffer[3] = tmpCard->getSymbol();
                                    std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->at(i)->getSock() << std::endl;
                                    send(tmpRoom->getPlayers()->at(i)->getSock(), buffer, 8, 0);
                                }
                                std::cout << "Sended message: " << "38000000" << " " << tmpRoom->getPlayers()->at(i)->getSock() << std::endl;
                                send(tmpRoom->getPlayers()->at(i)->getSock(), "38000000", 8, 0);
                                tmpRoom->getPlayers()->at(i)->setUno(false);
                                tmpRoom->getPlayers()->at(i)->setRoundPassed(false);
                            }
                        }
                        break;
                    }
                        //odpoved na discard a vybratie dalsieho hraca
                    case '4':
                    {
                        //ziskavanie poradia karty na ruke a nasledne aj karty
                        tmpPlayer = _messages.front()->getPlayer();
                        tmpRoom = (Room*) tmpPlayer->getRoom();
                        tmpInt = buffer[2] - '0';
                        tmpCard = tmpPlayer->getCard(tmpInt);
                        //ak pocet kariet po vyhodeni do kopky rovna sa nule tak jednemu posle spravu ze vyhral a zvysku ze prehrali
                        if (tmpPlayer->getCardsNumber() == 0) {
                            for (i = 0; i < tmpRoom->getPlayers()->size(); i++) {
                                if (tmpRoom->getPlayers()->at(i) == tmpPlayer) {
                                    std::cout << "Sended message: " << "41000000" << " " << tmpRoom->getPlayers()->at(i)->getSock() << std::endl;
                                    send(tmpRoom->getPlayers()->at(i)->getSock(), "41000000", 8, 0);
                                } else {
                                    std::cout << "Sended message: " << "41000000" << " " << tmpRoom->getPlayers()->at(i)->getSock() << std::endl;
                                    send(tmpRoom->getPlayers()->at(i)->getSock(), "42looser", 8, 0);
                                }
                            }
                            //if (tmpPlayer->getRoom() != nullptr) {
                            //    tmpRoom->deletePlayer(tmpPlayer);
                            //    deleteRoom(tmpRoom);
                            //    delete tmpRoom;
                            //    tmpRoom = nullptr;
                            //}
                            break;
                        }
                        //Ak ma hrac jednu kartu a nestlacil tlacidlo uno
                        if (tmpPlayer->getCardsNumber() == 1 && buffer[4] == '0') {
                            unoNotPressed = true;
                            tmpPlayer->setUno(true);
                        }
                        tmpRoom->setDiscard(tmpCard);
                        tmpSymbol = tmpCard->getSymbol();
                        buffer[0] = '3';
                        buffer[1] = '2';
                        buffer[2] = tmpCard->getColor();
                        //+4 alebo zmena farby
                        if (tmpSymbol == 'f' || tmpSymbol == 'c') {
                            buffer[2] = buffer[3];
                        }
                        buffer[3] = tmpSymbol;
                        //zobrazi uno ostatnym
                        for (i = 0; i < tmpRoom->getPlayers()->size(); i++) {
                            std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->at(i)->getSock() << std::endl;
                            send(tmpRoom->getPlayers()->at(i)->getSock(), buffer, 8, 0);
                            if (unoNotPressed && tmpRoom->getPlayers()->at(i) != tmpPlayer) {
                                send(tmpRoom->getPlayers()->at(i)->getSock(), "37000000", 8, 0);
                            }
                        }
                        unoNotPressed = false;
                        //skip hraca
                        if (tmpSymbol == 's' || tmpSymbol == 't' || tmpSymbol == 'f') {
                            if (tmpRoom->getDirection()) {
                                if (tmpRoom->getTurn() == (tmpRoom->getPlayers()->size() - 1)) {
                                    tmpRoom->setTurn(0);
                                } else {
                                    tmpRoom->setTurn(tmpRoom->getTurn() + 1);
                                }
                            } else {
                                if (tmpRoom->getTurn() == 0) {
                                    tmpRoom->setTurn(tmpRoom->getPlayers()->size() - 1);
                                } else {
                                    tmpRoom->setTurn(tmpRoom->getTurn() - 1);
                                }
                            }

                        }
                        //+2
                        if (tmpSymbol == 't') {
                            tmpPlayer = tmpRoom->getPlayers()->at(tmpRoom->getTurn());

                            buffer[0] = '3';
                            buffer[1] = '1';

                            for (i = 0; i < 2; i++) {
                                tmpCard = tmpRoom->getCard();
                                tmpPlayer->addCard(tmpCard);
                                buffer[2] = tmpCard->getColor();
                                buffer[3] = tmpCard->getSymbol();
                                std::cout << "Sended message: " << buffer << " " << tmpPlayer->getSock() << std::endl;
                                send(tmpPlayer->getSock(), buffer, 8, 0);
                            }
                        }
                        //+4
                        if (tmpSymbol == 'f') {
                            tmpPlayer = tmpRoom->getPlayers()->at(tmpRoom->getTurn());

                            buffer[0] = '3';
                            buffer[1] = '1';

                            for (i = 0; i < 4; i++) {
                                tmpCard = tmpRoom->getCard();
                                tmpPlayer->addCard(tmpCard);
                                buffer[2] = tmpCard->getColor();
                                buffer[3] = tmpCard->getSymbol();
                                std::cout << "Sended message: " << buffer << " " << tmpPlayer->getSock() << std::endl;
                                send(tmpPlayer->getSock(), buffer, 8, 0);
                            }
                        }
                        //zmena smeru
                        if (tmpSymbol == 'd') {
                            tmpRoom->setDirection(!tmpRoom->getDirection());
                        }
                        //zistuje kto je na rade a posiela mu spravu
                        buffer[0] = '3';
                        buffer[1] = '3';
                        if (tmpRoom->getDirection()) {
                            if (tmpRoom->getTurn() == (tmpRoom->getPlayers()->size() - 1)) {
                                tmpRoom->setTurn(0);
                                std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->front()->getSock() << std::endl;
                                send(tmpRoom->getPlayers()->front()->getSock(), buffer, 8, 0);
                            } else {
                                tmpRoom->setTurn(tmpRoom->getTurn() + 1);
                                std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->at(tmpRoom->getTurn())->getSock() << std::endl;
                                send(tmpRoom->getPlayers()->at(tmpRoom->getTurn())->getSock(), buffer, 8, 0);
                            }
                        } else {
                            if (tmpRoom->getTurn() == 0) {
                                tmpRoom->setTurn(tmpRoom->getPlayers()->size() - 1);
                                std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->at(tmpRoom->getTurn())->getSock() << std::endl;
                                send(tmpRoom->getPlayers()->at(tmpRoom->getTurn())->getSock(), buffer, 8, 0);
                            } else {
                                tmpRoom->setTurn(tmpRoom->getTurn() - 1);
                                std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->at(tmpRoom->getTurn())->getSock() << std::endl;
                                send(tmpRoom->getPlayers()->at(tmpRoom->getTurn())->getSock(), buffer, 8, 0);
                            }
                        }
                        //tlacidlo uno mizne ak uz toto cele raz prebehlo
                        if (tmpRoom->getPlayers()->at(tmpRoom->getTurn())->getUno() && tmpRoom->getPlayers()->at(tmpRoom->getTurn())->getRoundPassed()) {
                            for (i = 0; i < tmpRoom->getPlayers()->size(); i++) {
                                if (tmpRoom->getPlayers()->at(i) != tmpRoom->getPlayers()->at(tmpRoom->getTurn())) {
                                    send(tmpRoom->getPlayers()->at(i)->getSock(), "38000000", 8, 0);
                                } else {
                                    send(tmpRoom->getPlayers()->at(i)->getSock(), "39000000", 8, 0);
                                }
                            }
                            tmpRoom->getPlayers()->at(tmpRoom->getTurn())->setUno(false);
                            tmpRoom->getPlayers()->at(tmpRoom->getTurn())->setRoundPassed(false);

                        }
                        //nastavuje ze preslo kolo
                        if (_messages.front()->getPlayer()->getUno()) {
                            _messages.front()->getPlayer()->setRoundPassed(true);
                        }
                        //posiela informaciu a pocte kariet oponentov
                        buffer[0] = '3';
                        buffer[1] = '6';
                        for (i = 0; i < tmpRoom->getPlayers()->size(); i++) {
                            tmpInt = 2;
                            for (j = 0; j < tmpRoom->getPlayers()->size(); j++) {
                                if (i != j) {
                                    buffer[tmpInt] = tmpRoom->getPlayers()->at(j)->getCardsNumber() + '0';
                                    tmpInt++;
                                }
                            }
                            std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->at(i)->getSock() << std::endl;
                            send(tmpRoom->getPlayers()->at(i)->getSock(), buffer, 8, 0);
                        }
                        break;
                    }
                        //taha kartu odpoved
                    case '5':
                    {
                        tmpRoom = (Room*) _messages.front()->getPlayer()->getRoom();
                        //kontrola ci kopka neni prazdna ak je tak sa premiestnia karty zo zohdenych to kopky
                        tmpRoom->pileToPile();
                        tmpCard = tmpRoom->getCard();
                        _messages.front()->getPlayer()->addCard(tmpCard);
                        buffer[0] = '3';
                        buffer[1] = '1';
                        buffer[2] = tmpCard->getColor();
                        buffer[3] = tmpCard->getSymbol();
                        std::cout << "Sended message: " << buffer << " " << _messages.front()->getPlayer()->getSock() << std::endl;
                        send(_messages.front()->getPlayer()->getSock(), buffer, 8, 0);
                        buffer[0] = '3';
                        buffer[1] = '3';
                        std::cout << "Sended message: " << buffer << " " << _messages.front()->getPlayer()->getSock() << std::endl;
                        send(_messages.front()->getPlayer()->getSock(), buffer, 8, 0);
                        //update poctu kariet superov na klientovy
                        buffer[0] = '3';
                        buffer[1] = '6';
                        for (i = 0; i < tmpRoom->getPlayers()->size(); i++) {
                            tmpInt = 2;
                            for (j = 0; j < tmpRoom->getPlayers()->size(); j++) {
                                if (i != j) {
                                    buffer[tmpInt] = tmpRoom->getPlayers()->at(j)->getCardsNumber() + '0';
                                    tmpInt++;
                                }
                            }
                            std::cout << "Sended message: " << buffer << " " << tmpRoom->getPlayers()->at(i)->getSock() << std::endl;
                            send(tmpRoom->getPlayers()->at(i)->getSock(), buffer, 8, 0);
                        }
                        break;
                    }
                }
                break;
            }
            case '4':
            {
                switch (buffer[1]) {
                        //ukoncenie programu
                    case '3':
                    {
                        send(_messages.front()->getPlayer()->getSock(), "44444444", 8, MSG_NOSIGNAL);
                        break;
                    }
                }
                break;
            }

        }
        //maze spravu
        delete _messages.front();
        _messages.front() = nullptr;
        _messages.erase(_messages.begin());
        lck.unlock();
    }
}
//Hlada miestnost podla id a vracia jej miesto poziciu vo vektore

int Server::findRoom(char* id) {
    int rooms_size = _rooms.size();
    int where = -1;
    int i;
    char pomid[2];
    pomid[0] = id[0];
    pomid[1] = id[1];
    char pomChar[2];
    for (i = 0; i < rooms_size; i++) {
        _rooms.at(i)->getId(pomChar);
        if (pomChar[0] == pomid[0] && pomChar[1] == pomid[1]) {
            where = i;
        }
    }
    return where;
}
//vymaze miestnost zo zoznamu

void Server::deleteRoom(Room* room) {
    Room* tmpRoom = room;
    int tmpInt = 0;
    while (_rooms.at(tmpInt) != tmpRoom) {
        tmpInt++;
    }
    _rooms.at(tmpInt) = nullptr;
    _rooms.erase(_rooms.begin() + tmpInt);
}

Server::~Server() {
    while (!_players.empty()) {
        delete _players.back();
        _players.back() = nullptr;
        _players.pop_back();
    }
    while (!_messages.empty()) {
        delete _messages.back();
        _messages.back() = nullptr;
        _messages.pop_back();
    }
    while (!_rooms.empty()) {
        delete _rooms.back();
        _rooms.back() = nullptr;
        _rooms.pop_back();
    }
    close(_mainSocket);
}