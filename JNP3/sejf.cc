#include "sejf.h"
#include "kontroler.h"

Sejf::Sejf(Sejf&& other) 
	: _text(other._text)
	, _numOfAttempts(other._numOfAttempts)
	, _status(other._status)
	, _kontroler(other._kontroler)
{
	move(std::move(other));
}

short int Sejf::operator[](const int idx)
{
	if (_numOfAttempts == 0 && idx >= 0 && (size_t)idx < _text.size())
		setAlert();

	if (idx < 0 || (size_t)idx >= _text.size() || _numOfAttempts == 0)
		return -1;

	_numOfAttempts--;
	return (short int)_text[idx];
}

Sejf& Sejf::operator+=(const unsigned int val)
{
	if (val + _numOfAttempts >= _numOfAttempts) {
		_numOfAttempts += val;
		setModified();
	}
	return *this;
}

Sejf& Sejf::operator*=(const unsigned int val)
{
	if (val * _numOfAttempts >= _numOfAttempts) {
		_numOfAttempts *= val;
		setModified();
	}
	return *this;
}

Sejf& Sejf::operator-=(const unsigned int val)
{
	if (_numOfAttempts >= val) {
		_numOfAttempts -= val;
		setModified();
	}
	return *this;
}

Sejf& Sejf::operator=(Sejf&& other)
{
	if (&other != this) {
		_status = other._status;
		_text = other._text;
		_numOfAttempts = other._numOfAttempts;
		move(std::move(other));
	}
	return *this;
}

const Kontroler& Sejf::kontroler() const
{
	return _kontroler;
}


void Sejf::move(Sejf&& other)
{
	other._status = Status::OK;
	other._text = "";
	other._numOfAttempts = 0;
}

void Sejf::setAlert() {
	_status = Status::ALERT;
}

void Sejf::setModified() {
	if (_status != Status::ALERT)
		_status = Status::MODIFIED;
}