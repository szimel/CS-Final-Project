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

// menu output function
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

// this took forever to figure out - function that returns all students within student.dat
// in the form of a dynamic array/pointer thing
Student* getStudents(int& numStudents) {
  Student* students = new Student[numStudents];

  ifstream inFile("student.dat");
  if (!inFile.is_open()) {
    cout << "Error opening file.\n";
    return nullptr;
  }

  for (int i = 0; i < numStudents; i++) {
    string lastName, firstName;
    getline(inFile, lastName, ',');
    getline(inFile, firstName, ',');
    students[i].name = lastName + "," + firstName;

    inFile >> students[i].studentID;
    inFile.ignore(); // Skip comma

    inFile >> students[i].numTests;
    inFile.ignore(); // Skip comma

    students[i].testScores = new int[students[i].numTests];
    for (int j = 0; j < students[i].numTests; j++) {
      inFile >> students[i].testScores[j];
      inFile.ignore(); // Skip comma
    }
  }

  inFile.close();

  return students;
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
  // call getStudents to get all data from student.dat
  int numStudents = getNumber();
  Student* students = getStudents(numStudents);

  if (!students) { // make sure it worked
    cout << "Error getting students.\n";
    return;
  }

  bool found = false;

  // check if studentID is in the file
  for (int i = 0; i < numStudents; i++) {
    if (students[i].studentID == studentID) {
      found = true;
      break;
    }
  }

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
    cout << "Student removed.\n";

  } else {
    cout << "Student not found.\n";
  }

  // Delete dynamic array
  for (int i = 0; i < numStudents; i++) {
    delete[] students[i].testScores;
  }
  delete[] students;// not sure if this line is needed
}

void display() {
  // call getStudents to get all data from student.dat
  int numStudents = getNumber();
  Student* students = getStudents(numStudents);

  if (!students) { // make sure it worked
    cout << "Error getting students.\n";
    return;
  }

  for (int i = 0; i < numStudents; i++) {
    cout << setw(30) << left << students[i].name;
    cout << setw(15) << students[i].studentID;
    for (int j = 0; j < students[i].numTests; j++) {
      cout << setw(5) << students[i].testScores[j];
    }
  }

  // Delete dynamic array
  for (int i = 0; i < numStudents; i++) {
    delete[] students[i].testScores;
  }
  delete[] students;
}

void search(int studentID) {
  // call getStudents to get all data from student.dat
  int numStudents = getNumber();
  Student* students = getStudents(numStudents);

  if (!students) { // make sure it worked
    cout << "Error getting students.\n";
    return;
  }

  bool found = false;
  for (int i = 0; i < numStudents; i++) {
    if (students[i].studentID == studentID) {
      found = true;
      cout << setw(30) << left << students[i].name;
      cout << setw(15) << students[i].studentID;
      for (int j = 0; j < students[i].numTests; j++) {
        cout << setw(5) << students[i].testScores[j];
      }
    }
  }

  if (!found) cout << "Student not found.\n";
}

int findMinimum(int* scores, int size) {
  if (size < 5) {
    return 0;
  }

  int minScore = scores[0];
  for (int i = 1; i < size; i++) {
    if (scores[i] < minScore) {
      minScore = scores[i];
    }
  }

  return minScore;
}

void exportResults() {
  // open averages.dat file
  ofstream outFile("averages.dat");
  if (!outFile.is_open()) {
    cout << "Error opening file averages.dat\n";
    return;
  }
  // call getStudents to get all data from student.dat
  int numStudents = getNumber();
  Student* students = getStudents(numStudents);

  if (!students) { // make sure it worked
    cout << "Error getting students.\n";
    return;
  }

  // calculate average score for each student
  for (int i = 0; i < numStudents; i++) {
    double total = 0;
    int minScore = findMinimum(students[i].testScores, students[i].numTests);

    // add up all test scores
    for (int j = 0; j < students[i].numTests; j++) {
      total += students[i].testScores[j];
    }

    // calculate average
    total -= minScore;
    double numTests = minScore == 0 ? students[i].numTests : students[i].numTests - 1; // don't drop if there are less than 5 tests
    double average = total / numTests; 

    outFile << students[i].studentID << " " << fixed << setprecision(1) << average << "\n";
  }

  outFile.close();

  // Delete dynamic array
  for (int i = 0; i < numStudents; i++) {
    delete[] students[i].testScores;
  }
  delete[] students;
}

// adds a student to the student.dat file
void addStudent() {
  // open student.dat file
  ofstream outFile;
  outFile.open("student.dat", ofstream::app);
  if(!outFile.is_open()) {
    cout << "Error opening student.dat.\n";
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
      display();
      break;
    case Search: {
      int id;
      cout << "Enter the student ID to search for: ";
      cin >> id;
      search(id);
      break;
    }
    case Results:
      exportResults();
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