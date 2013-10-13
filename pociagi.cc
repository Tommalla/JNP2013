/* Tomasz Zakrzewski i Maciej Dmowski /
/  JNP 2013/2014, Projekt 1 */
//TODO: przeczyścić i pokomentować kod
#include <cstdio>
#include <climits>
#include <string>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <vector>

using namespace std;

typedef unsigned short int Delay;	//typedef, bo można łatwo zmodyfikować
typedef short int Hour;
typedef unsigned short int TrainNo;
typedef unsigned long long int Result;

const char* RESULT_FORMAT = "%llu\n";
const Delay MAX_DELAY = USHRT_MAX;	//do walidacji opóźnienia

const int BASE = 2048;
const int DAYSIZE = 1440;
//limity na długości stringów
const int TRAIN_ID_LIMIT = 9;
const int DATE_LIMIT = 10;
const int TIME_LIMIT = 5;
const int DELAY_LIMIT = 6;	//nawet trochę na wyrost
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
			
		b /= 2;
		e /= 2;
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


inline const TrainNo queryL(Hour b, Hour e) {
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
	fprintf(stderr, "Error %d: %s", lineNo, line.c_str());
}

inline vector<string> split(const string& s, const char delim) {
	vector<string> res;
	string tmp;
	
	for (int i = 0; i < s.length(); ++i) {
		if (s[i] == delim) {
			res.push_back(tmp);
			tmp.clear();
		} else tmp.push_back(s[i]);
	}
	
	res.push_back(tmp);
	return res;
}

inline const bool isUnsignedNumber(const string& txt) {
	for (int i = 0; i < txt.length(); ++i)
		if (!isdigit(txt[i]))
			return false;
	return true;
}

//waliduje datę
inline const bool validateDate(const string& date) {
	vector<string> s = split(date, '.');
	if (s.size() != 3)
		return false;
	
	unsigned short int tmp[3] = {2, 2, 4};
	for (int i = 0; i < 3; ++i) {
		if (s[i].empty() || !isUnsignedNumber(s[i]) || s[i].length() > tmp[i])
			return false;
		tmp[i] = atoi(s[i].c_str());
	}
	
	time_t rawtime;
	time (&rawtime);
	
	tm timeinfo = *(localtime(&rawtime));
	timeinfo.tm_mday = tmp[0];
	timeinfo.tm_mon = tmp[1] - 1;
	timeinfo.tm_year = tmp[2] - 1900;
	
	rawtime = mktime(&timeinfo);
	
	if (rawtime == -1)
		return false;
	timeinfo = *(localtime(&rawtime));
	if (timeinfo.tm_mday != tmp[0] || timeinfo.tm_mon != tmp[1] - 1 || timeinfo.tm_year != tmp[2] - 1900)
		return false;
	return true;
}

//zwraca <-1, -1>, jeśli godzina niepoprawna,w innym przypadku zwraca <h, m>
inline const pair<Hour, Hour> parseHour(const string& hour) {
	vector<string> s = split(hour, '.');
	if (s.size() != 2)
		return make_pair(-1, -1);
	
	int tmp[2] = {1, 2};
	
	for (int i = 0; i < 2; ++i) {
		if (s[i].length() > 2 || s[i].length() < tmp[i] || !isUnsignedNumber(s[i]))
			return make_pair(-1, -1);
		tmp[i] = atoi(s[i].c_str());
	}
	
	if (tmp[0] > 23 || tmp[1] > 59)
		tmp[0] = tmp[1] = -1;
	
	return make_pair(tmp[0], tmp[1]);
}

inline const Hour convertToMinutes(const Hour& h, const Hour& m) {
	//konwersja z h:m do ilości minut od północy
	return (h * 60 + m) % DAYSIZE;
}

