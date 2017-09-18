#include "date.h"

#include <sstream>
#include <iomanip>
#include <iostream>

static std::string _month_to_string(Month);
static tm _get_current_time();

Date::Date() : _time(_get_current_time()) {
	_time.tm_year += 1900;
}

unsigned Date::get_year() const {
	return static_cast<unsigned>(_time.tm_year);
}

Month Date::get_month() const {
	return static_cast<Month>(_time.tm_mon + 1);
}

unsigned Date::get_day() const {
	return static_cast<unsigned>(_time.tm_mday);
}

unsigned Date::get_hour() const {
	return static_cast<unsigned>(_time.tm_hour);
}

unsigned Date::get_minute() const {
	return static_cast<unsigned>(_time.tm_min);
}

unsigned Date::get_second() const {
	return static_cast<unsigned>(_time.tm_sec);
}

Date::Date(unsigned year, unsigned month, unsigned day, unsigned hours, unsigned minutes, unsigned seconds) : _time(
		_get_current_time()) {
	_time.tm_year = year;
	_time.tm_mon = month - 1;
	_time.tm_mday = day;
	_time.tm_hour = hours;
	_time.tm_min = minutes;
	_time.tm_sec = seconds;
	_correction(_time);
}

Date::Date(unsigned year, Month m, unsigned day) : _time(_get_current_time()) {
	_time.tm_year = year;
	_time.tm_mon = m - 1;
	_time.tm_mday = day;
	_time.tm_hour = 0;
	_time.tm_min = 0;
	_time.tm_sec = 0;
	_correction(_time);
}

Date::Date(unsigned hours, unsigned minutes, unsigned seconds) : _time(_get_current_time()) {
	_time.tm_hour = hours;
	_time.tm_min = minutes;
	_time.tm_sec = seconds;
	_correction(_time);
}

Date::Date(Date const& date) : _time(date._time) {}

Date& Date::operator=(const Date& date) {
	if (this == &date) {
		return *this;
	}

	_time = date._time;
	return *this;
}

Date Date::add_years(int years) const {
	if ((years < 0) && (abs(years) > get_year())) {
		return Date(1, Jan, 1);
	}

	return Date(get_year() + years, get_month(), get_day(), get_hour(), get_minute(), get_second());
}

Date Date::add_months(int months) const {
	if ((months < 0) && (abs(months) > get_month() - 1)) {
		return Date(get_year(), 12 + get_month() + months % 12, get_day(), get_hour(), get_minute(),
					get_second()).add_years((months - 12) / 12);
	}

	return Date(get_year(), get_month() + months, get_day(), get_hour(), get_minute(), get_second());
}

Date Date::add_days(int days) const {
	if ((days < 0) && (abs(days) > get_day())) {
		int count = _get_days_count(get_month(), _bissextile(get_year()));
		return Date(get_year(), get_month(), count + get_day() + days % count, get_hour(), get_minute(),
					get_second()).add_months((days - count) / count);
	}

	return Date(get_year(), get_month(), get_day() + days, get_hour(), get_minute(), get_second());
}

Date Date::add_hours(int hours) const {
	if ((hours < 0) && (abs(hours) > get_hour())) {
		return Date(get_year(), get_month(), get_day(), 24 + get_hour() + hours % 24, get_minute(),
					get_second()).add_days((hours - 24) / 24);
	}

	return Date(get_year(), get_month(), get_day(), get_hour() + hours, get_minute(), get_second());
}

Date Date::add_minutes(int minutes) const {
	if ((minutes < 0) && (abs(minutes) > get_minute())) {
		return Date(get_year(), get_month(), get_day(), get_hour(), 60 + get_minute() + minutes % 60,
					get_second()).add_hours((minutes - 60) / 60);
	}

	return Date(get_year(), get_month(), get_day(), get_hour(), get_minute() + minutes, get_second());
}

Date Date::add_seconds(int seconds) const {
	if ((seconds < 0) && (abs(seconds) > get_second())) {
		return Date(get_year(), get_month(), get_day(), get_hour(), get_minute(),
					60 + get_second() + seconds % 60).add_minutes((seconds - 60) / 60);
	}

	return Date(get_year(), get_month(), get_day(), get_hour(), get_minute(), get_second() + seconds);
}

bool Date::_bissextile(unsigned year) const {
	return !(year % 4) && (year % 100) || !(year % 400);
}

int Date::_get_days_count(Month m, bool bissextile) const {
	switch (m) {
		case Jan:
			return 31;
		case Feb:
			return bissextile ? 29 : 28;
		case Mar:
			return 31;
		case Apr:
			return 30;
		case May:
			return 31;
		case Jun:
			return 30;
		case Jul:
			return 31;
		case Aug:
			return 31;
		case Sep:
			return 30;
		case Oct:
			return 31;
		case Nov:
			return 30;
		case Dec:
			return 31;
	}

	return 0;
}

void Date::_correction(tm& t) {
	t.tm_min += t.tm_sec / 60;
	t.tm_sec %= 60;

	t.tm_hour += t.tm_min / 60;
	t.tm_min %= 60;

	t.tm_mday += t.tm_hour / 24;
	t.tm_hour %= 24;

	t.tm_year += t.tm_mon / 12;
	t.tm_mon %= 12;

	int count;
	while (t.tm_mday / (count = _get_days_count(Month(t.tm_mon + 1), _bissextile(t.tm_year)))) {
		if (++t.tm_mon / 12) {
			++t.tm_year;
			t.tm_mon = 0;
		}

		t.tm_mday -= count;
	}

	if (t.tm_year > 9999) {
		t.tm_year = 9999;
		t.tm_mon = 11;
		t.tm_mday = 31;
		t.tm_hour = 23;
		t.tm_min = 59;
		t.tm_sec = 59;
	}
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
	time_t endTime = mktime(&other._time);
	time_t beginTime = mktime(&other._time);
	std::cout << endTime << " " << beginTime << std::endl;
	bool positive = beginTime > endTime;
	time_t interval = positive ? beginTime - endTime : endTime - beginTime;
	tm result = *localtime(&interval);
	result.tm_year += 1900;
	return DateInterval(positive, result);
}

Date::Date(tm const& time) : _time(time) {}

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

static tm _get_current_time() {
	time_t currentTime;
	time(&currentTime);
	return *gmtime(&currentTime);
}