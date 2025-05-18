#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ParserQ6Old
{
private:
    vector <string> tokens;
    int indx = 0;

public:

    ParserQ6Old(string outputPath) {
        ReadTokens(outputPath);
        E();
        if (indx == tokens.size()) {
            cout << "accepted" << endl;
        }
        else {
            cout << "rejected" << endl;
        }
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

    void F() {
        if (current_token() == "id") {
            indx++;
        }
        else if (current_token() == "(") {
            indx++;
            E();
            if (current_token() == ")") {
                indx++;
            }
            else {
                cout << "rejected: the ) not found\n";
                exit(1);
            }
        }
        else {
            cout << "rejected: not id or (\n";
            exit(1);
        }
    }

    void TD() {
        if (current_token() == "*") {
            indx++;
            F();
            TD();
        }
        else if (current_token() == "/") {
            indx++;
            F();
            TD();
        }
        else
            return;
    }

    void T() {
        F();
        TD();
    }

    void ED() {
        if (current_token() == "+") {
            indx++;
            T();
            ED();
        }
        else if (current_token() == "-") {
            indx++;
            T();
            ED();
        }
        else
            return;
    }

    void E() {
        T();
        ED();
    }

};

