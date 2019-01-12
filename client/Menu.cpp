#include "Menu.h"

Menu::Menu()
{
    zobrazMenu();
}

Menu::~Menu()
{
    delete hra_;
    hra_ = nullptr;
}

void Menu::zobrazMenu()
{
    cout << "Hra UNO" << endl
    << "1. Zacat hru" << endl
    << "2. Ukoncit hru" << endl;
	vyberMoznost();
}

void Menu::vyberMoznost()
{
    int i;
    cout << "Vyber moznost: " << endl;
    cin >> i;
}