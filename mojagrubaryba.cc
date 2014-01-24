/* Tomasz Zakrzewski (tz336079) i Andrzej Białokozowicz (ab333903)
 * JNP1 2013/2014, Zadanie 6
 */
#include "mojagrubaryba.h"
#include <sstream>
#include <iostream>
#include <cstdio>

using std::ostringstream;
using std::printf;
using std::cout;
using std::endl;

//MojaGrubaRyba -------------------------------------------------------------------------

MojaGrubaRyba::MojaGrubaRyba() {
	board = unique_ptr<Board>(new Board(this));
}

void MojaGrubaRyba::setDie(std::shared_ptr< Die > die) {
	this->dieCopy = die;
}

void MojaGrubaRyba::addComputerPlayer(GrubaRyba::ComputerLevel level) {
	if (playersCopy.size() + 1 > maxNoPlayers)
		throw TooManyPlayersException(maxNoPlayers);

	shared_ptr<Player> player;
	ostringstream convert;
	string name;
	convert << "Gracz "<< playersCopy.size();
	name = convert.str();

	switch(level) {
		case GrubaRyba::ComputerLevel::DUMB:
			player = shared_ptr<Player> (new DumbComputerPlayer(1000, board->getInitialPosition(), name));
			break;
		case GrubaRyba::ComputerLevel::SMARTASS:
			player = shared_ptr<Player> (new SmartComputerPlayer(1000, board->getInitialPosition(), name));
			break;
	}
	playersCopy.push_back(player);
}

void MojaGrubaRyba::addHumanPlayer(std::shared_ptr< Human > human) {
	if (playersCopy.size() + 1 > maxNoPlayers)
		throw TooManyPlayersException(maxNoPlayers);

	shared_ptr<Player> player = shared_ptr<Player>(new HumanPlayer(1000, board->getInitialPosition(), human));
	playersCopy.push_back(player);
}

void MojaGrubaRyba::play(unsigned int rounds) {
	if (playersCopy.size() < minNoPlayers)
		throw TooFewPlayersException(minNoPlayers);

	reset();

	int counter = 1;
	while (rounds--) {
		printf("Runda: %d\n", counter);
		counter++;
		for (PlayerId id = getFirstPlayer(); id != getWatchdogPlayer(); id = getNextPlayerId(id)) {
// 			printf("%s ", players.at(id)->getName());
			cout<<players.at(id)->getName()<<" ";
			if (players.at(id)->isWaiting()) {
				printf("pole: Akwarium *** czekanie: %d ***\n", players.at(id)->getRoundsToWait());
			}
			else if (players.at(id)) {	//jeśli jeszcze gra
				unsigned int steps = rollDies();
				auto moveRes = board->makeMove(id, players.at(id)->getPosition(), steps);
				shared_ptr<Player> p;

				if (p = players.at(id)) {	//jeśli gracz po turze dalej jest w grze
					p->setPosition(moveRes.first);
					unsigned int rounds;
// 					printf("pole: %s", board->getFieldName(moveRes.first));
					cout<<"pole: "<<board->getFieldName(moveRes.first)<<endl;
					if ((rounds = moveRes.second->roundsToWait) > 0) {
						p->wait(rounds);
						printf(" *** czekanie: 3 ***\n");
					}
					else if (board->canBeBought(moveRes.first)) {
						auto name = board->getFieldName(moveRes.first);
						auto cost = board->getBuyValue(moveRes.first);
						if (p->wantBuy(name) && p->getMoney() >= cost) {
							p->setMoney(p->getMoney() - cost);
							p->addProperty(moveRes.first);
							board->own(id, moveRes.first);
						}
					}
					printf(" gotowka %d\n", players.at(id)->getMoney());
				}
				else {
					printf("*** bankrut ***\n");
				}
			}
			else {
				printf("*** bankrut ***\n");
			}
		}
	}
}

Money MojaGrubaRyba::takeMoneyFrom(const PlayerId& id, const Money& sum) {
	auto p = getPlayerAt(id);

	if (p->getMoney() >= sum) {
		p->setMoney(p->getMoney() - sum);
		return sum;
	}

	//player nie ma dosyć środków, musi zbankrutować
	Money debt = makeBankrupt(p);
	if (debt >= sum)
		return -1;
	return debt;
}

Money MojaGrubaRyba::transferMoney(const PlayerId& from, const PlayerId& to, const Money& sum) {
	Money debt = takeMoneyFrom(from, sum);

	if (debt == -1)
		debt = sum;

	giveMoneyTo(to, debt);

	return debt;
}

