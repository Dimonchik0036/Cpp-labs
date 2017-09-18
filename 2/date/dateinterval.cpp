#include <sstream>
#include <iomanip>
#include "date.h"

unsigned DateInterval::get_year() const {
	return _time.get_year();
}

unsigned DateInterval::get_month() const {
	return _time.get_month();
}

unsigned DateInterval::get_day() const {
	return _time.get_day();
}

unsigned DateInterval::get_hour() const {
	return _time.get_hour();
}

unsigned DateInterval::get_minute() const {
	return _time.get_minute();
}

unsigned DateInterval::get_second() const {
	return _time.get_second();
}

bool DateInterval::positive() const {
	return _positive;
}

DateInterval::DateInterval(
		bool positive,
		unsigned int year, unsigned int month, unsigned int day, unsigned int hours, unsigned int minutes, unsigned int seconds)
		: _positive(positive), _time(Date(year, month, day, hours, minutes, seconds)) {}

DateInterval::DateInterval(DateInterval const& d) : _positive(d._positive), _time(d._time) {}

DateInterval::DateInterval(bool positive, Date const& interval) : _positive(positive), _time(interval) {}

DateInterval& DateInterval::operator=(DateInterval const& other) {
	if (this == &other) {
		return *this;
	}

	_time = other._time;
	_positive = other._positive;
	return *this;
}

std::string DateInterval::to_string() const {
	std::ostringstream os;
	os << get_year() << "-"
	   << get_month() << "-" << std::setfill('0')
	   << std::setw(2) << get_day() << " "
	   << std::setw(2) << get_hour() << "::"
	   << std::setw(2) << get_minute() << "::"
	   << std::setw(2) << get_second();
	return os.str();
}