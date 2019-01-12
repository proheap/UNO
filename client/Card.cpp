#include "Card.h"

Card::Card(char cardColor, char cardSymbol, int posX, int posY) :
	_sendCardColor(cardColor),
	_sendCardSymbol(cardSymbol),
	_width(CARD_WIDTH),
	_height(CARD_HEIGHT),
	_posX(posX),
	_posY(posY)
{
	decryptCard();
	loadFileCardPicture();

	cout << "Karta vytvorena: " << cardColor << " " << cardSymbol << endl;
	cout << _fileCardPicture << endl;
}

Card::~Card()
{
	_fileCardPicture = nullptr;
}

void Card::decryptCard()
{
	switch (_sendCardColor)
	{
	case 'b':
		_cardColor = TypesOfCards::BLUE;
		break;
	case 'r':
		_cardColor = TypesOfCards::RED;
		break;
	case 'g':
		_cardColor = TypesOfCards::GREEN;
		break;
	case 'y':
		_cardColor = TypesOfCards::YELLOW;
		break;
	default:
		_cardColor = TypesOfCards::SPECIAL;
		break;
	}

	switch (_sendCardSymbol)
	{
	case '0':
		_cardSymbol = TypesOfCards::ZERO;
		break;
	case '1':
		_cardSymbol = TypesOfCards::ONE;
		break;
	case '2':
		_cardSymbol = TypesOfCards::TWO;
		break;
	case '3':
		_cardSymbol = TypesOfCards::THREE;
		break;
	case '4':
		_cardSymbol = TypesOfCards::FOUR;
		break;
	case '5':
		_cardSymbol = TypesOfCards::FIVE;
		break;
	case '6':
		_cardSymbol = TypesOfCards::SIX;
		break;
	case '7':
		_cardSymbol = TypesOfCards::SEVEN;
		break;
	case '8':
		_cardSymbol = TypesOfCards::EIGHT;
		break;
	case '9':
		_cardSymbol = TypesOfCards::NINE;
		break;
	case 's':
		_cardSymbol = TypesOfCards::SKIP;
		break;
	case 'd':
		_cardSymbol = TypesOfCards::CHANGE_DIRECTION;
		break;
	case 't':
		_cardSymbol = TypesOfCards::PLUS_2;
		break;
	case 'c':
		_cardSymbol = TypesOfCards::CHANGE_COLOR;
		break;
	default:
		_cardSymbol = TypesOfCards::PLUS_4;
		break;
	}
}

void Card::loadFileCardPicture()
{
	switch (_cardSymbol)
	{
	case TypesOfCards::ZERO:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_0.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_0.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_0.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_0.png";
			break;
		}
		break;
	case TypesOfCards::ONE:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_1.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_1.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_1.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_1.png";
			break;
		}
		break;
	case TypesOfCards::TWO:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_2.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_2.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_2.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_2.png";
			break;
		}
		break;
	case TypesOfCards::THREE:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_3.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_3.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_3.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_3.png";
			break;
		}
		break;
	case TypesOfCards::FOUR:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_4.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_4.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_4.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_4.png";
			break;
		}
		break;
	case TypesOfCards::FIVE:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_5.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_5.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_5.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_5.png";
			break;
		}
		break;
	case TypesOfCards::SIX:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_6.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_6.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_6.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_6.png";
			break;
		}
		break;
	case TypesOfCards::SEVEN:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_7.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_7.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_7.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_7.png";
			break;
		}
		break;
	case TypesOfCards::EIGHT:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_8.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_8.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_8.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_8.png";
			break;
		}
		break;
	case TypesOfCards::NINE:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_9.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_9.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_9.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_9.png";
			break;
		}
		break;
	case TypesOfCards::PLUS_2:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_p.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_p.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_p.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_p.png";
			break;
		}
		break;
	case TypesOfCards::CHANGE_DIRECTION:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_d.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_d.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_d.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_d.png";
			break;
		}
		break;
	case TypesOfCards::SKIP:
		switch (_cardColor)
		{
		case TypesOfCards::BLUE:
			_fileCardPicture = "assets/cards/blue_s.png";
			break;
		case TypesOfCards::RED:
			_fileCardPicture = "assets/cards/red_s.png";
			break;
		case TypesOfCards::GREEN:
			_fileCardPicture = "assets/cards/green_s.png";
			break;
		default:
			_fileCardPicture = "assets/cards/yellow_s.png";
			break;
		}
		break;
	case TypesOfCards::CHANGE_COLOR:
		_fileCardPicture = "assets/cards/special_c.png";
		break;
	default:
		_fileCardPicture = "assets/cards/special_p.png";
		break;
	}
}

const char * Card::getFileCardPicture()
{
	return _fileCardPicture;
}

TypesOfCards::Color Card::getCardColor()
{
	return _cardColor;
}

TypesOfCards::Symbol Card::getCardSymbol()
{
	return _cardSymbol;
}

int Card::getWidth()
{
	return _width;
}

int Card::getHeight()
{
	return _height;
}

int Card::getPosX()
{
	return _posX;
}

int Card::getPosY()
{
	return _posY;
}