void MojaGrubaRyba::giveMoneyTo(const PlayerId& id, const Money& sum) {
	auto p = getPlayerAt(id);
	p->setMoney(p->getMoney() + sum);
}

unsigned int MojaGrubaRyba::rollDies() {
	return die->roll() + die->roll();
}

Money MojaGrubaRyba::makeBankrupt(shared_ptr< Player >& p){
	Money res = sellPropertiesOf(p);
	p = nullptr;	//usuwamy playera
	return res;
}

Money MojaGrubaRyba::sellPropertiesOf(shared_ptr<Player>& p) {
	Money res = 0;
	auto properties = p->getProperties();
	for(const auto &pos: properties) {
		auto name = board->getFieldName(pos);
		if (p->wantSell(name)) {	//próbujemy sprzedawać wszystko
			res += board->getSellValue(pos);
			board->deown(pos);
		}
	}

	return res;
}

shared_ptr< Player > MojaGrubaRyba::getPlayerAt(const PlayerId& id) const {
	auto p = players.at(id);
	if (!p)
		throw IllegalOperationException();
	return p;
}

PlayerId MojaGrubaRyba::getFirstPlayer() {
	return 0;
}

PlayerId MojaGrubaRyba::getWatchdogPlayer() {
	return players.size();
}

PlayerId MojaGrubaRyba::getNextPlayerId(const PlayerId& id) {
	return id + 1;
}

void MojaGrubaRyba::reset() {
	players = vector<shared_ptr<Player>>(playersCopy);
	board = unique_ptr<Board>(new Board(this));
	die = dieCopy->clone();
}

// Koniec (MojaGrubaRyba) ---------------------------------------------------------------
// Board --------------------------------------------------------------------------------

Board::Board(MojaGrubaRyba* gameMaster) : gameMaster{gameMaster} {
	fields.push_back(shared_ptr<Field>(new StartField("Start", 50)));
	fields.push_back(shared_ptr<Field>(new CoralField("Anemonia", 160)));
	fields.push_back(shared_ptr<Field>(new IslandField("Wyspa")));
	fields.push_back(shared_ptr<Field>(new CoralField("Aporina", 220)));
	fields.push_back(shared_ptr<Field>(new AquariumField("Akwarium", 3)));
	fields.push_back(shared_ptr<Field>(new PublicPropertyField("Grota", 300)));
	fields.push_back(shared_ptr<Field>(new CoralField("Menella", 280)));
	fields.push_back(shared_ptr<Field>(new DepositField("Laguna", 15)));
	fields.push_back(shared_ptr<Field>(new PublicPropertyField("Statek", 250)));
	fields.push_back(shared_ptr<Field>(new AwardField("Blazenki", 120)));
	fields.push_back(shared_ptr<Field>(new CoralField("Pennatula", 400)));
	fields.push_back(shared_ptr<Field>(new PenaltyField("Rekin", 180)));
}

MoveResult Board::makeMove(const PlayerId& id, const BoardPosition& pos, unsigned int steps) {
	MoveResult res;
	res.first = pos;

	while (steps--) {
		res.first = getNext(res.first);

		res.second = (steps == 0) ? getFieldAt(res.first)->stop() : getFieldAt(res.first)->pass();

		if (res.second->accountChange != 0) {
			Money sum = res.second->accountChange;
			if (sum > 0)	//dostajemy pieniądze
				gameMaster->giveMoneyTo(id, sum);
			else {
				Money paid;

				auto f = getFieldAt(res.first);
				if (f->isOwned())	//płacimy komuś trybut
					paid = gameMaster->transferMoney(id, f->getOwner(), sum);
				else	//płacimy rządowi i pieniądze gdzieś magicznie znikają!!
					f->store(paid = gameMaster->takeMoneyFrom(id, sum));

				if (paid < sum)
					return res;	//zbankrutował gracz, przerywamy
			}

			res.second->accountChange = 0;	//cokolwiek było do zapłacenia, zapłaciliśmy
		}
	}

	return res;
}

bool Board::canBeBought(const BoardPosition& pos) {
	return !getFieldAt(pos)->isOwned();
}

string Board::getFieldName(const BoardPosition& pos) {
	return getFieldAt(pos)->getName();
}

Money Board::getSellValue(const BoardPosition& pos) {
	return getFieldAt(pos)->getCost() / 2;
}

Money Board::getBuyValue(const BoardPosition& pos) {
	return getFieldAt(pos)->getCost();
}

