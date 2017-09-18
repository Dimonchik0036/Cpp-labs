#include "date.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>

static std::string _month_to_string(Month);
tm *_get_current_time(__time64_t *current_time_);
void _correct(__time64_t *, tm *);

Date::Date() {
	_time_st_ = _get_current_time(&_time_);
	_time_st_->tm_year += 1970;
	_correct(&_time_, _time_st_);
}

unsigned Date::get_year() const {
	return static_cast<unsigned>(_time_st_->tm_year - 70);
}

Month Date::get_month() const {
	return static_cast<Month>(_time_st_->tm_mon + 1);
}

unsigned Date::get_day() const {
	return static_cast<unsigned>(_time_st_->tm_mday);
}

unsigned Date::get_hour() const {
	return static_cast<unsigned>(_time_st_->tm_hour);
}

unsigned Date::get_minute() const {
	return static_cast<unsigned>(_time_st_->tm_min);
}

unsigned Date::get_second() const {
	return static_cast<unsigned>(_time_st_->tm_sec);
}

Date::Date(unsigned year, unsigned month, unsigned day, unsigned hours, unsigned minutes, unsigned seconds) : _time_st_(
		_get_current_time(&_time_)) {
	_time_st_->tm_year = year + 70;
	_time_st_->tm_mon = month - 1;
	_time_st_->tm_mday = day;
	_time_st_->tm_hour = hours;
	_time_st_->tm_min = minutes;
	_time_st_->tm_sec = seconds;
	_correct(&_time_, _time_st_);
}

Date::Date(unsigned year, Month m, unsigned day) : _time_st_(_get_current_time(&_time_)) {
	_time_st_->tm_year = year + 70;
	_time_st_->tm_mon = m - 1;
	_time_st_->tm_mday = day;
	_time_st_->tm_hour = 0;
	_time_st_->tm_min = 0;
	_time_st_->tm_sec = 0;
	_correct(&_time_, _time_st_);
}

Date::Date(unsigned hours, unsigned minutes, unsigned seconds) : _time_st_(_get_current_time(&_time_)) {
	_time_st_->tm_hour = hours;
	_time_st_->tm_min = minutes;
	_time_st_->tm_sec = seconds;
	_correct(&_time_, _time_st_);
}

Date::Date(Date const& date) : _time_st_(new tm(*date._time_st_)) {}

Date& Date::operator=(const Date& date) {
	if (this == &date) {
		return *this;
	}

	_time_st_ = new tm(*date._time_st_);
	return *this;
}

Date Date::add_years(int years) const {
	Date result_;
	result_._time_st_->tm_year = _time_st_->tm_year + years;
	result_._time_st_->tm_mon = _time_st_->tm_mon;
	result_._time_st_->tm_mday = _time_st_->tm_mday;
	result_._time_st_->tm_hour = _time_st_->tm_hour;
	result_._time_st_->tm_min = _time_st_->tm_min;
	result_._time_st_->tm_sec =  _time_st_->tm_sec;
	_correct(&result_._time_,result_._time_st_);

	return result_;
}

Date Date::add_months(int months) const {
	Date result_;
	result_._time_st_->tm_year = _time_st_->tm_year;
	result_._time_st_->tm_mon = _time_st_->tm_mon + months;
	result_._time_st_->tm_mday = _time_st_->tm_mday;
	result_._time_st_->tm_hour = _time_st_->tm_hour;
	result_._time_st_->tm_min = _time_st_->tm_min;
	result_._time_st_->tm_sec =  _time_st_->tm_sec;
	_correct(&result_._time_,result_._time_st_);

	return result_;
}

Date Date::add_days(int days) const {
	Date result_;
	result_._time_st_->tm_year = _time_st_->tm_year;
	result_._time_st_->tm_mon = _time_st_->tm_mon;
	result_._time_st_->tm_mday = _time_st_->tm_mday + days;
	result_._time_st_->tm_hour = _time_st_->tm_hour;
	result_._time_st_->tm_min = _time_st_->tm_min;
	result_._time_st_->tm_sec =  _time_st_->tm_sec;
	_correct(&result_._time_,result_._time_st_);

	return result_;
}

Date Date::add_hours(int hours) const {
	Date result_;
	result_._time_st_->tm_year = _time_st_->tm_year;
	result_._time_st_->tm_mon = _time_st_->tm_mon;
	result_._time_st_->tm_mday = _time_st_->tm_mday;
	result_._time_st_->tm_hour = _time_st_->tm_hour + hours;
	result_._time_st_->tm_min = _time_st_->tm_min;
	result_._time_st_->tm_sec =  _time_st_->tm_sec;
	_correct(&result_._time_,result_._time_st_);

	return result_;
}

