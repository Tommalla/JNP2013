/**
 * Tomasz Zakrzewski i Mateusz Bałasz
 * Trzecie zadanie z JNP
 */
#include "kontroler.h"

using std::ostream;

Kontroler::operator bool() const {
	return *_numOfAttempts > 0;
}

ostream& operator<<(ostream& output, const Kontroler& k) {
	switch (*k._status) {
		case Status::OK:
			output << "OK\n";
			break;
		case Status::MODIFIED:
			output << "ALARM: ZMANIPULOWANY\n";
			break;
		case Status::ALERT:
			output << "ALARM: WLAMANIE\n";
			break;
	}
	return output;
}