void Board::own(const PlayerId& id, const BoardPosition& pos) {
	getFieldAt(pos)->setOwner(id);
}

void Board::deown(const BoardPosition& pos) {
	getFieldAt(pos)->removeOwner();
}

BoardPosition Board::getInitialPosition() const {
	return 0;
}

Board::~Board() {
	gameMaster = nullptr;	//we don't own our parent
}

shared_ptr< Field > Board::getFieldAt(const BoardPosition& pos) const {
	return fields.at(pos);
}

BoardPosition Board::getNext(const BoardPosition& pos) const {
	return (pos + 1) % fields.size();
}

BoardPosition Board::getBoardSize() const {
	return fields.size();
}


// Koniec (Board) -----------------------------------------------------------------------
// Player -------------------------------------------------------------------------------

Player::Player(const Money& money, const BoardPosition& position)
	: money{money}
	, position{position} {}

Player::Player(const Player& other) {
	clone(other);
}

Player::Player(Player&& other) {
	move(std::move(other));
}

void Player::clone(const Player& other) {
	money = other.money;
	roundsToWait = other.roundsToWait;
	position = other.position;
	properties = other.properties;
}

void Player::move(Player&& other) {
	money = std::move(other.money);
	roundsToWait = std::move(other.roundsToWait);
	position = std::move(other.position);
	properties = std::move(other.properties);
}

void Player::wait(unsigned int roundsToWait) {
	this->roundsToWait += roundsToWait;
}

bool Player::isWaiting() {
	if (roundsToWait > 0)
		roundsToWait--;
	return (roundsToWait == 0);
}

void Player::setMoney(const Money& sum) {
	if (sum < 0)
		throw IllegalOperationException();
	money = sum;
}

Money Player::getMoney() const {
	return money;
}

BoardPosition Player::getPosition() {
	return position;
}

void Player::setPosition(const BoardPosition& pos) {
	position = pos;
}

vector< BoardPosition > Player::getProperties() const {
	return properties;
}

void Player::addProperty(const BoardPosition& pos) {
	properties.push_back(pos);
}

unsigned int Player::getRoundsToWait() {
	return roundsToWait;
}


// Koniec (Player) ----------------------------------------------------------------------
// HumanPlayer
HumanPlayer::HumanPlayer(const Money& money, const BoardPosition &pos, shared_ptr< Human >& human)
	: Player{money, pos} {
		this->human = human->clone();
	}

void HumanPlayer::clone(const HumanPlayer& other) {
    Player::clone(other);
    human = other.human->clone();
}

void HumanPlayer::move(HumanPlayer&& other) {
    Player::move(std::move(other));
    human = std::move(other.human);
}


bool HumanPlayer::wantBuy(const string& propertyName) {
	return human->wantBuy(propertyName);
}

bool HumanPlayer::wantSell(const string& propertyName) {
	return human->wantSell(propertyName);
}

string const& HumanPlayer::getName() const {
	return human->getName();
}

// Koniec (HumanPlayer)
// ComputerPlayer
ComputerPlayer::ComputerPlayer(const Money& money, const BoardPosition &pos, const string& name)
	: Player(money, pos)
	, name{name} {}

string const& ComputerPlayer::getName() const {
	return name;
}

// Koniec ComputerPlayer
// DumbComputerPlayer
DumbComputerPlayer::DumbComputerPlayer(const Money& money, const BoardPosition &pos, const string& name)
	: ComputerPlayer{money, pos, name}
	, counter{0} {}

bool DumbComputerPlayer::wantBuy(const string& propertyName) {
	counter = (counter + 1) % 3;
	return (counter == 0);
}

bool DumbComputerPlayer::wantSell(const string& propertyName) {
	return false;
}

// Koniec DumbComputerPlayer
// SmartComputerPlayer
SmartComputerPlayer::SmartComputerPlayer(const Money& money, const BoardPosition &pos, const string& name)
	: ComputerPlayer{money, pos, name} {}

bool SmartComputerPlayer::wantBuy(const string& propertyName) {
	return true;
}

bool SmartComputerPlayer::wantSell(const string& propertyName) {
	return false;
}

// Koniec (SmartComputerPlayer)
// Field --------------------------------------------------------------------------------

Field::Field(const string& name, const Money cost)
	: name{name}
	, cost{cost} {}

Field::~Field() {
}

effectPtr Field::pass()
{
	effectPtr ptr(new EffectInfo(0, 0));
	return ptr;
}

const Money Field::getCost() const {
	return cost;
}

