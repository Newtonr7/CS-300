// CS300 
// Project Two
// Roger Newton

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;

// The course structure to hold course info
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;

    // default constructor
    // this is needed to return an empty course when not found
    Course() {}

    Course(string number, string name) {
        courseNumber = number;
        courseName = name;
    
    }
};

// Now the node structure for the binary tree
struct Node {
    Course course;
    Node* left;
    Node* right;
    
    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// THe binary tree class
class BinaryTree {
private:
    Node* root;

    // the helper method in order to add a node recusively
    void addNode(Node* node, Course course) {
        // if the course number is > the current node we go left
        if (course.courseNumber < node -> course.courseNumber) {
            if (node -> left == nullptr) {
                node -> left = new Node(course);
            } else {
                addNode(node -> left, course);
            }
        } 
        //else go to the right
        else {
            if (node -> right == nullptr) {
                node -> right = new Node(course);
            } else{
                addNode(node -> right, course);
            }
        }
    }

// This is the helper method for the ordered traversal
    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node -> left);
            
            cout << node -> course.courseNumber << ", " << node -> course.courseName << endl; 
            inOrder(node -> right);
        }
    }

    // The helper method for searching up a course
    Node* searchCourse(Node* node, string courseNumber) {
        // the base case for found or not 
        if (node == nullptr || node-> course.courseNumber == courseNumber) {
            return node;
        }

        // this is where we search the left or right subtree
        // start with the left if the course number is less than the current node
        if (courseNumber < node -> course.courseNumber) {
            return searchCourse(node -> left, courseNumber);
        } 
        
        // otherwise we just search the right
        return searchCourse(node -> right, courseNumber);
    }
// Public methods for the binary tree
public:
    BinaryTree() {
        root = nullptr;
    }

    //This is to insert a course into the tree
    void insertCourse(Course course) {
        if (root == nullptr) {
            root = new Node(course);
        } 
        else {
            addNode(root, course);
        }
    }

    // now pring all the courses in order
    void InOrder() {
        inOrder(root);
    }

    // search for a course
    Course SearchCourse(string courseNumber) {
        Node* node = searchCourse(root, courseNumber);
        if (node != nullptr) {
            return node -> course;
        } 
        // if not found return an empty course
        return Course(); 
    }
};

// This is the function to load courses from a file
void loadCourses(string fileName, BinaryTree* tree) {
    ifstream file(fileName);
    
    // check first if the file opened correctly
    if (!file.is_open()) {
        cout << "Error opening file: " << fileName << endl;
        return;
    }

    // if no error show loading message
    cout << "Loading courses from file: " << fileName << endl;

    // In order to keep track of courses loaded
    string line;
    int courseCount = 0;

    // now read each line from the file
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        // and split by comma
        while (getline(ss, token, ',')) {
            // After reading the feedback I added trim for whitespace and make it uppercase 
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            transform(token.begin(), token.end(), token.begin(), ::toupper);
            tokens.push_back(token);
        }

        // we need to validate the line has a course number or name
        if (tokens.size() < 2) {
            cout << "Missing data in the file line" << endl;
            continue;
        }
        // Added this to skip any empty tokens  
        if (tokens[0].empty() || tokens[1].empty()) {
            continue;

        // create the course object
        Course course(tokens[0], tokens[1]);

        // add any prerequisites if they exist
        for (size_t i = 2; i < tokens.size(); ++i) {
            //get rid of any empty prerequisite tokens
            if(!tokens[i].empty()) {
                course.prerequisites.push_back(tokens[i]);
        }
    }
        // This moves to inserting the course into the tree
        tree -> insertCourse(course);
        courseCount++;
    }

    file.close();
    cout << courseCount << " courses loaded successfully." << endl;
}
}

//Function to print out the course info
void printCourseInfo(Course course){
    // check if the course is there
    if (course.courseNumber.empty()) {
        cout << "Course not found" << endl;
        return;
    
    }

    cout << course.courseNumber << ", " << course.courseName << endl;

    // now print any prerequisites
    if (!course.prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            // edited this to look up each prereq  in the tree 
            Course Prereq = tree -> SearchCourse(course.prerequisites[i]);
            // now make sure it exists
            if (!Prereq.courseNumber.empty()) {
                cout << Prereq.courseNumber << ", " << Prereq.courseName;
            } 
            else {
                cout << course.prerequisites[i] << " (The Prerequisite was not found)";
            } 
            if (i < course.prerequisites.size() - 1) {
                cout << endl << "             ";
            }
        }
        cout << endl;

     } 
     else {
        cout << "No prerequisites" << endl;
    }
}

// THis is the main function of the program
int main() {
    BinaryTree* tree = new BinaryTree();
    string fileName;
    string courseNumber;
    int choice = 0;
    // added to make sure data was loaded
    bool dataLoaded = false;


    // print welcome message
    // I tried to match the sample output given 
    // personaly I like some extra spacing but this works
    cout << "Welcome to the Course planner." << endl;

    // The main menue loop
    while (choice != 9) {
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";

        // take in the user choice
        cin >> choice;

        // This handles the invalid choices
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << endl << "Invalid input. Please enter a number." << endl;
            continue;
        }
        // Decided to use a switch statement for the choices
        switch (choice) {
            // loading the data from the file
            case 1:
                cout << "Enter the file name: ";
                cin >> fileName;
                loadCourses(fileName, tree);
                // Make sure the data is loaded
                dataLoaded = true;
                break;

            // printing the course list in order
            case 2:
            // added a chek for data being loaded 
                if (!dataLoaded) {
                    cout << "Please load the data structure first." << endl;
                }
                else {
                    cout << "Here is a sample schedule:" << endl;
                    tree -> InOrder();
                }
                break;
            
            // printing a specific course
            case 3:
                // added to make sure data is loaded
                if (!dataLoaded) {
                    cout << "Please load the data structure first." << endl;
                }
                else {
                    cout << "What course do you want to know about? ";
                    cin >> courseNumber;

                    // this converts to uppercase just to be consistent
                    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper)
                    printCourseInfo(tree -> SearchCourse(courseNumber));
                }
                break;

            // exiting the program
            case 9:
                cout << "Thank you for using the course Planner!" << endl;
                break;

            // handling invalid choice
            default:
                cout << choice << " is not a valid option" << endl;
                break;
        }
  
    }
    // Added this to clean up the memory 
    delete tree;
    return 0;
}


            
