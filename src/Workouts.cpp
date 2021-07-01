//Workouts.cpp
//Analyzes Apple HealthKit export.xml file to extract basic workout data for analysis
//Created by Christian Bernier on 2021-06-14

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "Workout.h"

void readFile(std::string filename, std::vector<Workout> &workouts);
bool typeIsWorkout(std::string line);
std::string getPropertyFromLine(std::string line, std::string property);
void populateWorkoutTypes(std::map<std::string, std::string> &types);
void writeToCSV(std::string filename, std::vector<Workout> &workouts);

int main() {
	std::vector<Workout> workouts;
	std::string inputFile = "export.xml";
	std::string outputFile = "allWorkouts.csv";

	readFile(inputFile, workouts);
	writeToCSV(outputFile, workouts);

	return 0;
}

//Reads the XML file and updates the workouts vector
void readFile(std::string filename, std::vector<Workout> &workouts) {
	std::ifstream infile(filename);
	std::string line;

	//This is to detect what type of workout each entry is
	std::string workoutType;
	std::map<std::string, std::string> workoutTypes;
	populateWorkoutTypes(workoutTypes);

	//Loop through XML file
	while (std::getline(infile, line)) {
		std::istringstream iss(line);

		//This is a slight optimization to skip lines that are too short or don't have the W in "Workout"
		if (line.length() < 9 || line.at(2) != 'W') continue;

		//If the line isn't a workout entry, it is skipped
		if (!typeIsWorkout(line)) continue;

		//Uses the activity type map generated above to detect what type of workout the entry is
		std::string type = workoutTypes[getPropertyFromLine(line, "workoutActivityType")];
		if (type.empty()) type = "unknown";

		//Data from the workout entry
		double duration = stod(getPropertyFromLine(line, "duration"));
		double totalDistance = stod(getPropertyFromLine(line, "totalDistance"));
		double totalEnergyBurned = stod(getPropertyFromLine(line, "totalEnergyBurned"));
		std::string startDate = getPropertyFromLine(line, "startDate");
		int year = stoi(startDate.substr(0, 4));
		int month = stoi(startDate.substr(5, 2));
		int date = stoi(startDate.substr(8, 2));

		//Creates a new Workout instance for this entry and add it to the array
		Workout w(type, year, month, date, duration, totalEnergyBurned, totalDistance);
		workouts.push_back(w);
	}
}

//Used to detect if the line parameter is a workout entry
bool typeIsWorkout(std::string line) {
	//The line looks something like this: <Workout ...>
	//This function extracts just the "Workout" part to determine if it's an opening Workout tag
	int openingBracket = line.find("<");
	std::string fromOpeningBracket = line.substr(openingBracket + 1);
	int nextSpace = fromOpeningBracket.find(" ");
	int closingBracket = fromOpeningBracket.find(">");
	int lengthOfProperty = (nextSpace == -1) ? closingBracket : nextSpace;
	std::string property = fromOpeningBracket.substr(0, lengthOfProperty);

	return property.compare("Workout") == 0;
}

//This function returnns the value for any property submitted for a given workout entry (line parameter)
std::string getPropertyFromLine(std::string line, std::string property) {
	int indexOfProperty = line.find(property);
	if (indexOfProperty == -1) return "ERR: can't find property";

	std::string lineFromProperty = line.substr(indexOfProperty + property.length() + 2);

	int closingQuote = lineFromProperty.find("\"");
	if (closingQuote == -1) return "ERR: can't find closing quote";

	return lineFromProperty.substr(0, closingQuote);
}

//This function populates the activity types map so it can be used
void populateWorkoutTypes(std::map<std::string, std::string> &types) {
	//All activity types are stored in activity_types.txt
	std::ifstream infile("activity_types.txt");
	std::string line;

	while (std::getline(infile, line)) {
		std::istringstream iss(line);

		//All workout types are formatted as such: "HKWorkoutActivityTypeWalking walking"
		int indexOfSpace = line.find(" ");
		if (indexOfSpace < 1 || indexOfSpace >= line.length()) continue;
		std::string appleActivityType = line.substr(0, indexOfSpace);
		std::string shortActivityType = line.substr(indexOfSpace + 1);

		//Each activity type is inserted as a pair so it can be looked up using a key-value relationship
		types.insert(std::pair<std::string, std::string>(appleActivityType, shortActivityType));
	}
}

//This writes the workouts data to a CSV file
void writeToCSV(std::string filename, std::vector<Workout> &workouts) {
	std::ofstream csv;
	csv.open(filename);

	//CSV header
	csv << "date,type,duration,calories,distance";

	//CSV data
	for (int i = 0; i < workouts.size(); i++) {
		csv << std::endl << workouts.at(i).csvFormatString();
	}

	csv.close();
}
