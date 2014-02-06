//
// grubaryba_helper.h
// pomocnicze klasy i funkcje
//

#ifndef GRUBARYBA_HELPER_H
#define GRUBARYBA_HELPER_H

#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <cassert>
#include "../../mojagrubaryba.h"

using namespace std::placeholders;

namespace test {

    typedef std::shared_ptr<Die> DiePtr;
    typedef std::shared_ptr<Human> HumanPtr;

    const size_t MIN_PLAYERS = 2;
    const size_t MAX_PLAYERS = 8;

    typedef enum {START = 1, ANEMONIA, WYSPA, APORINA, AKWARIUM, GROTA, MENELLA, LAGUNA, STATEK, BLAZENKI, PENNATULA, REKIN} Field;

    typedef unsigned short Roll;
    typedef std::vector<Roll> Rolls;
    typedef Rolls::iterator RollsIter;
    typedef std::vector<Rolls> PlayersRolls;
    typedef PlayersRolls::iterator PlayersRollsIter;
    typedef std::vector<Field> Fields;

    // tworzy wyniki podwojnych rzutow w kolejnosci rzutow na podstawie danych dla poszczegolnych graczy
    Rolls combinePlayers(PlayersRolls const& all) {
        Rolls ret;
        if (all.size()) {
            Rolls const& first = all[0];
            for (size_t i = 0; i < first.size(); i++) {
                for (Rolls playerRolls : all) {
                    if (i < playerRolls.size()) {
                        ret.push_back(playerRolls[i]);
                    } else {
                        ret.push_back(first[i]);
                    }
                }
            }
        }
        return ret;
    }

    Rolls expandPlayers(Rolls const& singleRolls, unsigned pNum) {
        PlayersRolls pRolls;
        while (pNum--) pRolls.push_back(singleRolls);
        return combinePlayers(pRolls);
    }

    Rolls toRolls(Fields const& fields, unsigned multiplyLast = 0) {
        Rolls ret;
        unsigned short last = START;
        for (auto field : fields) {
            auto total = (field > last) ? field-last : 12+field-last;
            ret.push_back(total);
            last = field;
        }
        while (multiplyLast--) {
            ret.push_back(12);
        }
        return ret;
    }

    class TestDie : public Die {
        static unsigned int rollNum;
        const Rolls singleDieRolls;
    public:
        static Rolls expandResults(std::vector<unsigned short> const& totals) {
            std::vector<unsigned short> singles;
            for (auto total : totals) {
                singles.push_back(1);
                singles.push_back(total-1);
            }
            return singles;
        }

        // creates a test die based on total results of subsequent players' rolls
        TestDie(std::vector<unsigned short> const& totals): singleDieRolls(expandResults(totals)) {
            rollNum = 0;
        }
        TestDie(TestDie const& die): singleDieRolls(die.singleDieRolls) {}

        unsigned short roll() const {
            return singleDieRolls[rollNum++ % singleDieRolls.size()];
        }
        std::shared_ptr<Die> clone() const {
            return std::shared_ptr<Die>(new TestDie(*this));
        }
    };
    unsigned int TestDie::rollNum = 0;

    void initSmartass(MojaGrubaRyba& mgr, unsigned num = 2) {
        while (num--) {
            mgr.addComputerPlayer(GrubaRyba::ComputerLevel::SMARTASS);
        }
    }

    void initDumb(MojaGrubaRyba& mgr, unsigned num = 2) {
        while (num--) {
            mgr.addComputerPlayer(GrubaRyba::ComputerLevel::DUMB);
        }
    }

    class TestSelling {
        typedef std::string Property;
        std::string name;
        std::map<Property, unsigned int> sellQuestions;
        Property buying;
        std::set<Property> bought;
        bool saleInProgress;
    public:
        TestSelling(std::string const& name_): name(name_), saleInProgress(false) {}

