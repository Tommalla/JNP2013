/* Tomasz Zakrzewski i Maciej Dmowski /
/  JNP 2013/2014, Projekt 1 */
#include <iostream>
#include <climits>
#include <string>

using namespace std;

typedef unsigned short int Delay;	//typedef, bo można łatwo zmodyfikować
typedef unsigned short int Hour;
typedef unsigned short int TrainNo;

const Delay MAX_DELAY = USHRT_MAX;	//do walidacji opóźnienia
const int BASE = 2048;
const int DAYSIZE = 1440;
const int STRLEN_LIMIT = 10;
const int BUFFER_SIZE = 8;	//FIXME przemyśl rozmiar bufora

//drzewo przedziałowe typu max
Delay tree[2 * BASE + 10];	//drzewo przedziałowe typu max

inline void insert(Hour h, const Delay &d) {	//dodawanie opóźnienia do drzewa przedz.
	h += BASE;
	
	for (; h > 0 && tree[h] < d; h /= 2)
		tree[h] = d;
}

//operacja obliczania max opóźnienia na przedziale [b; e] (drzewo przedziałowe)
inline const Delay queryM(Hour b, Hour e) {
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

//koniec drzewa przedziałowego

//sumy prefiksowe (l, s)

TrainNo l[DAYSIZE];
Delay s[DAYSIZE];

inline const void addTrainL(const Hour &h,const Delay &d){
  l[h]++; 
}

inline const void addTrainS(const Hour &h, const Delay &d){
  s[h]+=d;
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

inline const void updateData(){
  // Kiedy skonczymy wczytywać dane o pociagach wywolujemy tę funkcje. 
  // Ona reorganizuje nam struktury.
  // Liczy sie tablica prefiksowa l;
  // Liczy sie drzewko podzialowe tree;
  // Liczy sie tablica prefiksowa s;

  updateL();
  updateS();
}


inline const TrainNo& queryL(Hour b, Hour e) {
	//operacja obliczania liczby pociagów które przejechaly przez posterunek na przedziale [b; e] (ogolna idea: l[e]-l[b-1]);
        return l[e] - (b>0 ? l[b-1] : 0);
}

inline const Delay queryS(Hour b, Hour e) {
	//operacja obliczania sumy opóźnień na przedziale [b; e] (Ogólna idea: s[e]-s[b-1]);
        return s[e] - (b>0 ? s[b-1] : 0);
}

//koniec sum prefiksowych


inline void addTrain(const Hour &h, const Delay &d) {
	//dodawanie pociągu do struktur
        
        addTrainL(h,d);
	insert(h, d);
        addTrainS(h,d);
}

//walidacja i parsowanie
inline void printError(const string& line, const int& lineNo) {
	std::cerr<<"Error "<<lineNo<<": "<<line<<"\n";
}

inline const bool isUnsignedNumber(const string& txt) {
	//TODO walidacja numeru
}

inline const bool validateTrainId(const string& trainId) {
	//TODO walidacja id pociągu (max 9 cyfr)
}

inline const bool validateDate(const string& date) {
	//TODO walidacja daty
}

//zwraca <-1, -1>, jeśli godzina niepoprawna,w innym przypadku zwraca <h, m>
inline const pair<Hour, Hour> parseHour(const string& hour) {
	//TODO walidacja godziny i parsowanie
}

inline const Hour convertToMinutes(const Hour& h, const Hour& m) {
	//TODO konwersja z h:m do ilości minut od północy
}

//newline - czy oczekujemy nowej linii?
//jeśli tak, to funkcja tylko do niej przeskoczy
//przy jakimkolwiek błędzie odczytu (brak lub nadmiar danych), funkcja zwróci pustego stringa
inline string getNextInputString(const bool& newline = false) {
	//TODO składanie kolejnego inputowego stringa z wejściowego bufora
	static char buffer[BUFFER_SIZE];
	static unsigned short int id;
	//FIXME: to trzeba głęboko przemyśleć
	//(globalny bufor? statyczna historia buforów? string na historię?)
}

//koniec walidacji

int main() {
	//TODO parsowanie: 
	//czytamy linię (trzeba jeszcze wymyślić jak to robić sensownie)
	//parsujemy
	//if (niepoprawna linia)
	//	printError(line);
	//else {
	//	if (to jest pociąg)
	//		addTrain(godzina przyjazdu, opóźnienie);
	//	else
	//		cout << query(pocz, koniec);
	//}

	return 0;
}
