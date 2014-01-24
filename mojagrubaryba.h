/* Tomasz Zakrzewski (tz336079) i Andrzej Białokozowicz (ab333903)
 * JNP1 2013/2014, Zadanie 6
 */
#ifndef MOJAGRUBARYBA_H
#define MOJAGRUBARYBA_H

#include <cstdio>
#include <vector>
#include <list>
#include <string>
#include <exception>

#include "grubaryba.h"

using std::vector;
using std::list;
using std::shared_ptr;
using std::max;
using std::unique_ptr;
using std::pair;
using std::string;

class Board;
class Player;
class Field;
struct EffectInfo;
typedef size_t PlayerId;
typedef size_t BoardPosition;
typedef int Money;
typedef pair<BoardPosition, shared_ptr<EffectInfo>> MoveResult;
typedef shared_ptr <EffectInfo> effectPtr;

//wyjątek rzucany, jeśli czyjaś implementacja, używając naszych klas,
//
class IllegalOperationException : public std::exception {
public:
	IllegalOperationException() {}
	const char* what() const noexcept {
		return "Violated one of the contracts";
	}
};

struct EffectInfo {
	unsigned int roundsToWait;

	//> 0 -> gracz dostaje pieniądze
	//< 0 -> gracz płaci
	Money accountChange;

	//pomocniczy konstruktor
	EffectInfo(const Money& accountChange, const unsigned int roundsToWait);
	EffectInfo(){};
};

class MojaGrubaRyba : public GrubaRyba {
public:
	MojaGrubaRyba();

	virtual void setDie(shared_ptr<Die> die);
	virtual void addComputerPlayer(ComputerLevel level);
	virtual void addHumanPlayer(shared_ptr<Human> human);
	virtual void play(unsigned int rounds);

	//Dwie poniższe metody służą do transferu pieniędzy między graczami/zabierania pieniędzy graczowi.
	//Metody zwracają sumę pieniędzy jaka została przekazana/odebrana (może być < sum) lub -1, w przypadku
	//gdy przekazana suma była równa sum, ale gracz musiał zbankrutować, aby ją wypłacić.
	//Obie metody rzucają wyjątkami, jeśli argumentem jest nieistniejący (nigdy nie stworzony, lub taki który
	//już przegrał) gracz
	Money takeMoneyFrom(const PlayerId& id, const Money& sum);
	Money transferMoney(const PlayerId& from, const PlayerId& to, const Money& sum);

	void giveMoneyTo(const PlayerId& id, const Money& sum);

	//Metoda rzuca kostkami i zwraca wynik rzutu.
	unsigned int rollDies();

private:
	//Gracz p bankrutuje.
	//Metoda zwraca ilość pieniędzy odzyskanych od niego ze sprzedawania nieruchomości
	Money makeBankrupt(shared_ptr<Player>& p);

	//Pyta gracza o każdą z jego nieruchomości i próbuje je sprzedać.
	//Zwraca ilość uzbieranych pieniędzy.
	Money sellPropertiesOf(shared_ptr< Player >& p);

	//Zwraca shared_ptr na gracza o danym id
	shared_ptr<Player> getPlayerAt(const PlayerId& id) const;

	//nie-const, bo nie wiadomo jak ktoś kiedyś chce trzymać playerów
	//może np. chcieć coś sobie przetwarzać przed zwróceniem pierwszego
	PlayerId getFirstPlayer();

	//zwraca id watchdoga - końca zbioru playerów
	PlayerId getWatchdogPlayer();

	PlayerId getNextPlayerId(const PlayerId& id);

	//przywróć planszę do stanu początkowego
	void reset();

	const size_t minNoPlayers = 2;
	const size_t maxNoPlayers = 8;
	shared_ptr<Die> die;
	shared_ptr<Die> dieCopy;
	vector<shared_ptr<Player>> players;
	//przechowuje kopię playerów z oryginalnymi wartościami
	//na potrzeby gry od nowa
	vector<shared_ptr<Player>> playersCopy;
	unique_ptr<Board> board;
};


class Board {
public:
	Board(MojaGrubaRyba* gameMaster);

	//Wykonuje ruch z perspektywy planszy - nie ma dostępu do playera, wie tylko że jakiś obiekt
	//identyfikowalny przez od chce wykonać ruch ze swojej obecnej pozycji pos o steps pól do przodu.
	//Możliwość wykonania każdego ruchu będzie walidowana w MojejGrubejRybie.
	//FIXME tu jest zaszyta logika tego co się dzieje w grze,  więc i możliwości.
	//W związku z tym ta klasa powinna mieć część metod (włącznie z tą) wirtualnych oraz
	//wirtualny destruktor, bo ktoś chcący dodać nowe rodzaje pól z nieznanymi wcześniej efektami, będzie musiał przedefiniować
	//tę metodę, żeby te pola działały
	MoveResult makeMove(const PlayerId &id, const BoardPosition& pos, unsigned int steps);


	virtual bool canBeBought(const BoardPosition &pos);

	string getFieldName(const BoardPosition &pos);
	Money getSellValue(const BoardPosition &pos);
	Money getBuyValue(const BoardPosition &pos);

	//Przypisuje pole na pozycji pos graczowi id
	void own(const PlayerId &id, const BoardPosition &pos);
	//Zwalnia pole na pozycji pos (pole traci poprzedniego)
	//właściciela
	void deown(const BoardPosition &pos);

