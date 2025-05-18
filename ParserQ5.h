#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ParserQ5
{
private:
    vector <string> tokens;
    int indx = 0;

public:

    ParserQ5(string outputPath) {
        ReadTokens(outputPath);
        S();
    }

	void ReadTokens(string outputPath) {
        ifstream file(outputPath);
        if (!file) {
            cerr << "Error opening output.txt" << endl;
            exit(1);
        }
        string line;
        while (getline(file, line)) {
            tokens.push_back(line);
        }
        file.close();
	}

    string current_token() {
        if (indx < tokens.size()) {
            return tokens[indx];
        }
        return ""; 
    }

    void A() {
        if (current_token() == "a_Token") {
            indx++;
            A();
        }
        else {
            return;
        }
    }

   void S() {
        if (current_token() == "b_Token") {
            indx++;
            A();
            if (indx == tokens.size()) {
                cout << "accepted" << endl; 
            }
            else {
                cout << "rejected" << endl; 
            }
        }
        else {
            cout << "rejected: not b in start";
        }
    }
};

