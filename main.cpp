/* Program name: File I/O
*  Author: Lauren Davis
*  Date last updated: 3/26/2026
* Purpose: This program has the user pick between choices, they can add a participant, fill out a survey, view participant files, and exit the program. Participant information and survey results are saved in their respective files.
*note AI was used to help fix syntax errors in the code*
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

//participant class 
class Participant {
private:
    int id;
    string firstName, lastName, age, gender, group;

public:
//stores participant info in object
    Participant(int id, string first, string last, string age, string gender) {
        this->id = id;
        this->firstName = first;
        this->lastName = last;
        this->age = age;
        this->gender = gender;
        //checks if group is even or odd to decice if they recieve studyMor or a placebo
        this->group = (id % 2 == 0) ? "StudyMor" : "Placebo";
    }
//formats participant information to be put into file
    string toFileString() {
        return to_string(id) + "," + firstName + "," + lastName + "," +
               age + "," + gender + "," + group;
    }

    static void display(string line) {
        stringstream ss(line);
        string id, first, last, age, gender, group;

        getline(ss, id, ',');
        getline(ss, first, ',');
        getline(ss, last, ',');
        getline(ss, age, ',');
        getline(ss, gender, ',');
        getline(ss, group, ',');

        cout << "\nID: " << id << "\nName: "
             << first << " " << last
             << "\nAge: " << age
             << "\nGender: " << gender
             << "\nGroup: " << group << endl;
             
        cout << "\n**************************************************";
    }
};

//saves survey info and time into survey data file
class Survey {
private:
//pid = participant id
    string date, pid;
    string headache, constipation, sleep, other, studyMore;

public:
    Survey(string pid, string headache, string constipation,
           string sleep, string other, string studyMore) {
        this->pid = pid;
        this->headache = headache;
        this->constipation = constipation;
        this->sleep = sleep;
        this->other = other;
        this->studyMore = studyMore;

    
        char formatDate[80];
        time_t currentDate = time(NULL);
        strftime(formatDate, 80, "%F", localtime(&currentDate));
        string inv_date(formatDate);

        this->date = inv_date;
    }

    string toFileString() {
        return date + "," + pid + "," + headache + "," +
               constipation + "," + sleep + "," +
               other + "," + studyMore;
    }
};


//runs main menu and options displayed on menu
class StudyManager {
private:
    const string participantFile = "participant.dat";
    const string surveyFile = "survey.dat";

    int getNextID() {
        ifstream file(participantFile);
        if (!file) return 1;

        string line, lastLine;
        while (getline(file, line)) {
            if (!line.empty()) lastLine = line;
        }

        if (lastLine.empty()) return 1;

        stringstream ss(lastLine);
        string idStr;
        getline(ss, idStr, ',');

        return stoi(idStr) + 1;
    }

public:
    void addParticipant() {
        string first, last, age, gender;

        cout << "Enter first name: ";
        cin >> first;

        cout << "Enter last name: ";
        cin >> last;

        cout << "Enter age: ";
        cin >> age;

        cout << "Enter gender: ";
        cin >> gender;

        int id = getNextID();
        Participant p(id, first, last, age, gender);
//file goes to append text mode for participant information
        ofstream file(participantFile, ios::app);
        file << p.toFileString() << endl;

        cout << "Participant added with ID " << id << endl;
    }

    void displayParticipants() {
        ifstream file(participantFile);
        if (!file) {
            cout << "No participants found.\n";
            return;
        }

        string line;
        cout << "\n****************** Participants ******************\n";
        while (getline(file, line)) {
            Participant::display(line);
        }
        
    }
//checks to see if participant on file exists with id 
    bool participantExists(string pid) {
        ifstream file(participantFile);
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string id;
            getline(ss, id, ',');

            if (id == pid) return true;
        }
        return false;
    }

    void collectSurvey() {
        displayParticipants();
        string pid;
        cout << "\nEnter participant ID: ";
        cin >> pid;

        if (!participantExists(pid)) {
            cout << "Invalid ID.\n";
            return;
        }

        string headache, constipation, sleep, other, studyMore;

        cout << "1. Did you have any headaches using StudyMor? ";
        cin >> headache;

        cout << "2. Did you have any constipation using StudyMor?  ";
        cin >> constipation;

        cout << "3. Did you experience any difficulty sleeping while using StudyMor? ";
        cin >> sleep;

        cin.ignore();
        cout << "4. List any other potential side effects, you experienced using StudyMor. ";
        getline(cin, other);

        cout << "5. Did you feel like you could study more using StudyMor? ";
        cin >> studyMore;

        Survey s(pid, headache, constipation, sleep, other, studyMore);
//files goes to appends mode to save new survey information
        ofstream file(surveyFile, ios::app);
        file << s.toFileString() << endl;

        cout << "Survey recorded.\n";
    }

    void menu() {
        int choice;

        do {
            cout << "\nWelcome to the Westlake Research Hospital StudyMor Study!\n";
            cout << "1. Add new participant\n";
            cout << "2. Collect survey\n";
            cout << "3. Display participants\n";
            cout << "4. Quit\n";
            cout << "Please enter a command to continue: ";
            cin >> choice;
            //using switch case to show the choices 
            switch (choice) {
                case 1: addParticipant(); break;
                case 2: collectSurvey(); break;
                case 3: displayParticipants(); break;
                case 4: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice.\n";
            }

        } while (choice != 4);
    }
};
//do while loop will continue to run until user enters 4 to quit program

int main() {
    //calls the manager class as an object
    StudyManager manager;
    //runs StudyManager class menu
    manager.menu();
    return 0;
}
