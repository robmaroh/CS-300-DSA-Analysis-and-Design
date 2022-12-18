// =======================================================
// Name:        Advising Assistance Program.cpp 
// Author:      Rob Marlatt
// Description: This program loads a comma separated value 
//		        file to a Binary Search Tree and allows it 
//				to be displayed in its entirety or just one
//				item. The program shows all courses including 
//				prerequisites for a computer science program.
// Class:       CS-300-T2805 DSA: Analysis and Design 22EW2 
// Date:        12/11/2022
// =======================================================


#include <iostream>
#include <fstream>
#include <vector>
#include "CSVparser.hpp"

using namespace std;

// =======================================================
// Global definitions visible to all methods and classes
// =======================================================

struct Course { // Each course is made up of an ID, Name and
	// vector of prerequisites
	string courseId;
	string courseName;
	vector<string> coursePrerequisites;
	Course() {}
};

struct Node { // Each node of the Binary Search Tree contains
	// course info and left and right children
	// Using multiple constructors allow a node with or without
	// a defined course
	Course course;
	Node* left;
	Node* right;

	Node() { // initializing an empty node
		left = nullptr;
		right = nullptr;
	}

	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};

// =======================================================
// Class definition for CourseBST
// =======================================================

class CourseBST {
private:
	Node* root;

	void addNode(Node* node, Course course);
	void printSampleSchedule(Node* node);
	void printCourseInformation(Node* node, string courseNum);

public:
	CourseBST();
	virtual ~CourseBST();
	void DeleteRecursive(Node* node);
	void Insert(Course course);
	int NumPrerequisiteCourses(Course course);
	void PrintSampleSchedule();
	void PrintCourseInformation(string courseNum);
};

// =======================================================
// Construct the Binary Search Tree and initialize root
// =======================================================

CourseBST::CourseBST() {
	root = nullptr;
}

// =======================================================
// Deconstructor for Binary Search Tree recursive deletion
// =======================================================

CourseBST::~CourseBST() {
	DeleteRecursive(root);
}

// =======================================================
// Deconstruct Binary Search Tree recursively
// =======================================================

void CourseBST::DeleteRecursive(Node* node) {
	if (node) {
		DeleteRecursive(node->left);
		DeleteRecursive(node->right);
		delete node;
	}
}

// =======================================================
// Node insertion for Binary Search Tree
// =======================================================

void CourseBST::Insert(Course course) {
	//If the root is not initialized
	if (root == nullptr)
		//initialize it with the current course info
		root = new Node(course);
	//Else, send the node to the addNode method
	//to be added to the Binary Search Tree
	else {
		this->addNode(root, course);
	}
}

// =======================================================
// Function for prerequisite count
// =======================================================
int CourseBST::NumPrerequisiteCourses(Course course) {
	int count = 0;
	for (unsigned int i = 0; i < course.coursePrerequisites.size(); i++) {
		if (course.coursePrerequisites.at(i).length() > 0)
			count++;
	}
	return count;
}

// =======================================================
// Function for printing sample schedule
// =======================================================

void CourseBST::PrintSampleSchedule() {
	this->printSampleSchedule(root);
}

// =======================================================
// Function for printing course information
// =======================================================

void CourseBST::PrintCourseInformation(string courseId) {
	this->printCourseInformation(root, courseId);
}

// =======================================================
// Function to add a node
// =======================================================

void CourseBST::addNode(Node* node, Course course) {
	// Current courseId is less than the current node's courseId
	if (node->course.courseId.compare(course.courseId) > 0) {
		if (node->left == nullptr)
			node->left = new Node(course);
		else
			this->addNode(node->left, course);
	}
	// Current courseId is equal or greater than 
	// the current node's courseId
	else {
		if (node->right == nullptr)
			node->right = new Node(course);
		else
			this->addNode(node->right, course);
	}
}

// =======================================================
// Function to print sample schedule alphanumerically
// =======================================================

void CourseBST::printSampleSchedule(Node* node) {
	if (node != nullptr) {
		printSampleSchedule(node->left);
		cout << node->course.courseId << ", " << node->course.courseName << endl;
		printSampleSchedule(node->right);
	}
	return;
}

// =======================================================
// Function to print a single course with prerequisites
// =======================================================

