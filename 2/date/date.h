#pragma once
#ifndef DATE_H
#define DATE_H

#include <string>
#include <ctime>

class Date;
class DateInterval;

enum Month {
	Jan = 1,
	Feb = 2,
	Mar = 3,
	Apr = 4,
	May = 5,
	Jun = 6,
	Jul = 7,
	Aug = 8,
	Sep = 9,
	Oct = 10,
	Nov = 11,
	Dec = 12,
};

class Date {
public:
	unsigned get_year() const;
	Month get_month() const;
	unsigned get_day() const;
	unsigned get_hour() const;
	unsigned get_minute() const;
	unsigned get_second() const;
	Date();
	Date(unsigned year, unsigned month, unsigned day, unsigned hours, unsigned minutes, unsigned seconds);
	Date(unsigned year, Month m, unsigned day);
	Date(unsigned hours, unsigned minutes, unsigned seconds);
	Date(tm const&);
	Date(Date const&);
	Date& operator=(Date const&);
	Date add_years(int) const;
	Date add_months(int) const;
	Date add_days(int) const;
	Date add_hours(int) const;
	Date add_minutes(int) const;
	Date add_seconds(int) const;
	std::string to_string() const;
	DateInterval get_interval(Date) const;
	Date add_interval (const DateInterval&) const;
private:
	tm _time;
	bool _bissextile(unsigned) const;
	int _get_days_count(Month, bool = false) const;
	void _correction(tm&);
};

class DateInterval {
public:
	unsigned get_year() const;
	unsigned get_month() const;
	unsigned get_day() const;
	unsigned get_hour() const;
	unsigned get_minute() const;
	unsigned get_second() const;
	bool positive() const;
	DateInterval(bool positive, unsigned year, unsigned month, unsigned day, unsigned hours, unsigned minutes, unsigned seconds);
	DateInterval(bool positive, Date const& interval);
	DateInterval(DateInterval const&);
	DateInterval& operator=(DateInterval const&);
	std::string to_string() const;
private:
	Date _time;
	bool _positive;
};

#endif //DATE_H
