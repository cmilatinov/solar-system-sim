#include <ctime>
#include <math.h>
#include <string>

#pragma once
class Time
{
private:
	time_t time;


public:
	Time(time_t time);
	Time(int month, int day, int year, int hour, int minute);
	Time(int month, int day, int year);
	Time(float yearsSince1970);
	~Time();

	double secondsSinceJ2000();

	time_t getTime();

	double difference(Time t);

	std::string to_string();

	static Time * J2000();

};

