//Workout.h
//Workout object for Workouts.cpp
//Created by Christian Bernier on 2021-06-14

#ifndef WORKOUT_H_
#define WORKOUT_H_

#include <string>
#include <iostream>
#include <sstream>

class Workout {
public:
	Workout(std::string workoutType, int year, int month, int day, double workoutDuration, double caloriesBurned); //Other
	Workout(std::string workoutType, int year, int month, int day, double workoutDuration, double caloriesBurned, double distanceTravelled); //Walk, Cycle, etc.
	void setIndoor(); //unused currently
	std::string getType();
	double getDuration();
	double getDistance();
	std::string getDistanceStr();
	double getCalories();
	friend std::ostream& operator<<(std::ostream& os, Workout& w);
	std::string csvFormatString(); //dateString type duration calories distance

private:
	std::string type;
	int y, m, d;
	double duration;
	double distance;
	double calories;
	bool indoor;
	std::string const dateString();
	bool isDistanceBased();
};

Workout::Workout(std::string workoutType, int year, int month, int day, double workoutDuration, double caloriesBurned) {
	type = workoutType;
	y = year;
	m = month;
	d = day;
	duration = workoutDuration;
	distance = 0;
	calories = caloriesBurned;
	indoor = false;
}

Workout::Workout(std::string workoutType, int year, int month, int day, double workoutDuration, double caloriesBurned, double distanceTravelled) {
	type = workoutType;
	y = year;
	m = month;
	d = day;
	duration = workoutDuration;
	distance = distanceTravelled;
	calories = caloriesBurned;
	indoor = false;
}

void Workout::setIndoor() {
	indoor = true;
}

std::string Workout::getType(){
	return type;
}

double Workout::getDuration() {
	return duration;
}

double Workout::getDistance() {
	return distance;
}

std::string Workout::getDistanceStr(){
	std::stringstream ss;
	ss << distance;
	return ss.str();
}

double Workout::getCalories() {
	return calories;
}

bool Workout::isDistanceBased() {
	return distance != 0;
}

std::string const Workout::dateString(){
	std::stringstream ss;
	ss << y << "-" << std::setfill('0') << std::setw(2) << m << "-" << std::setfill('0') << std::setw(2) << d;
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, Workout& w){
	os << "Workout" << std::endl
	   << "    " << "Date: " << w.dateString() << std::endl
	   << "    " << w.getType() << std::endl
	   << "    " << w.getDuration() << "min" << std::endl
	   << "    " << (w.isDistanceBased() ? w.getDistanceStr() + "mi" : "Not distance based") << std::endl
	   << "    " << w.getCalories() << "Cal" << std::endl;
	return os;
}

std::string Workout::csvFormatString(){ //dateString type duration calories distance
	std::stringstream ss;
	ss << dateString() << "," << type << "," << duration << "," << calories;
	if(isDistanceBased()){
		ss << "," << distance;
	}
	return ss.str();
}

#endif /* WORKOUT_H_ */
