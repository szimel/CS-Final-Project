#include <iostream>
#include <string>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

const int numTests = 10;

struct Student {
  string name;
  int studentID;
  int numTests;
  int* testScores;
  double averageScore;
};

enum MenuOption {
  Add = 1,
  Remove = 2,
  Display = 3,
  Search = 4,
  Results = 5,
  Quit = 6
};

int displayMenu() {
  cout << "1. Add \n";
  cout << "2. Remove \n";
  cout << "3. Display \n";
  cout << "4. Search \n";
  cout << "5. Results \n";
  cout << "6. Quit \n";

  cout << "Enter choice: ";
  int userChoice;
  cin >> userChoice;

  return userChoice;
}

// returns the number of students in the file
int getNumber() {
  ifstream inFile("student.dat");
  if (!inFile.is_open()) {
    cout << "Error opening file.\n";
    return -1;
  }

  int count = 0;
  string line;
  while (getline(inFile, line)) {
    count++;
  }

  inFile.close();
  return count;
}

void removeStudent(int studentID) {
  // Get number of students & create dynamic array
  int numStudents = getNumber();
  Student* students = new Student[numStudents];

  // Open file
  ifstream inFile("student.dat");
  if (!inFile.is_open()) {
    cout << "Error opening file.\n";
    return;
  }

  bool found = false;

  // map through the file and store the data in the array
  for (int i = 0; i < numStudents; i++) {
    // Read student data from file
    string lastName, firstName;
    getline(inFile, lastName, ',');
    getline(inFile, firstName, ',');
    students[i].name = lastName + "," + firstName; // set student name

    inFile >> students[i].studentID;
    inFile.ignore(); // ignore comma

    inFile >> students[i].numTests;
    inFile.ignore(); // ignore comma

    students[i].testScores = new int[students[i].numTests];
    for (int j = 0; j < students[i].numTests; j++) {
      inFile >> students[i].testScores[j];
      inFile.ignore(); // Skip comma
    }

    // Check if this is the student to remove
    if (students[i].studentID == studentID) {
      found = true;
    }
  }

  inFile.close();

  // if we found a match
  if (found) {
    // make sure it opens the file
    ofstream outFile("student.dat");
    if (!outFile.is_open()) {
      cout << "Error opening file.\n";
      return;
    }

    // write all students except the one to remove
    for (int i = 0; i < numStudents; i++) {
      if (students[i].studentID != studentID) {
        outFile << students[i].name << "," << students[i].studentID << "," << students[i].numTests << ",";
        for (int j = 0; j < students[i].numTests; j++) {
          outFile << students[i].testScores[j] << ",";
        }
      }
    }

    outFile.close();
  } else {
    cout << "Student not found.\n";
  }

  // Delete dynamic array
  for (int i = 0; i < numStudents; i++) {
    delete[] students[i].testScores;
  }
  delete[] students;// not sure if this line is needed
}

// adds a student to the student.dat file
void addStudent() {
  // open student.dat file
  ofstream outFile;
  outFile.open("student.dat", ofstream::app);
  if(!outFile.is_open()) {
    cout << "Error opening file.\n";
    return;
  }

  // get student info
  Student student;
  cout << "Enter full name of the student (First Last): ";
  cin >> ws; // clear whitespace - doesn't work unless i use this
  getline(cin, student.name);
  
  // Split name into first and last
  size_t spacePos = student.name.find(' ');
  if (spacePos != string::npos) {
    string firstName = student.name.substr(0, spacePos);
    string lastName = student.name.substr(spacePos + 1);
    student.name = lastName + "," + firstName;
  }

  // get student ID and number of tests
  cout << "Enter student ID: ";
  cin >> student.studentID;
  cout << "Enter number of tests: ";
  cin >> student.numTests;

  // get test scores
  student.testScores = new int[student.numTests];
  for (int i = 0; i < student.numTests; i++) {
    cout << "Enter score #" << i + 1 << ": ";
    cin >> student.testScores[i];
  }

  // calculate average
  double total = 0;
  for (int i = 0; i < student.numTests; i++) {
    total += student.testScores[i];
  }
  student.averageScore = total / student.numTests;

  // write to file
  outFile << "\n" << student.name << "," << student.studentID << "," << student.numTests << ",";
  for (int i = 0; i < student.numTests; i++) {
    outFile << student.testScores[i] << ",";
  }

  // close file and delete (dynamic?) memory
  outFile.close(); 
  delete[] student.testScores;
}

int main() {
  // Menu options + user input
  MenuOption choice = static_cast<MenuOption>(displayMenu());

  // switch statement
  switch (choice) {
    case Add:
      addStudent();
      break;
    case Remove: {
      int id;
      cout << "Enter the student ID to remove: ";
      cin >> id;
      removeStudent(id);
      break;
    }
    case Display:
      // display();
      break;
    case Search: {
      int id;
      cout << "Enter the student ID to search for: ";
      cin >> id;
      // search(id);
      break;
    }
    case Results:
      // exportResults();
      break;
    case Quit:
      cout << "Exiting the program.";
      // exitProgram();
      return 0;
    default:
      cout << "Incorrect choice, please enter a valid number.\n";
      break;
  }
  return 0;
}