	BoardPosition getInitialPosition() const;

	~Board();
private:
	//Metoda zwracająca kolejne pole (abstrakcja BoardPosition)
	BoardPosition getNext(const BoardPosition& pos) const;

	//Metoda zwracająca rozmiar planszy (abstrakcja wokół std::vector)
	BoardPosition getBoardSize() const;

	shared_ptr<Field> getFieldAt(const BoardPosition& pos) const;

	MojaGrubaRyba* gameMaster;

	vector<shared_ptr <Field> >fields;
};


class Player {
public:
	Player(const Money& money, const BoardPosition& position);
	Player(const Player &other);
	Player(Player &&other);
	virtual ~Player(){}

	virtual void clone(const Player& other);
	virtual void move(Player &&other);

	// Zwraca imię gracza
	virtual std::string const& getName() const = 0;

	// Zwraca true, jeśli gracz chce kupić daną posiadłość.
	virtual bool wantBuy(std::string const& propertyName) = 0;

	// Zwraca true, jeśli gracz chce sprzedać daną posiadłość.
	// Wywoływane w przypadku, gdy brakuje graczowi pieniędzy na zakup lub opłaty.
	virtual bool wantSell(std::string const& propertyName) = 0;

	void wait(unsigned int roundsToWait);
	bool isWaiting();
	unsigned int getRoundsToWait();

	//Ustawia stan konta gracza na sum. Nielegalnym jest ustawianie konta na ujemną
	//wartość. W takim przypadku, metoda rzuca IllegalOperationException i stan konta
	//gracza pozostaje niezmieniony.
	void setMoney(const Money &sum);
	Money getMoney() const;

	BoardPosition getPosition();
	void setPosition(const BoardPosition& pos);

	//przez kopię, bo może się zmieniać
	vector<BoardPosition> getProperties() const;
	void addProperty(const BoardPosition& pos);
private:
	unsigned int roundsToWait;
	Money money;
	BoardPosition position;
	vector<BoardPosition> properties;
};

class HumanPlayer : public Player {

public:
	HumanPlayer(const Money& money, const BoardPosition& pos, shared_ptr< Human >& human);

	virtual void clone(const HumanPlayer& other);
	virtual void move(HumanPlayer&& other);

	virtual bool wantBuy(string const& propertyName);
	virtual bool wantSell(string const& propertyName);
	string const& getName() const;

private:
	shared_ptr<Human> human;
};

class ComputerPlayer : public Player {

public:
	ComputerPlayer(const Money& money, const BoardPosition& pos, const string& name);

	virtual bool wantBuy(string const& propertyName) = 0;
	virtual bool wantSell(string const& propertyName) = 0;
	string const& getName() const;

private:
	string name;
};

class DumbComputerPlayer : public ComputerPlayer
{
public:
	DumbComputerPlayer(const Money& money, const BoardPosition& pos, const string& name);

	virtual bool wantBuy(string const& propertyName);
	virtual bool wantSell(string const& propertyName);

private:
	int counter;
};


class SmartComputerPlayer : public ComputerPlayer
{
public:

	SmartComputerPlayer(const Money& money, const BoardPosition& pos, const string& name);

	virtual bool wantBuy(string const& propertyName);
	virtual bool wantSell(string const& propertyName);
};


class Field {
public:
	Field(const string& name, const Money cost);
	virtual ~Field();

	virtual effectPtr pass();
	virtual effectPtr stop() = 0;
	virtual bool isOwned();
	virtual void store(Money amount);

	const PlayerId getOwner() const;
	const Money getCost() const;
	const string& getName() const;

	void setOwner(const PlayerId& ownerId);
	void removeOwner();

private:
	PlayerId ownerId;
	string name;
	bool owned;

protected:
	Money cost;
};


class NotOwnedField : public Field {
public:
	NotOwnedField(const string& name);
	virtual ~NotOwnedField();

	virtual effectPtr stop() = 0;
	bool isOwned();
};


class StartField : public NotOwnedField {
public:
	StartField(const string& name, Money prize);

	virtual effectPtr pass();
	virtual effectPtr stop();

private:
	Money prize;
};


class AwardField : public NotOwnedField {
public:
	AwardField(const string& name, Money prize);

	virtual effectPtr stop();

private:
	Money prize;
};


class PenaltyField : public NotOwnedField {
public:
	PenaltyField(const string& name, Money penalty);

	virtual effectPtr stop();

private:
	Money penalty;
};


class DepositField : public NotOwnedField {
public:
	DepositField(const string& name, Money penalty);

	virtual effectPtr pass();
	virtual effectPtr stop();
	virtual void store(Money amount);

private:
	Money penalty;
	Money currentSum;
};


class AquariumField : public NotOwnedField {
public:
	AquariumField(const string& name, unsigned int rounds);

	virtual effectPtr stop();

private:
	unsigned int rounds;
};


class PropertyField : public Field {
public:
	PropertyField(const string& name, const Money cost, const Money penalty);

	virtual effectPtr stop() = 0;

protected:
	Money penalty;
};


class PublicPropertyField : public PropertyField {
public:
	PublicPropertyField(const string& name, const Money cost);

	virtual effectPtr stop();
};


class CoralField : public PropertyField {
public:
	CoralField(const string& name, const Money cost);

	virtual effectPtr stop();
};

class IslandField : public NotOwnedField {
public:
	IslandField(const string& name);

	virtual effectPtr stop();
};

#endif
