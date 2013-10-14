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

//typedef, bo można łatwo zmodyfikować
typedef unsigned short int Delay;
typedef short int Hour;
typedef unsigned long long int TrainNo;
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
const int BUFFER_SIZE = 100000;	//http://stackoverflow.com/a/12759003

//drzewo przedziałowe typu max

inline void insert(vector<Delay> &tree, Hour h, const Delay &d) {	//dodawanie opóźnienia do drzewa przedz.
	h += BASE;
	
	for (; h > 0 && tree.at(h) < d; h /= 2)
		tree.at(h) = d;
}

//operacja obliczania max opóźnienia na przedziale [b; e] (drzewo przedziałowe)
inline const Delay queryM(vector<Delay> &tree, Hour b, Hour e) {
	b += BASE;
	e += BASE;
	
	Delay res = max(tree.at(b), tree.at(e));
	
	while (b / 2 != e / 2) {
		if (b % 2 == 0)
			res = max(res, tree.at(b + 1));	//lewy kraniec, bierzemy prawego brata
		if (e % 2 == 1)
			res = max(res, tree.at(e - 1));	//prawy kraniec, bierzemy lewego brata
			
		b /= 2;
		e /= 2;
	}
	
	return res;
}

//koniec drzewa przedziałowego

//sumy prefiksowe (l, s)


//liczenie sum w tablicach
inline const void addTrainL(vector<TrainNo> &l, const Hour &h,const Delay &d){
	l.at(h)++; 
}

inline const void addTrainS(vector<Result> &s, const Hour &h, const Delay &d){
	s.at(h) += d;
}

//przeliczanie tablic
inline const void updateL(vector<TrainNo> &l) {
	for(int i = 1; i < DAYSIZE; i++)
		l.at(i) += l.at(i - 1);
}

inline const void updateS(vector<Result> &s) {
	for (int i = 1; i < DAYSIZE; i++)
		s.at(i) += s.at(i - 1);
} 

inline const void updateData(vector<TrainNo> &l, vector<Result> &s){
  // Kiedy skonczymy wczytywać dane o pociagach wywolujemy tę funkcje. 
  // Ona reorganizuje nam struktury.
  // Liczy sie tablica prefiksowa l;
  // Liczy sie tablica prefiksowa s;

  updateL(l);
  updateS(s);
}

//operacja obliczania liczby pociagów które przejechaly przez posterunek na przedziale [b; e] (ogolna idea: l[e]-l[b-1]);
inline const TrainNo queryL(vector<TrainNo> &l, Hour b, Hour e) {
        return l[e] - (b > 0 ? l[b - 1] : 0);
}

//operacja obliczania sumy opóźnień na przedziale [b; e] (Ogólna idea: s[e]-s[b-1]);
inline const Result queryS(vector<Result> &s, Hour b, Hour e) {
        return s[e] - (b > 0 ? s[b - 1] : 0);
}

//koniec sum prefiksowych

//dodawanie pociągu do struktur
inline void addTrain(vector<TrainNo> &l, vector<Delay> &tree, vector<Result> &s, const Hour &h, const Delay &d) {
        addTrainL(l, h, d);
	insert(tree, h, d);
        addTrainS(s, h, d);
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
		} else 
			tmp.push_back(s[i]);
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
	
	tm timeinfo;
	timeinfo.tm_sec = timeinfo.tm_min = timeinfo.tm_hour = 12;
	timeinfo.tm_mday = tmp[0];
	timeinfo.tm_mon = tmp[1] - 1;
	timeinfo.tm_year = tmp[2] - 1900;
	
	time_t rawtime = mktime(&timeinfo);
	
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

//konwersja z h:m do ilości minut od północy
inline const Hour convertToMinutes(const Hour& h, const Hour& m) {
	return (h * 60 + m) % DAYSIZE;
}

//czyta bufor od nowa jeśli trzeba
inline bool checkBuffer(char* buffer, unsigned short int& size, unsigned short int &id) {
	if (id >= size) {
		if ((size = fread(buffer, 1, BUFFER_SIZE, stdin)) == 0)
			return false;
		id = 0;
	}
	
	return true;
}

