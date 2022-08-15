#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const unsigned int DEFAULT_SIZE = 179;
// Definition of structure course
struct Course {

	string courseNumber;
	string courseName;
	vector<string> prerequisites;
};

class HashTable{

	// Define structures to hold courses
	struct Node {
		Course course;
		unsigned int key;
		Node* next;

		// default constructor
		Node() {
			key = UINT_MAX;
			next = nullptr;
		}

		// initialize with a bid
		Node(Course aCourse) : Node() {
			course = aCourse;
		}

		// initialize with a bid and a key
		Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
			key = aKey;
		}
	};

	vector<Node> nodes;

	unsigned int tableSize = DEFAULT_SIZE;
	unsigned int hash(int key);

public:
	HashTable();
	virtual ~HashTable();
	void Insert(Course course);
	void PrintAll();
	Course Search(string course);
};

//default Constructor
HashTable::HashTable() {
	nodes.resize(tableSize);                                            // Initalize node structure by resizing tableSize
}

//destructor
HashTable::~HashTable() {
	nodes.erase(nodes.begin());                                         // erase nodes beginning
}

//getting hash value for key
unsigned int HashTable::hash(int key) {
	return key % tableSize;												// return key tableSize
}

//inserting course 
void HashTable::Insert(Course course) {
	//Converting courseNumber in to ASCII for hashing
	int tempInt = 0;
	string tempStr = "";
	for (int i = 0; i < course.courseNumber.length(); i++) {
		tempInt = course.courseNumber[i];
		tempStr += to_string(tempInt);
	}

	unsigned key = hash(atoll(tempStr.c_str()));						// create the key for the given bid

	Node* rNode = &(nodes.at(key));										// retrieve node using key
	if (rNode == nullptr) {												// if no entry found for the key
		Node* newNode = new Node(course, key);							// setting up new node
		nodes.insert(nodes.begin() + key, (*newNode));					// assign this node to the key position
	} else if (rNode->key == UINT_MAX) {									// else if node is not used
		rNode->key = key;												// set to key
		rNode->course = course;                                         // set old node to bid
		rNode->next = nullptr;											// set old node next to null pointer
	} else {																// else find the next open node
		rNode->next = new Node(course, key);							// add new newNode to end
	}
}

// function to split course line 
vector<string>splitCourse(string raw) {
	
	vector<string> courses;								// Var to hold course info
	string delim = ",";									// delimiter to split string by

	int start = 0;										
	int end = raw.find(delim);

	while (end != -1) {												// while position isnt equal to string length 
		courses.push_back(raw.substr(start, end - start));			// adding from string start to delimiter pos
		start = end + delim.size();									// resetting starting var
		end = raw.find(delim, start);								// resetting end var
	}
	courses.push_back(raw.substr(start, end - start));				// to pick up last string

	return courses;
}

// function to load data
vector<Course> loadData(HashTable* hashTable){

	string fileName("ABCU_Advising_Program_Input.txt");				// Declaring file name // Had to resave as ANSI
	vector<string> lines;											// Temp Vecotr to hold lines
	vector<Course> courses;											// Vector to hold Courses
	string line;													// declaring line

	ifstream inFile(fileName);										// Decalring input file

	while (getline(inFile, line)) {									// While file is open and there are more lines
		Course newCourse;											// declaring temp course to be added 
		vector<string> tempCourse = splitCourse(line);				// Calling spit function on line
 
		newCourse.courseNumber = tempCourse[0];						// storing index 0 as course number
		newCourse.courseName = tempCourse[1];								// storing index 1 as course name
		for (int i = 2; i < tempCourse.size(); i++) {				// Only triggers if size is greater than 2 which mean ther is a prereq
			newCourse.prerequisites.push_back(tempCourse[i]);		// adds prereq to newCourse
		}
		courses.push_back(newCourse);								// Pushing new cours on vector
		hashTable->Insert(newCourse);
	}

	inFile.close();													// close file

	return courses;													// returns vector of courses
}

//function to print all
void HashTable::PrintAll() {

	for (int i = 0; i < nodes.size(); i++) {                         // for node begin to end iterate
		if (nodes[i].key != UINT_MAX) {                              // if key not equal to UINT_MAx
			cout << nodes[i].course.courseNumber << ", " << nodes[i].course.courseName << endl;
		}
	}
}

//function to search for specific course
Course HashTable::Search(string courseNumber) {
	Course course;
	//Converting courseNumber in to ASCII for hashing
	int tempInt = 0;
	string tempStr = "";
	for (int i = 0; i < courseNumber.length(); i++) {
		tempInt = courseNumber[i];
		tempStr += to_string(tempInt);
	}

	unsigned key = hash(atoll(tempStr.c_str()));									// getting hash valu for course num
	Node* node = &(nodes.at(key));													// locating node at key

	if (node == nullptr || node->key == UINT_MAX) {									// if no entry found for the key
		return course;																// return Course
	}
	while (node != nullptr) {														// while node not equal to nullptr
		if (node->key != UINT_MAX && node->course.courseNumber == courseNumber) {   // if the current node is not UINT_MAX and matches, return it
			return node->course;
		}
		node = node->next;															//node is equal to next node
	}

	return course;
}

//function to print single course
void displayCourse(Course course) {
	cout << "\n";																	// spacing 
	cout << course.courseNumber << ", " << course.courseName << endl;				// prininting course info for every course
	if (course.prerequisites.size() != 0) {											//if course has prereqs
		cout << "Prerequisites: " ;													// print with commas seperating
		for (int i = 0; i < course.prerequisites.size(); i++) {
			if (i == course.prerequisites.size()-1) {
				cout << course.prerequisites[i] << endl;
			} else {
				cout << course.prerequisites[i] << ", ";
			}
		}
	}
	return;
}


int main(int argc, char** argv)
{
	vector<Course> courses;									// holding value of courses vector after data loaded
	HashTable* courseTable;									// Defining new hashtable
	Course course;											// initializing course
	courseTable = new HashTable();

	int selection;											// declaring user selection
	string searchString;

	do {													// do while loop to propmpt for selection unless quit
		// MENU DISPALY
		cout << "\n1. Load Data Structure" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course" << endl;
		cout << "9. Exit" << endl;

		cout << "\nEnter your choice: ";
		cin >> selection;

		switch (selection){
		case 1: 
			courses = loadData(courseTable);			// loading data
			break;
		case 2: 
			cout << "Here is a sample schedule:" << endl;
			courseTable->PrintAll();
			break;

		case 3: 
			cout << "What course do you want to know about?" << endl;
			cin >> searchString;
			transform(searchString.begin(), searchString.end(), searchString.begin(), ::toupper);

			course = courseTable->Search(searchString);

			if (!course.courseNumber.empty()) {
				displayCourse(course);
			}
			else {
				cout << "Course " << searchString << " not found." << endl;
			}
			
			break;

		case 9: 
			cout << "Thank you for using the course planner."; 
			break;
		default: 
			cout << selection << " is not a valid option." << endl;
		}
	} while (selection != 9);

	return 0;
}

