/* Tomasz Zakrzewski (tz336079) i Andrzej Białokozowicz (ab333903)
 * JNP1 2013/2014, Zadanie 6
 */
#ifndef MOJAGRUBARYBA_H
#define MOJAGRUBARYBA_H

#include <cstdio>
#include <vector>
#include <string>
#include <memory>

#include "grubaryba.h"

class Board;
class Player;
class Field;
struct EffectInfo;
typedef size_t PlayerId;
typedef size_t BoardPosition;
typedef int Money;
typedef std::pair<BoardPosition, std::shared_ptr<EffectInfo>> MoveResult;
typedef std::shared_ptr <EffectInfo> effectPtr;

//wyjątek rzucany, jeśli czyjaś implementacja, używając naszych klas,
//naruszy kontrakty wystawiane przez nie
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

	virtual void setDie(std::shared_ptr<Die> die);
	virtual void addComputerPlayer(ComputerLevel level);
	virtual void addHumanPlayer(std::shared_ptr<Human> human);
	virtual void play(unsigned int rounds);

	//Dwie poniższe metody służą do transferu pieniędzy między graczami/zabierania pieniędzy graczowi.
	//Metody zwracają sumę pieniędzy jaka została przekazana/odebrana (może być < sum) lub -1, w przypadku
	//gdy przekazana suma była równa sum, ale gracz musiał zbankrutować, aby ją wypłacić.
	//Obie metody rzucają wyjątkami, jeśli argumentem jest nieistniejący (nigdy nie stworzony, lub taki który
	//już przegrał) gracz
	Money takeMoneyFrom(const PlayerId& id, const Money& sum);
	Money transferMoney(const PlayerId& from, const PlayerId& to, const Money& sum);

	void giveMoneyTo(const PlayerId& id, const Money& sum);

private:
	//wypisuje stan po turze
	void printState() const;

	//Metoda rzuca kostkami i zwraca wynik rzutu.
	unsigned int rollDies();

	//Gracz p bankrutuje.
	//Metoda zwraca ilość pieniędzy odzyskanych od niego ze sprzedawania nieruchomości
	Money makeBankrupt(std::shared_ptr< Player >& p, Money sum);

	//Pyta gracza o każdą z jego nieruchomości i próbuje je sprzedać.
	//Zwraca ilość uzbieranych pieniędzy.
	Money sellPropertiesOf(std::shared_ptr<Player>& p, std::vector<BoardPosition>& properties);

    //Symuluje sellPropertiesOf bez faktycznego sprzedawania
    //Zwraca parę <Money, std::vector<BoardPosition>> - ilość pieniędzy uzyskanych z transakcji
    //oraz lista sprzedawanych nieruchomości
    std::pair<Money, std::vector<BoardPosition>> trySellPropertiesOf(std::shared_ptr< Player >& p);

	//Zwraca std::shared_ptr na gracza o danym id
	std::shared_ptr<Player> getPlayerAt(const PlayerId& id) const;

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
	std::shared_ptr<Die> die;
	std::shared_ptr<Die> dieCopy;
	std::vector<std::shared_ptr<Player>> players;
	std::unique_ptr<Board> board;
};


class Board {
public:
	Board(MojaGrubaRyba* gameMaster);

	//Wykonuje ruch z perspektywy planszy - nie ma dostępu do playera, wie tylko że jakiś obiekt
	//identyfikowalny przez od chce wykonać ruch ze swojej obecnej pozycji pos o steps pól do przodu.
	//Możliwość wykonania każdego ruchu będzie walidowana w MojejGrubejRybie.
	//Tu jest zaszyta logika tego co się dzieje w grze,  więc i możliwości.
	//W związku z tym ta klasa powinna mieć część metod (włącznie z tą) wirtualnych oraz
	//wirtualny destruktor, bo ktoś chcący dodać nowe rodzaje pól z nieznanymi wcześniej efektami, będzie musiał przedefiniować
	//tę metodę, żeby te pola działały
	virtual MoveResult makeMove(const PlayerId &id, const BoardPosition& pos, unsigned int steps);

	virtual bool canBeBought(const BoardPosition &pos);

	std::string getFieldName(const BoardPosition &pos);
	virtual Money getSellValue(const BoardPosition &pos);
	virtual Money getBuyValue(const BoardPosition &pos);

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

	std::shared_ptr<Field> getFieldAt(const BoardPosition& pos) const;

	MojaGrubaRyba* gameMaster;

	std::vector<std::shared_ptr <Field> >fields;
};


class Player {
public:
	Player(const Money& money, const BoardPosition& position);
	Player(const Player &other);
	Player(Player &&other);
	virtual ~Player(){}

	// Zwraca imię gracza
	virtual std::string const& getName() const = 0;

	// Zwraca true, jeśli gracz chce kupić daną posiadłość.
	virtual bool wantBuy(std::string const& propertyName) = 0;

