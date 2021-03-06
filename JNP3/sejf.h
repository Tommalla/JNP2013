#ifndef SEJF_H
#define SEJF_H

#include <string>
#include "kontroler.h"

class Kontroler;

class Sejf final
{
public:
	friend class Kontroler;

	Sejf(const std::string& text, unsigned int numOfAttempts = 42)
		: _text(text)
		, _numOfAttempts(numOfAttempts)
		, _status(Status::OK)
		, _kontroler(&_status, &_numOfAttempts)
		{
		}
	Sejf(Sejf&& other);

public:
	short int operator[](const int idx);
	Sejf& operator+=(const unsigned int val);
	Sejf& operator-=(const unsigned int val);
	Sejf& operator*=(const unsigned int val);
	Sejf& operator=(Sejf&& other);
	const Kontroler& kontroler() const;

private:
	std::string _text;
	unsigned int _numOfAttempts;
	Status _status;
	const Kontroler _kontroler;

	void move(Sejf&& other);
	void setAlert();
	void setModified();
};

#endif // SEJF_H
