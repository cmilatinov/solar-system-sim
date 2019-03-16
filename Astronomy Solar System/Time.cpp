#include "Time.h"

Time::Time(time_t time){
	this->time = time;
}

Time::Time(int month, int day, int year, int hour, int min) {
	struct tm timeStruct; 
	timeStruct.tm_year = year - 1900;
	timeStruct.tm_mday = day;
	timeStruct.tm_mon = month - 1;
	timeStruct.tm_hour = hour;
	timeStruct.tm_min = min;
	timeStruct.tm_sec = 0;
	this->time = mktime(&timeStruct);
}

Time::Time(int month, int day, int year) {
	struct tm timeStruct;
	timeStruct.tm_year = year - 1900;
	timeStruct.tm_mday = day;
	timeStruct.tm_mon = month - 1;
	timeStruct.tm_hour = 0;
	timeStruct.tm_min = 0;
	timeStruct.tm_sec = 0;
	this->time = mktime(&timeStruct);
}

Time::Time(float yearsSinceJ200){
	int years = yearsSinceJ200 - fmod(yearsSinceJ200, 1.0f);
	int days = fmod(yearsSinceJ200, 1.0f) * 365.25f;
	struct tm time;
	time.tm_year = 100 + years;
	time.tm_mday = days + 2;
	time.tm_mon = 0;
	time.tm_hour = 0;
	time.tm_min = 0;
	time.tm_sec = 0;
	this->time = mktime(&time);
}

Time::~Time(){
	
}

double Time::secondsSinceJ2000(){
	return 0;
}

time_t Time::getTime(){
	return this->time;
}

double Time::difference(Time t){
	return difftime(this->time, t.getTime());
}

Time * Time::J2000(){
	return new Time(1, 1, 2000, 12, 0);
}

std::string Time::to_string() {
	std::tm * ptm = std::localtime(&time);
	char buffer[32];
	
	// Format: 15.06.2009 20:20:00
	std::strftime(buffer, 32, "%b %e, %Y", ptm);
	return buffer;
}