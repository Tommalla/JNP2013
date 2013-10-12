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
typedef unsigned short int TrainNo;

const Delay MAX_DELAY = USHRT_MAX;	//do walidacji opóźnienia

Delay tree[2 * BASE + 10];	//drzewo przedziałowe typu max

inline void insert(Hour h, const Delay &d) {	//dodawanie opóźnienia do drzewa przedz.
	h += BASE;
	
	for (; h > 0 && tree[h] < d; h /= 2)
		tree[h] = d;
}

TrainNo l[DAYSIZE];
Delay s[DAYSIZE];

inline const void addTrainL(const Hour &h,const Delay &d){
  l[(h+d)%DAYSIZE]++; 
}
inline const void addTrainS(const Hour &h, const Delay &d){
  s[(h+d)%DAYSIZE]+=d;
}

inline const void updateData() {
  // Kiedy skonczymy wczytywać dane o pociagach wywolujemy tę funkcje. 
  // Ona reorganizuje nam struktury.
  // Liczy sie tablica prefiksowa l;
  // Liczy sie drzewko podzialowe tree;
  // Liczy sie tablica prefiksowa s;

  updateL();
  updateS();
}

inline const void updateL(){
  int suma =0;
  for(int i=0;i<DAYSIZE;i++){
    suma+=l[i];
    l[i]=suma;
  }
}

inline const void updateS(){
  int suma = 0;
  for (int i=0;i<DAYSIZE;i++){
    suma+=s[i];
    s[i]=suma;
  }
}

inline const TrainNo& queryL(Hour b, Hour e) {
	//operacja obliczania liczby pociagów które przejechaly przez posterunek na przedziale [b; e] (ogolna idea: l[e]-l[b-1]);
        return l[e] - (b>0 ? l[b-1] : 0);
}

//operacja obliczania max opóźnienia na przedziale [b; e] (drzewo przedziałowe)
inline const Delay& queryM(Hour b, Hour e) {
	b += BASE;
	e += BASE;
	
	Delay res = max(tree[b], tree[e]);
	
	while (b / 2 != e / 2) {
		if (b % 2 == 0)
			res = max(res, tree[b + 1]);	//lewy kraniec, bierzemy prawego brata
		if (e % 2 == 1)
			res = max(res, tree[e - 1]);	//prawy kraniec, bierzemy lewego brata
	}
	
	return res;
}

inline const Delay& queryS(Hour b, Hour e) {
	//operacja obliczania sumy opóźnień na przedziale [b; e] (Ogólna idea: s[e]-s[b-1]);
        return s[e] - (b>0 ? s[b-1] : 0);

}

inline void addTrain(const Hour &h, const Delay &d) {
	//dodawanie pociągu do struktur
        
        addTrainL(h,d);
	insert(h, d);
        addTrainS(h,d);
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