Date Date::add_minutes(int minutes) const {
	Date result_;
	result_._time_st_->tm_year = _time_st_->tm_year;
	result_._time_st_->tm_mon = _time_st_->tm_mon;
	result_._time_st_->tm_mday = _time_st_->tm_mday;
	result_._time_st_->tm_hour = _time_st_->tm_hour;
	result_._time_st_->tm_min = _time_st_->tm_min + minutes;
	result_._time_st_->tm_sec =  _time_st_->tm_sec;
	_correct(&result_._time_,result_._time_st_);

	return result_;
}

Date Date::add_seconds(int seconds) const {
	Date result_;
	result_._time_st_->tm_year = _time_st_->tm_year;
	result_._time_st_->tm_mon = _time_st_->tm_mon;
	result_._time_st_->tm_mday = _time_st_->tm_mday;
	result_._time_st_->tm_hour = _time_st_->tm_hour;
	result_._time_st_->tm_min = _time_st_->tm_min;
	result_._time_st_->tm_sec =  _time_st_->tm_sec + seconds;
	_correct(&result_._time_,result_._time_st_);

	return result_;
}

std::string Date::to_string() const {
	std::ostringstream os;
	os << get_year() << "-"
	   << _month_to_string(get_month()) << "-" << std::setfill('0')
	   << std::setw(2) << get_day() << " "
	   << std::setw(2) << get_hour() << "::"
	   << std::setw(2) << get_minute() << "::"
	   << std::setw(2) << get_second();
	return os.str();
}

DateInterval Date::get_interval(Date other) const {
	time_t endTime = _mktime64(other._time_st_);
	time_t beginTime = _mktime64(other._time_st_);
	std::cout << endTime << " " << beginTime << std::endl;
	bool positive = beginTime > endTime;
	time_t interval = positive ? beginTime - endTime : endTime - beginTime;
	tm result = *localtime(&interval);
	result.tm_year += 1900;
	return DateInterval(0,0,0,0,0,0,0);
}

Date::Date(tm* time_) : _time_st_(new tm(*time_)) {}

Date Date::add_interval(const DateInterval& interval) const {
	int uno = interval.positive() ? 1 : -1;
	return this->add_seconds(
			uno * interval.get_second()).add_minutes(
			uno * interval.get_minute()).add_hours(
			uno * interval.get_hour()).add_days(
			uno * interval.get_day()).add_months(
			uno * interval.get_month()).add_years(
			uno * interval.get_year());
}
Date::~Date() {
	delete _time_st_;
}

static std::string _month_to_string(Month m) {
	switch (m) {
		case Jan:
			return "Jan";
		case Feb:
			return "Feb";
		case Mar:
			return "Mar";
		case Apr:
			return "Apr";
		case May:
			return "May";
		case Jun:
			return "Jun";
		case Jul:
			return "Jul";
		case Aug:
			return "Aug";
		case Sep:
			return "Sep";
		case Oct:
			return "Oct";
		case Nov:
			return "Nov";
		case Dec:
			return "Dec";
	}

	return "???";
}

tm *_get_current_time(__time64_t *current_time_) {
	_time64(current_time_);
	return new tm(*_gmtime64(current_time_))	;
}

void _correct(__time64_t *t, tm * tm_) {
	tm *tmp_ = _localtime64(t);
	//std::cout << "Do " << tmp_->tm_year << "  " << tm_->tm_year << std::endl;
	tmp_->tm_year =  tm_->tm_year % 400;
	tmp_->tm_mon = tm_->tm_mon;
	tmp_->tm_mday = tm_->tm_mday;
	tmp_->tm_hour = tm_->tm_hour;
	tmp_->tm_min = tm_->tm_min;
	tmp_->tm_sec =  tm_->tm_sec;
	_mktime64(tmp_);
	//std::cout << _mktime64(tmp_) << " flag" << std::endl;

	tm_->tm_year = tmp_->tm_year - tm_->tm_year % 400 + tm_->tm_year;
	tm_->tm_mon = tmp_->tm_mon;
	tm_->tm_mday = tmp_->tm_mday;
	tm_->tm_hour = tmp_->tm_hour;
	tm_->tm_min = tmp_->tm_min;
	tm_->tm_sec =  tmp_->tm_sec;
}