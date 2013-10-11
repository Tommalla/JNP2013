/* Tomasz Zakrzewski i Maciej Dmowski /
/  JNP 2013/2014, Projekt 1 */
#include <iostream>
#include <climits>
#include <string>

#define BASE 2048
#define DAYSIZE 1440

using namespace std;

typedef unsigned short int Delay;	//typedef, bo można łatwo zmodyfikować
typedef unsigned short int Hour;
const Delay MAX_DELAY = USHRT_MAX;	//do walidacji opóźnienia

//cała impelementacja drzewa przedziałowego zaczyna się tu
Delay tree[2 * BASE + 10];

inline void insert(Hour h, const Delay &d) {
	//TODO operacja insert w drzewie
}

inline const Delay& query(Hour b, Hour e) {
	//TODO operacja obliczania max opóźnienia na przedziale [b; e] (drzewo przedziałowe)
}

//cała implementacja drzewa przedziałowego kończy się tu

inline void addTrain(const Hour &h, const Delay &d) {
	//TODO dodawanie pociągu do struktur
	//w szczególności do drzewa przedziałowego:
	insert(h, d);
}

inline void printError(const string& line) {
	//TODO wypisywanie błędnej linii
	//w osobnej funkcji dlatego, żeby w razie potrzeby łatwo to było zmienić/użyć w wielu miejscach
}

int main() {
	//TODO parsowanie: 
	//czytamy linię (trzeba jeszcze wymyślić jak to robić sensownie)
	//parsujemy
	//if (niepoprawna linia)
	//	printError(line);
	//else {
	//	if (to jest pociąg)
	//		addTrain(godzina przyjazdu + opóźnienie, opóźnienie);
	//	else
	//		cout << query(pocz, koniec);
	//}
	return 0;
}