        void addSelling(Property const& prop) {
            // std::cout << "wants to sell " << prop << std::endl;
            auto found = sellQuestions.find(prop);
            if (found == sellQuestions.end()) {
                sellQuestions[prop] = 1;
            } else {
                found->second++;
            }
            bool sold = bought.erase(prop);
            assert(sold || buying == prop);
            if (buying == prop) buying = "";
            if (bought.size() == 0) {
                saleInProgress = false;
            }
        }

        void addBuying(Property const& prop) {
            // std::cout << "wants to buy " << prop << std::endl;
            if (buying.size() > 0) {
                bought.insert(buying);
            }
            assert(!saleInProgress);
            buying = prop;
        }

        void print(std::string const& op, std::set<Property> set) {
            std::cout << op << ": ";
            for (Property name : set) std::cout << name << " ";
            std::cout << std::endl;
        }

        void info(std::string const& humanName) {
            std::cout << humanName << std::endl;
            for (auto sell : sellQuestions) {
                std::cout << sell.first << "=" << sell.second << std::endl;
            }
        }

    };

    class TestHuman : public Human {
        static std::map<std::string, TestSelling> tests;
        static unsigned num;
        std::string name;
    public:
        TestHuman(): TestHuman("Czlowiek" + std::to_string(num++)) {}
        TestHuman(std::string const& name_): name(name_) { tests.emplace(name, TestSelling(name)); }

        virtual std::string const& getName() const { return name; }

        virtual bool wantBuy(std::string const& propertyName) {
            // std::cout << name << " wants buy " << propertyName << std::endl;
            test().addBuying(propertyName);
            return true;
        }

        virtual bool wantSell(std::string const& propertyName) {
            // std::cout << name << " wants sell " << propertyName << std::endl;
            test().addSelling(propertyName);
            return true;
        }

        TestSelling& test() {
            try {
		return tests.at(name);
            } catch (std::out_of_range const& e) {
	        std::cerr << "TestHuman: " << name << " not found!" << std::endl;
		tests.emplace(name, TestSelling(name));
                return tests.at(name);
            }
        }

        virtual std::shared_ptr<Human> clone() const {
            return std::shared_ptr<Human>(new TestHuman(name));
        }

        static void reset() {
            num = 0;
            tests.clear();
        }

        static void info() {
            for (auto test : tests) {
                test.second.info(test.first);
            }
        }
    };

    std::map<std::string, TestSelling> TestHuman::tests;
    unsigned TestHuman::num = 0;

    void initHuman(MojaGrubaRyba& mgr, unsigned num = 2) {
        TestHuman::reset();
        while (num--) {
            mgr.addHumanPlayer(HumanPtr(new TestHuman()));
        }
    }

    void testGame(std::string const& test, DiePtr die, std::function<void(MojaGrubaRyba&)> initPlayers, unsigned rounds) {
        std::cout << "Start: " << test << std::endl;
        std::shared_ptr<MojaGrubaRyba> mgr = std::shared_ptr<MojaGrubaRyba>(new MojaGrubaRyba());
        mgr->setDie(die);
        initPlayers(*mgr);
        mgr->play(rounds);
        std::cout << "End: " << test << std::endl;
    }

    void testGameDumb(std::string const& test, Rolls const& pRolls, unsigned pNum, unsigned rounds) {
        auto initPlayers = std::bind(&initDumb, _1, pNum);
        DiePtr die = DiePtr(new TestDie(expandPlayers(pRolls, pNum)));
        testGame(test, die, initPlayers, rounds);
    }

    void testGameSmartass(std::string const& test, Rolls const& pRolls, unsigned pNum, unsigned rounds) {
        auto initPlayers = std::bind(&initSmartass, _1, pNum);
        DiePtr die = DiePtr(new TestDie(expandPlayers(pRolls, pNum)));
        testGame(test, die, initPlayers, rounds);
    }

