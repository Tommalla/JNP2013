//
//  grubaryba_tests.cc
//

#include <iostream>
#include <memory>
#include "grubaryba_tests_helper.h"

using namespace test;

int main(int argc, const char * argv[]) {
    // test z tresci zadania
     #if TEST_NUM == 100
    std::shared_ptr<MojaGrubaRyba> mgr = std::shared_ptr<MojaGrubaRyba>(new MojaGrubaRyba());
    std::shared_ptr<Die> die = std::shared_ptr<Die>(new ExampleDie());
    std::shared_ptr<Human> albin = std::shared_ptr<Human>(new VeryDumbHuman("Albina"));
    mgr->setDie(die);
    mgr->addComputerPlayer(GrubaRyba::ComputerLevel::DUMB);
    mgr->addComputerPlayer(GrubaRyba::ComputerLevel::SMARTASS);
    mgr->addHumanPlayer(albin);
    mgr->play(100);
    #endif

    // testy wyjatkow
    #if TEST_NUM == 200
    test_tooManyPlayers();
    #endif

    #if TEST_NUM == 201
    for (size_t i = test::MIN_PLAYERS; i <= test::MAX_PLAYERS; i++) {
        test_noDie(i);
    }
    #endif

    #if TEST_NUM == 202
    for (size_t i = 0; i < test::MIN_PLAYERS; i++) {
        test_tooFewPlayers(i);
    }
    #endif

    // proste testy na wszystkie pola
    #if TEST_NUM == 300
    test::testGame2Smartass("Test: Wyspa", {2});
    #endif
    #if TEST_NUM == 301
    test::testGame2Smartass("Test: Blazenki", {9});
    #endif
    #if TEST_NUM == 302
    test::testGame2Smartass("Test: Rekin", {11});
    #endif
    #if TEST_NUM == 303
    test::testGame2Smartass("Test: Start-Ladowanie", {12});
    #endif
    #if TEST_NUM == 304
    test::testGame2Smartass("Test: Start-Przejscie", {14});
    #endif
    #if TEST_NUM == 305
    test::testGame2Smartass("Test: Akwarium", {4}, {}, 4);
    #endif
    #if TEST_NUM == 306
    test::testGame2Smartass("Test: Anemonia", {13});
    #endif
    #if TEST_NUM == 307
    test::testGame2Smartass("Test: Aporina", {3});
    #endif
    #if TEST_NUM == 308
    test::testGame2Smartass("Test: Grota", {5});
    #endif
    #if TEST_NUM == 309
    test::testGame2Smartass("Test: Menella", {6});
    #endif
    #if TEST_NUM == 310
    test::testGame2Smartass("Test: Statek", {8});
    #endif
    #if TEST_NUM == 311
    test::testGame2Smartass("Test: Pennatula", {10});
    #endif

    // testy logiki graczy
    #if TEST_NUM >=400 && TEST_NUM < 500
    Rolls basicRolls = toRolls({APORINA, GROTA, STATEK, PENNATULA, ANEMONIA, MENELLA, START});
    #endif
    #if TEST_NUM == 400
    test::testGameDumb("Test: DUMB", basicRolls, 8, 12);
    #endif
    #if TEST_NUM == 401
    test::testGameSmartass("Test: SMARTASS", basicRolls, 8, 12);
    #endif
    #if TEST_NUM == 402
    TestHuman::reset();
    test::testGameHuman("Test: HUMAN (sprzedaz podczas kupna)", basicRolls, 2, 20);
    TestHuman::info();
    #endif
    #if TEST_NUM == 403
    TestHuman::reset();
    Rolls feeRollsP1 = toRolls({APORINA, STATEK}, 100);
    Rolls feeRollsP2 = toRolls({STATEK, STATEK}, 100);
    DiePtr die = DiePtr(new TestDie(combinePlayers({feeRollsP1, feeRollsP2})));
    auto initPlayer = std::bind(&test::initHuman, _1, 2);
    test::testGame("Test: HUMAN (sprzedaz podczas oplat)", die, initPlayer, 50);
    TestHuman::info();
    #endif

    // testy bankructwa
    #if TEST_NUM == 500
    testGameSmartass("Test: Bankructwo przy oplacie", toRolls({STATEK}, 50), 2, 50);
    #endif
    #if TEST_NUM == 501
    testGameDumb("Test: Bankructwo przy karze", toRolls({REKIN}, 100), 2, 50);
    #endif
    #if TEST_NUM == 502
    // bankructwo przy przejsciu przez depozyt (ladowanie na neutralnym polu!)
    Rolls p1 = toRolls({GROTA, MENELLA}, 28);
    Rolls p2 = toRolls({PENNATULA, MENELLA}, 27);
    p2.push_back(PENNATULA-MENELLA);
    DiePtr die = DiePtr(new TestDie(combinePlayers({p1, p2})));
    auto initPlayers = std::bind(&initSmartass, _1, 2);
    testGame("Test5_3: Bankructwo przy depozycie (wlasne)", die, initPlayers, 30);
    #endif
    #if TEST_NUM == 503
    // bankructwo przy przejsciu przez depozyt (ladowanie na Blazenkach (nagroda)!)
    Rolls p1 = toRolls({GROTA, MENELLA}, 28);
    Rolls p2 = toRolls({PENNATULA, MENELLA}, 27);
    p2.push_back(BLAZENKI-MENELLA);
    DiePtr die = DiePtr(new TestDie(combinePlayers({p1, p2})));
    auto initPlayers = std::bind(&initSmartass, _1, 2);
    testGame("Test5_3: Bankructwo przy depozycie (nagroda)", die, initPlayers, 30);
    #endif

    // testy ponownego uruchamiania gry (losowa kostka)
    #if TEST_NUM == 600
    srand(123456);
    TestHuman::reset();
    std::shared_ptr<MojaGrubaRyba> mgr = std::shared_ptr<MojaGrubaRyba>(new MojaGrubaRyba());
    mgr->setDie(DiePtr(new RandDie()));
    mgr->addComputerPlayer(GrubaRyba::ComputerLevel::DUMB);
    mgr->addComputerPlayer(GrubaRyba::ComputerLevel::SMARTASS);
    mgr->addHumanPlayer(HumanPtr(new TestHuman()));
    mgr->play(50);
    TestHuman::info();
    srand(123456);
    TestHuman::reset();
    mgr->play(50);
    TestHuman::info();
    #endif
}