//funkcja do wczytywania kolejnego stringa z wejścia
//omija białe znaki poza \n, 
//newline - czy oczekujemy nowej linii? -> jeśli tak, to funkcja tylko do niej przeskoczy
//przy braku danych zwróci pustego stringa
//przy nadmiarze danych zwróci "e" i wypisze błąd na stderr
//przy końcu pliku zwróci "eof"
inline string getNextInputString(string& soFar, int& lineId, const int lengthLimit, const bool& newline = false) {
	//składanie kolejnego inputowego stringa z wejściowego bufora
	static char buffer[BUFFER_SIZE];
	static unsigned short int id, size;
	
	if (lineId == -1) {	//inicjalizacja
		lineId = 1;
		size = fread(buffer, 1, BUFFER_SIZE, stdin);
	}
	
	if (size == 0)
		return "eof";
	
	//przechodzimy przez białe znaki
	while (isspace(buffer[id]) && buffer[id] != '\n') {
		soFar.push_back(buffer[id]);
		if (!checkBuffer(buffer, size, ++id))
			return "eof";
	}
	
	//kolejny znak to albo początek stringa, albo znak nowej linii
	if (buffer[id] == '\n') {
		if (newline) {
			soFar.clear();
			lineId++;
			if (!checkBuffer(buffer, size, ++id))
				return "eof";
		}
		return "";	//niezależnie, czy omijamy nl czy nie, wynik to pusty ciąg
	}
	
	//kolejny znak musi być początkiem stringa
	string res;
	while (!isspace(buffer[id]) && res.length() <= lengthLimit) {
		soFar.push_back(buffer[id]);
		res.push_back(buffer[id]);
		if (!checkBuffer(buffer, size, ++id))
			return res;
	}
	
	if (res.length() > lengthLimit || newline) {	//więcej znaków niż limit, lub znaki, tam gdzie nie powinno ich być
		printError(soFar, lineId);
		for (;checkBuffer(buffer, size, id) && buffer[id] != '\n'; ++id) {
			soFar.push_back(buffer[id]);	//niekoniecznie potrzebne tu, ale konsekwentnie
			fputc(buffer[id], stderr);
		}
		fputc('\n', stderr);
		return "e";
	} else
		return res;
}

inline void processQueries(vector<TrainNo> &l, vector<Delay> &tree, vector<Result> &s, const string& lastCommand, string& soFar, int& lineId) {
	string command = lastCommand;
	string data[2];
	
	while (command != "eof") {
		if (command != "e") {
			int i = 0;
			for (; i < 2; ++i)
				if ((data[i] = getNextInputString(soFar, lineId, TIME_LIMIT)) == "e")
					break;
			
			if (i == 2 && getNextInputString(soFar, lineId, 0).empty()) {	//na etapie samego czytania wszystko się powiodło
				//walidacja i obsługa zapytań
				pair<Hour, Hour> begin = parseHour(data[0]), end = parseHour(data[1]);

				if (begin.first == -1 || begin.second == -1 || begin.first > end.first || 
					(begin.first == end.first && begin.second > end.second) ||
					!(command == "L" || command == "M" || command == "S")) {	//dane się nie walidują
					printError(soFar, lineId);
					fprintf(stderr, "\n");
				} else {
					Result res;
					Hour b = convertToMinutes(begin.first, begin.second), 
						e = convertToMinutes(end.first, end.second);
						
					switch (command[0]) {
						case 'L':
							res = queryL(l, b, e);
							break;
						case 'M':
							res = queryM(tree, b, e);
							break;
						case 'S':
							res = queryS(s, b, e);
							break;
					}
						
					printf(RESULT_FORMAT, res);
				}
			}
		}
		getNextInputString(soFar, lineId, 0, true);	//omiń znak nowej linii
		command = getNextInputString(soFar, lineId, 1);
	}
}

inline void processTrains(vector<TrainNo> &l, vector<Delay> &tree, vector<Result> &s){
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
				
				if (h.first == -1 || !isUnsignedNumber(trainId) || !validateDate(data[0]) || !isUnsignedNumber(data[2]) || 
					(d = atoi(data[2].c_str())) > MAX_DELAY) {	//dane się nie walidują
					printError(soFar, lineId);
					fprintf(stderr, "\n");
				} else	//dane się walidują
					addTrain(l,tree,s,(convertToMinutes(h.first, h.second) + d) % DAYSIZE, d);
			}
		}
		
		getNextInputString(soFar, lineId, 0, true);	//skocz do nowej linii
		trainId = getNextInputString(soFar, lineId, TRAIN_ID_LIMIT);
	}
	
	//jak pojawia się pierwszy nie-pociąg(query)
	if (!trainId.empty()) {
		updateData(l, s);
		processQueries(l, tree, s, trainId, soFar, lineId);
	}
}

//koniec walidacji

int main() {
        
        vector<Delay> tree;	//drzewo przedziałowe typu max
        vector<TrainNo> l;
        vector<Result> s;

	tree.resize(2 * BASE);
	l.resize(DAYSIZE);
	s.resize(DAYSIZE);
        
	processTrains(l,tree, s);
	return 0;
}