    void testGameHuman(std::string const& test, Rolls const& pRolls, unsigned pNum, unsigned rounds) {
        auto initPlayers = std::bind(&initHuman, _1, pNum);
        DiePtr die = DiePtr(new TestDie(expandPlayers(pRolls, pNum)));
        testGame(test, die, initPlayers, rounds);
    }

    void testGame2Smartass(std::string const& test, Rolls const& p1Rolls, Rolls const& p2Rolls = {}, unsigned rounds = 1) {
        auto initPlayers = std::bind(&initSmartass, _1, 2);
        DiePtr die = DiePtr(new TestDie(combinePlayers({p1Rolls, p2Rolls})));
        testGame(test, die, initPlayers, rounds);
    }

    class ExampleDie : public Die {
    public:
        unsigned short roll() const {
            static unsigned int rollNum = 0;
            unsigned short rolls[] = { 1, 1, 1, 2, 1, 3 };
            return rolls[rollNum++ % 6];
        }
        std::shared_ptr<Die> clone() const {
            return std::shared_ptr<Die>(new ExampleDie());
        }
    };

    class VeryDumbHuman : public Human {
        std::string name;
    public:
        VeryDumbHuman(std::string const& name_): name(name_) {}

        std::string const& getName() const { return name; }
        bool wantBuy(std::string const& property) { return false; }
        bool wantSell(std::string const& property) { return false; }
        std::shared_ptr<Human> clone() const {
            return std::shared_ptr<Human>(new VeryDumbHuman(name));
        }
    };

    class ConstDie : public Die {
    public:
        unsigned short roll() const {
            return 1;
        }
        std::shared_ptr<Die> clone() const {
            return std::shared_ptr<Die>(new ConstDie());
        }
    };

    void test_tooManyPlayers() {
        std::shared_ptr<MojaGrubaRyba> mgr = std::shared_ptr<MojaGrubaRyba>(new MojaGrubaRyba());
        std::shared_ptr<Die> die = std::shared_ptr<Die>(new ConstDie());
        try {
            mgr->setDie(die);
            for (size_t i = 0; i < test::MAX_PLAYERS; i++) {
                mgr->addComputerPlayer(GrubaRyba::ComputerLevel::DUMB);
            }
            try {
                mgr->addComputerPlayer(GrubaRyba::ComputerLevel::DUMB);
            } catch(TooManyPlayersException const& e) {
                // expected exception
                assert(e.getMax() == test::MAX_PLAYERS);
                return;
            }
        } catch (...) {}
        assert(false);
    }

    void test_noDie(size_t players) {
        std::shared_ptr<MojaGrubaRyba> mgr = std::shared_ptr<MojaGrubaRyba>(new MojaGrubaRyba());
        std::shared_ptr<Die> die = std::shared_ptr<Die>(new ConstDie());
        try {
            for (size_t i = 0; i < players; i++) {
                mgr->addComputerPlayer(GrubaRyba::ComputerLevel::DUMB);
            }
            try {
                mgr->play(1);
            } catch (NoDieException const& e) {
                // expected exception
                return;
            }
        } catch (...) {}
        assert(false);
    }

    void test_tooFewPlayers(size_t players) {
        std::shared_ptr<MojaGrubaRyba> mgr = std::shared_ptr<MojaGrubaRyba>(new MojaGrubaRyba());
        std::shared_ptr<Die> die = std::shared_ptr<Die>(new ConstDie());
        try {
            mgr->setDie(die);
            for (size_t i = 0; i < players; i++) {
                mgr->addComputerPlayer(GrubaRyba::ComputerLevel::DUMB);
            }
            try {
                mgr->play(1);
            } catch (TooFewPlayersException const& e) {
                // expected exception
                assert(e.getMin() == MIN_PLAYERS);
                return;
            }
        } catch (...) {}
        assert(false);
    }

    class RandDie : public Die {
    public:
        unsigned short roll() const { return 1 + rand() % 6; }
        DiePtr clone() const { return DiePtr(new RandDie()); }
    };
}

#endif