//newline - czy oczekujemy nowej linii?
//jeśli tak, to funkcja tylko do niej przeskoczy
//przy braku danych zwróci pustego stringa
//przy nadmiarze danych zwróci "e"
inline string getNextInputString(string& soFar, int& lineId, const int lengthLimit, const bool& newline = false) {
	//składanie kolejnego inputowego stringa z wejściowego bufora
	static char buffer[BUFFER_SIZE];
	static unsigned short int id, size;
	
	if (lineId == -1) {	//inicjalizacja
		lineId = 0;
		size = fread(buffer, 1, BUFFER_SIZE, stdin);
	}
	
	if (size == 0)
		return "eof";
	
	//przechodzimy przez białe znaki
	while (isspace(buffer[id]) && buffer[id] != '\n') {
		soFar.push_back(buffer[id]);
		++id;
		if (id >= size) {
			if ((size = fread(buffer, 1, BUFFER_SIZE, stdin)) == 0)
				return "eof";
			id = 0;
		}
	}
	
	//kolejny znak, to albo początek stringa, albo znak nowej linii
	if (buffer[id] == '\n') {
		if (newline) {
			soFar.clear();
			lineId++;
			++id;
			if (id >= size) {
				if ((size = fread(buffer, 1, BUFFER_SIZE, stdin)) == 0)
					return "eof";
				id = 0;
			}
		}
		return "";	//niezależnie, czy omijamy nl czy nie, wynik to pusty ciąg
	}
	
	//kolejny znak musi być początkiem stringa
	string res = "";
	while (!isspace(buffer[id]) && res.length() <= lengthLimit) {
		soFar.push_back(buffer[id]);
		res.push_back(buffer[id]);
		++id;
		if (id >= size) {
			if ((size = fread(buffer, 1, BUFFER_SIZE, stdin)) == 0)
				return res;
			id = 0;
		}
	}
	
	if (res.length() > lengthLimit || newline) {	//więcej znaków niż limit, lub znaki, tam gdzie nie powinno ich być
		//soFar += res;
		printError(soFar, lineId);
		while (buffer[id] != '\n') {
			soFar.push_back(buffer[id]);	//FIXME niekoniecznie potrzebne tu, ale konsekwentnie
			fputc(buffer[id], stderr);
			++id;
			if (id >= size) {
				if ((size = fread(buffer, 1, BUFFER_SIZE, stdin)) == 0) {
					fputc('\n', stderr);
					return "e";
				}
				id = 0;
			}
		}
		fputc('\n', stderr);
		return "e";
	} else
		return res;
}

inline void processQueries(const string& lastCommand, string& soFar, int& lineId) {
	string command = lastCommand;
	string data[2];
	
	while (command != "eof") {
		int i = 0;
		for (; i < 2; ++i)
			if ((data[i] = getNextInputString(soFar, lineId, TIME_LIMIT)) == "e")
				break;
			
		if (i == 2 && getNextInputString(soFar, lineId, 0).empty()) {	//na etapie samego czytania wszystko się powiodło
			//walidacja i obsługa zapytań
			pair<Hour, Hour> begin = parseHour(data[0]), end = parseHour(data[1]);

			if (begin.first == -1 || begin.second == -1 || 
				!(command == "L" || command == "M" || command == "S")) {	//dane się nie walidują
				printError(soFar, lineId);
				fprintf(stderr, "\n");
			} else {
				Result res;
				Hour b = convertToMinutes(begin.first, begin.second), 
					e = convertToMinutes(end.first, end.second);
					
				switch (command[0]) {
					case 'L':
						res = queryL(b, e);
						break;
					case 'M':
						res = queryM(b, e);
						break;
					case 'S':
						res = queryS(b, e);
						break;
				}
					
				printf(RESULT_FORMAT, res);
			}
		}
		
		getNextInputString(soFar, lineId, 0, true);	//omiń znak nowej linii
		command = getNextInputString(soFar, lineId, 1);
	}
}

inline void processTrains() {
	//wczytywanie kolejnych pociągów
	string trainId, soFar;
	string data[3];
	unsigned short int limits[3] = {DATE_LIMIT, TIME_LIMIT, DELAY_LIMIT};
	int lineId = -1;
	
	trainId = getNextInputString(soFar, lineId, TRAIN_ID_LIMIT);
	while (trainId != "eof" && trainId != "L" && trainId != "M" && trainId != "S") {	//dopóki coś jest na wejściu i nie jest poleceniem
		if (trainId != "e") {
			int i = 0;
			for (; i < 3; ++i) //wczytywanie
				if ((data[i] = getNextInputString(soFar, lineId, limits[i])) == "e")
					break;
			
			if (i == 3 && getNextInputString(soFar, lineId, 0).empty()) {	//na etapie samego czytania ilość słów
				//pora na walidację					//się zgadza i żadne nie jest dłuższe niż może być
				pair<Hour, Hour> h = parseHour(data[1]);
				int d;
				
				//FIXME usuń debug z końcowej wersji
				//printf("Dotarłem do: %s, %s, %d:%d, %s\n", trainId.c_str(), data[0].c_str(), h.first, h.second, data[2].c_str());
				if (h.first == -1 || !isUnsignedNumber(trainId) || !validateDate(data[0]) || !isUnsignedNumber(data[2]) || 
					(d = atoi(data[2].c_str())) > MAX_DELAY) {	//dane się nie walidują
					printError(soFar, lineId);
					fprintf(stderr, "\n");
				} else	//dane się walidują
					addTrain((convertToMinutes(h.first, h.second) + d) % DAYSIZE, d);
			}
		}
		
		getNextInputString(soFar, lineId, 0, true);	//skocz do nowej linii
		trainId = getNextInputString(soFar, lineId, TRAIN_ID_LIMIT);
		//FIXME: usuń to
		//printf("trainId: %s\n", trainId.c_str());
	}
	
	//jak pojawia się pierwszy nie-pociąg(query)
	if (!trainId.empty()) {
		updateData();
		processQueries(trainId, soFar, lineId);
	}
}

//koniec walidacji

int main() {
	processTrains();	//lubię takie "jednolinijkowe" programy ;D
	return 0;
}
