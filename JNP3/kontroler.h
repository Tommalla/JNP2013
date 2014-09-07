/**
 * Tomasz Zakrzewski i Mateusz Bałasz
 * Trzecie zadanie z JNP
 */
#ifndef KONTROLER_H
#define KONTROLER_H
#include <ostream>

enum class Status {
	OK,
	MODIFIED,
	ALERT
};

class Kontroler final {
	public:
		friend class Sejf;
		friend std::ostream& operator<<(std::ostream& output, const Kontroler& k);

		explicit operator bool() const;
		Kontroler(const Kontroler& other)
			: _status(other._status)
			, _numOfAttempts(other._numOfAttempts)
			{}

	private:
		const Status* _status;
		const unsigned int* _numOfAttempts;

		Kontroler(Status* status, unsigned int* numOfAttempts)
			: _status(status)
			, _numOfAttempts(numOfAttempts)
			{}
};

#endif