	// Zwraca true, jeśli gracz chce sprzedać daną posiadłość.
	// Wywoływane w przypadku, gdy brakuje graczowi pieniędzy na zakup lub opłaty.
	virtual bool wantSell(std::string const& propertyName) = 0;

	bool isActive() const;
	void deactivate();

	void wait(unsigned int roundsToWait);
	virtual bool isWaiting();
	unsigned int getRoundsToWait() const;

	//Ustawia stan konta gracza na sum. Nielegalnym jest ustawianie konta na ujemną
	//wartość. W takim przypadku, metoda rzuca IllegalOperationException i stan konta
	//gracza pozostaje niezmieniony.
	void setMoney(const Money &sum);
	Money getMoney() const;

	BoardPosition getPosition();
	void setPosition(const BoardPosition& pos);

	//przez kopię, bo może się zmieniać
	std::vector<BoardPosition> getProperties() const;
	void addProperty(const BoardPosition& pos);
    void removeProperty(const BoardPosition& pos);

    virtual void reset();

protected:
	virtual void clone(const Player &other);
	virtual void move(Player &&other);

private:

	bool active;
	unsigned int roundsToWait;
	Money money, startMoney;
	BoardPosition position, startPosition;
	std::vector<BoardPosition> properties;
};


class HumanPlayer : public Player {
public:
	HumanPlayer(const Money &money, const BoardPosition &pos, std::shared_ptr<Human> &human);

	virtual bool wantBuy(std::string const &propertyName);
	virtual bool wantSell(std::string const &propertyName);
	std::string const& getName() const;

protected:
	virtual void clone(const HumanPlayer& other);
	virtual void move(HumanPlayer&& other);

private:
	std::shared_ptr<Human> human;
};


class ComputerPlayer : public Player {
public:
	ComputerPlayer(const Money& money, const BoardPosition& pos, const std::string& name);

	virtual bool wantBuy(std::string const& propertyName) = 0;
	virtual bool wantSell(std::string const& propertyName) = 0;
	std::string const& getName() const;

private:
	std::string name;
};


class DumbComputerPlayer : public ComputerPlayer {
public:
	DumbComputerPlayer(const Money& money, const BoardPosition& pos, const std::string& name);

	virtual bool wantBuy(std::string const& propertyName);
	virtual bool wantSell(std::string const& propertyName);

private:
	int counter;
};


class SmartComputerPlayer : public ComputerPlayer {
public:

	SmartComputerPlayer(const Money& money, const BoardPosition& pos, const std::string& name);

	virtual bool wantBuy(std::string const& propertyName);
	virtual bool wantSell(std::string const& propertyName);
};


class Field {
public:
	Field(const std::string& name, const Money cost);
	virtual ~Field();

	virtual effectPtr pass();
	virtual effectPtr stop() = 0;
	virtual bool isOwned();
	virtual bool canBeBought();
	virtual void store(Money amount);

	const PlayerId getOwner() const;
	const Money getCost() const;
	const std::string& getName() const;

	void setOwner(const PlayerId& ownerId);
	void removeOwner();

private:
	PlayerId ownerId;
	std::string name;
	bool owned;

protected:
	Money cost;
};


class NotOwnedField : public Field {
public:
	NotOwnedField(const std::string& name);
	virtual ~NotOwnedField();

	virtual effectPtr stop() = 0;
	virtual bool isOwned();
	virtual bool canBeBought();
};


class StartField : public NotOwnedField {
public:
	StartField(const std::string& name, Money prize);

	virtual effectPtr pass();
	virtual effectPtr stop();

private:
	Money prize;
};


class AwardField : public NotOwnedField {
public:
	AwardField(const std::string& name, Money prize);

	virtual effectPtr stop();

private:
	Money prize;
};


class PenaltyField : public NotOwnedField {
public:
	PenaltyField(const std::string& name, Money penalty);

	virtual effectPtr stop();

private:
	Money penalty;
};


class DepositField : public NotOwnedField {
public:
	DepositField(const std::string& name, Money penalty);

	virtual effectPtr pass();
	virtual effectPtr stop();
	virtual void store(Money amount);

private:
	Money penalty;
	Money currentSum;
};


class AquariumField : public NotOwnedField {
public:
	AquariumField(const std::string& name, unsigned int rounds);

	virtual effectPtr stop();

private:
	unsigned int rounds;
};


class PropertyField : public Field {
public:
	PropertyField(const std::string& name, const Money cost, const Money penalty);

	virtual effectPtr stop() = 0;

protected:
	Money penalty;
};


class PublicPropertyField : public PropertyField {
public:
	PublicPropertyField(const std::string& name, const Money cost);

	virtual effectPtr stop();
};


class CoralField : public PropertyField {
public:
	CoralField(const std::string& name, const Money cost);

	virtual effectPtr stop();
};

class IslandField : public NotOwnedField {
public:
	IslandField(const std::string& name);

	virtual effectPtr stop();
};

#endif
