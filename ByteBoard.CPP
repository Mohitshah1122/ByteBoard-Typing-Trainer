#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <iomanip>

using namespace std;
using namespace std::chrono;

class ByteBoard {
    string userName;
    string currentSentence;
    string userInput;
    int totalWords = 0, correctWords = 0, errors = 0;
    double accuracy = 0.0, wpm = 0.0;

public:
    void run();
    void getUserName();
    void provideFingerInstructions();
    void loadRandomSentence();
    void startTypingTest();
    void calculateResults();
    void saveProgress();
    void generateCertificate();
    int countWords(const string& str);
};

void ByteBoard::run() {
    // Check and create progress.txt if it doesn't exist
    ifstream checkFile("progress.txt");
    if (!checkFile) {
        ofstream createFile("progress.txt");
        createFile << "=== Typing Progress ===\n";
        createFile.close();
        cout << "Created 'progress.txt' to store your progress.\n";
    }
    while (true) {
        getUserName();
        provideFingerInstructions();
        loadRandomSentence();
        startTypingTest();
        calculateResults();
        saveProgress();

        if (wpm >= 60) {
            generateCertificate();
        }

        cout << "\nPress Enter to start again or Ctrl+C to exit...\n";
        cin.ignore();
    }
}

void ByteBoard::getUserName() {
    cout << "\n====== Welcome to ByteBoard: Typing Practice Tool ======\n";
    do {
        cout << "Enter your full name: ";
        getline(cin, userName);
    } while (userName.empty());
}

void ByteBoard::provideFingerInstructions() {
    cout << "\n===== Finger and Hand Placement Instructions =====\n";
    cout << "1. Place your fingers on the 'home row' keys: ASDF for the left hand and JKL; for the right hand.\n";
    cout << "2. Keep your wrists straight and relaxed, and your elbows at a 90-degree angle.\n";
    cout << "3. Use your index fingers to locate the 'F' and 'J' keys.\n";
    cout << "4. Use proper finger placement and avoid looking at the keyboard.\n\n";
    cout << "Press Enter to continue...\n";
    cin.ignore();
}

void ByteBoard::loadRandomSentence() {
    ifstream file("sentences.txt");
    if (!file.is_open()) {
        cerr << "Error: 'sentences.txt' not found!\n";
        exit(1);
    }

    vector<string> sentences;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) sentences.push_back(line);
    }
    file.close();

    if (sentences.empty()) {
        cerr << "No sentences found in file!\n";
        exit(1);
    }

    srand(time(0));
    currentSentence = sentences[rand() % sentences.size()];
}

void ByteBoard::startTypingTest() {
    cout << "\nType the following sentence:\n\n";
    cout << currentSentence << "\n\n";
    cout << "Start typing and press Enter when done:\n";

    auto start = high_resolution_clock::now();
    getline(cin, userInput);
    auto end = high_resolution_clock::now();

    duration<double> timeTaken = end - start;
    double seconds = timeTaken.count();

    totalWords = countWords(currentSentence);
    correctWords = 0;
    errors = 0;

    for (size_t i = 0; i < min(userInput.size(), currentSentence.size()); i++) {
        if (userInput[i] != currentSentence[i]) {
            errors++;
        } else {
            correctWords++;
        }
    }

    wpm = (countWords(userInput) / seconds) * 60;
    accuracy = ((double)correctWords / currentSentence.length()) * 100;
}

void ByteBoard::calculateResults() {
    cout << "\n===== Typing Test Results =====\n";
    cout << "Original Sentence: " << currentSentence << "\n";
    cout << "Your Input       : " << userInput << "\n\n";
    cout << "WPM (Words/min)  : " << fixed << setprecision(2) << wpm << "\n";
    cout << "Accuracy         : " << fixed << setprecision(2) << accuracy << "%\n";
    cout << "Errors           : " << errors << "\n";

    cout << "\n Congratulations, " << userName << ;
    cout << "Note: Earn a certificate by reaching 60 WPM or more!\n";

    if (wpm >= 60) {
        cout << "\n***************************************************\n";
        cout << "                  CONGRATULATIONS!\n";
        cout << "  You have achieved a speed of over 60 WPM.\n";
        cout << "    This is to certify that " << userName << " has\n";
        cout << "        demonstrated exceptional typing skills.\n";
        cout << "***************************************************\n";
    } else if (wpm >= 40) {
        cout << "\nGreat job! You're a proficient typist.\n";
    } else {
        cout << "\nKeep practicing to improve your speed and accuracy!\n";
    }
}

void ByteBoard::saveProgress() {
    // Create file if it doesn't exist
    ifstream checkFile("progress.txt");
    if (!checkFile.good()) {
        ofstream createFile("progress.txt");
        if (!createFile.is_open()) {
            cerr << "Error: Could not create progress.txt\n";
            return;
        }
        createFile << "====== ByteBoard Typing Progress Log ======\n";
        createFile.close();
        cout << "File 'progress.txt' created successfully.\n";
    }
    checkFile.close();

    // Now append progress
    ofstream file("progress.txt", ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not write to progress.txt\n";
        return;
    }

    time_t now = time(0);
    tm* ltm = localtime(&now);

    file << (1900 + ltm->tm_year) << "-"
         << setw(2) << setfill('0') << (1 + ltm->tm_mon) << "-"
         << setw(2) << setfill('0') << ltm->tm_mday << " "
         << setw(2) << setfill('0') << ltm->tm_hour << ":"
         << setw(2) << setfill('0') << ltm->tm_min << ":"
         << setw(2) << setfill('0') << ltm->tm_sec << " | ";

    file << "Name: " << userName
         << " | WPM: " << fixed << setprecision(2) << wpm
         << " | Accuracy: " << fixed << setprecision(2) << accuracy << "%"
         << " | Errors: " << errors << "\n";

    file.close();
    cout << "\nYour result has been saved to 'progress.txt'.\n";
}

void ByteBoard::generateCertificate() {
    ofstream cert("certificate.txt");
    cert << "=================================================\n";
    cert << "             BYTEBOARD CERTIFICATE\n";
    cert << "=================================================\n";
    cert << "This certificate is awarded to:\n\n";
    cert << "           " << userName << "\n\n";
    cert << "For achieving an impressive typing speed of:\n";
    cert << "           " << fixed << setprecision(2) << wpm << " WPM\n";
    cert << "With an accuracy of:\n";
    cert << "           " << fixed << setprecision(2) << accuracy << "%\n\n";
    cert << "=================================================\n";
    cert << "       Keep up the great work and keep typing!\n";
    cert << "=================================================\n";
    cert.close();

    cout << "\nYour certificate has been saved to 'certificate.txt'.\n";
}

int ByteBoard::countWords(const string& str) {
    int count = 0;
    bool inWord = false;
    for (char c : str) {
        if (isspace(c)) {
            inWord = false;
        } else if (!inWord) {
            count++;
            inWord = true;
        }
    }
    return count;
}

int main() {
    ByteBoard app;
    app.run();
    return 0;
}