const string& Field::getName() const {
	return name;
}

const PlayerId Field::getOwner() const {
	return ownerId;
}

bool Field::isOwned() {
	return owned;
}

void Field::removeOwner() {
	owned = false;
}

void Field::setOwner(const PlayerId& ownerId) {
	owned = true;
	this->ownerId = ownerId;
}

void Field::store(Money amount) {
}

// Koniec (Field) -----------------------------------------------------------------------
// NotOwnedField ------------------------------------------------------------------------

NotOwnedField::NotOwnedField(const string& name)
	: Field(name, 0) {}

NotOwnedField::~NotOwnedField() {
}

bool NotOwnedField::isOwned() {
	return false;
}

// Koniec (NotOwnedField) ---------------------------------------------------------------
// StartField ---------------------------------------------------------------------------

StartField::StartField(const string& name, Money prize)
	: NotOwnedField(name)
	, prize{prize} {}

effectPtr StartField::pass()
{
	effectPtr ptr(new EffectInfo(prize, 0));
	return ptr;
}

effectPtr StartField::stop() {
	return this->pass();
}

// Koniec (StartField) ------------------------------------------------------------------
// AwardField ---------------------------------------------------------------------------

AwardField::AwardField(const string& name, Money prize)
	: NotOwnedField(name)
	, prize{prize} {}

effectPtr AwardField::stop()
{
	effectPtr ptr(new EffectInfo(prize, 0));
	return ptr;
}

// Koniec (AwardField) ------------------------------------------------------------------
// PenaltyField -------------------------------------------------------------------------

PenaltyField::PenaltyField(const string& name, Money penalty)
	: NotOwnedField(name)
	, penalty{penalty} {}

effectPtr PenaltyField::stop() {
	effectPtr ptr(new EffectInfo(-penalty, 0));
	return ptr;
}

// Koniec (PenaltyField) ----------------------------------------------------------------
// DepositField -------------------------------------------------------------------------

DepositField::DepositField(const string& name, Money penalty)
	: NotOwnedField(name)
	, penalty{penalty} {}

effectPtr DepositField::pass()
{
	effectPtr ptr(new EffectInfo(-penalty, 0));
	return ptr;
}

effectPtr DepositField::stop()
{
	effectPtr ptr(new EffectInfo(currentSum, 0));
	currentSum = 0;
	return ptr;
}

void DepositField::store(Money amount) {
	currentSum += amount;
}

// Koniec (DepositField) ----------------------------------------------------------------
// AquariumField ------------------------------------------------------------------------

AquariumField::AquariumField(const string& name, unsigned int rounds)
	: NotOwnedField(name)
	, rounds{rounds} {}

effectPtr AquariumField::stop() {
	effectPtr ptr(new EffectInfo(0, rounds));
	return ptr;
}

// Koniec (AquariumField) ---------------------------------------------------------------
// PropertyField ------------------------------------------------------------------------

PropertyField::PropertyField(const string& name, const Money cost, const Money penalty)
	: Field(name, cost)
	, penalty{penalty} {}

// Koniec (PropertyField) ---------------------------------------------------------------
// PublicPropertyField ------------------------------------------------------------------
PublicPropertyField::PublicPropertyField(const string& name, const Money cost)
	: PropertyField(name, cost, (cost * 40) / 100) {}

effectPtr PublicPropertyField::stop() {
	Money amount;
	if (this->isOwned())
		amount = -penalty;
	else
		amount = -cost;
	effectPtr ptr(new EffectInfo(amount, 0));
	return ptr;
}

// Koniec (PublicPropertyField) ---------------------------------------------------------
// CoralField ---------------------------------------------------------------------------
CoralField::CoralField(const string& name, const Money cost)
	: PropertyField(name, cost, (cost * 20) / 100) {}

effectPtr CoralField::stop() {
	Money amount;
	if (this->isOwned())
		amount = -penalty;
	else
		amount = -cost;
	effectPtr ptr(new EffectInfo(amount, 0));
	return ptr;
}

IslandField::IslandField(const string& name)
	: NotOwnedField(name) {}


effectPtr IslandField::stop() {
	return effectPtr(new EffectInfo(0, 0));
}


// Koniec (CoralField) ------------------------------------------------------------------
// EffectInfo ---------------------------------------------------------------------------

EffectInfo::EffectInfo(const Money& accountChange, const unsigned int roundsToWait)
	: roundsToWait{roundsToWait}
	, accountChange{accountChange} {}

//Koniec (EffectInfo) -------------------------------------------------------------------