void CourseBST::printCourseInformation(Node* curr, string courseId) {
	// while loop to travel all of BST until end or courseId found
	while (curr != nullptr) {
		// if loop to see if courseId equals current courseId
		if (curr->course.courseId.compare(courseId) == 0) {
			// Display course and get num prereqs
			cout << endl << curr->course.courseId << ", " << 
				curr->course.courseName << endl;
			unsigned int size = NumPrerequisiteCourses(curr->course);
			cout << "Prerequisite(s): ";
			// If prereqs exist, display each prereq with proper formatting
			unsigned int i = 0;
			for (i = 0; i < size; i++) {
				cout << curr->course.coursePrerequisites.at(i);
				if (i != size - 1)
					cout << ", ";
			}
			// If no prereqs, inform user and return to stop the search
			if (i == 0)
				cout << "No prerequisites required.";
			cout << endl;
			return;
		}
		// Passed courseId is smaller than the current courseId so traverse left
		else if (courseId.compare(curr->course.courseId) < 0)
			curr = curr->left;
		// Passed courseId is larger than the current courseId so traverse right
		else
			curr = curr->right;
	}
	// nullptr has been reached but course hasn't been found/ printed, 
		// so inform user
		cout << "Course " << courseId << " not found." << endl;
}

// =======================================================
// Static testing methods
// =======================================================

//Load Courses from cin choice for csv file
bool loadCourses(string csvPath, CourseBST* coursesBST) {
	// Open course file, separate each line & insert into BST
	try {
		ifstream courseFile(csvPath);
		if (courseFile.is_open()) {
			while (!courseFile.eof()) {
				// A vector will hold each csv that a course contains
				vector<string> courseInfo;
				string courseData;
				getline(courseFile, courseData);
				while (courseData.length() > 0) {
					// Get each course data, add to vector & delete 
					// from string
					unsigned int comma = courseData.find(',');
					if (comma < 100) { // Any data field is 99 chars or less
						courseInfo.push_back(courseData.substr(0, comma));
						courseData.erase(0, comma + 1);
					}
					// Add the last course after the final comma
					else {
						courseInfo.push_back(courseData.substr(0,
							courseData.length()));
						courseData = "";
					}
				}
				// Load the separated values into a course,
				//insert into Binary Search Tree and close file
				Course course;
				course.courseId = courseInfo[0];
				course.courseName = courseInfo[1];
				for (unsigned int i = 2; i < courseInfo.size(); i++) {
					course.coursePrerequisites.push_back(courseInfo[i]);
				}
				coursesBST->Insert(course);
			}
			courseFile.close();
			return true;
		}
	}
	catch (csv::Error& e) {
		cerr << e.what() << endl;
	}
	return false;
}

// =======================================================
// Function to print menu
// =======================================================

void printMenu() {
	cout << " 1. Load Data Structure" << endl;
	cout << " 2. Print Course List" << endl;
	cout << " 3. Print Course" << endl;
	cout << " 9. Exit" << endl;
	cout << "What would you like to do? ";
}

// =======================================================
// Main method
// =======================================================

int main(int argc, char* argv[]) {
	// Process command line argument
	string csvPath, courseId;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;
	default:
		csvPath = "Advising_Assistance_Program.csv";
		break;
	}	
	
	// Welcome message
	cout << "Welcome to the course planner!" << endl << endl;
	
	// declaring here keeps memory for entire process
	CourseBST* coursesBST = nullptr;
	int choice = 0;

	// if not 9 or exit
	while (choice != 9) {
		// load menu
		printMenu();
		cin >> choice;
		bool success = 1;
		// Handle users choice from menu
		switch (choice) {

			// Instantiate Binary Search Tree
			// get file path name
			// and load courses into Binary Search Tree
		case 1:
			if (coursesBST == nullptr) {
				coursesBST = new CourseBST();
			}

			loadCourses(csvPath, coursesBST);
			if (success) {
				cout << "Courses have been loaded.\n" << endl;
			}
			else {
				cout << "File not found.\n" << endl;
			}
			csvPath = "";
			break;

			// Check that a Binary Search Tree exists;
			// print a sample schedule
		case 2:
			if (coursesBST != nullptr && success) {
				cout << "Here is a sample schedule:" << endl << endl;
				coursesBST->PrintSampleSchedule();
				cout << endl;
			}
			else {
				cout << "Load courses first - option 1" << endl << endl;
			}
			break;


			// Check that a Binary Search Tree exists;
			// search and display course info
		case 3:
			if (coursesBST != nullptr && success) {
				if (courseId.length() == 0) {
					cout << "What course do you want to know about? ";
					cin >> courseId;
					for (auto& userChoice : courseId) userChoice =
						toupper(userChoice);
				}
				coursesBST->PrintCourseInformation(courseId);
				cout << endl;
			}

			else {
				cout << "Load courses first - option 1\n" << endl;
			}
			courseId = "";
			break;

		case 9: // Exit program
			cout << endl << "Thank you for using the course planner!" << endl;

		default: // User entered invalid data
			cout << choice << " is not a valid option" << endl << endl;
			break;
		}
	}
	
	return 0;
}
