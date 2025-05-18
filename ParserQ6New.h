#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <set>
#include <string>

using namespace std;

class ParserQ6New {
private:
    string start;
    unordered_map<string, unordered_map<string, vector<string>>> parseTable;
    stack<string> inputTokens;
    stack<string> pStack; 
    set<string> Terminal;
    unordered_map<string, vector<vector<string>>> Grammer;

public:

    ParserQ6New(string PathGrammer, string tablePath, string inputPath) {
        ReadGrammer(PathGrammer);
        ReadTerminal(PathGrammer);
        readParseTable(tablePath);
        readInput(inputPath);

        pStack.push("$");
        pStack.push(start);
        
        parse();
       
    }

    void ReadTerminal(string PathGrammer) {
        ifstream file(PathGrammer);
        if (!file.is_open()) {
            cerr << "Error in file Grammer\n";
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string word;


            while (ss >> word) {

                if (word == "->" || word == "|") {
                    continue;
                }

                if (word[0] == '<' && word.back() == '>') {
                    continue;
                }

                string current = "";
                for (int i = 0; i < word.length(); i++) {
                    if (word[i] == '<') {
                        i++;
                        while (i < word.length() && word[i] != '>') {
                            i++;
                        }
                        current = "";
                    }
                    else {
                        if (isalpha(word[i])) {
                            current = "";
                            while (isalnum(word[i])) {
                                current += word[i];
                                i++;
                            }
                            i--;
                        }
                        else {
                            current = "";
                            current = string(1, word[i]);
                        }

                        Terminal.insert(current);
                    }
                }
            }
        }
    }

    string RemoveS(const string& s) {
        string result = s;
        result.erase(remove(result.begin(), result.end(), '<'), result.end());
        result.erase(remove(result.begin(), result.end(), '>'), result.end());
        return result;
    }

    void ReadGrammer(string PathGrammer) {
        ifstream file(PathGrammer);

        if (!file.is_open()) {
            cerr << "Error in file Grammer\n";
            exit(1);
        }

        bool first = true;
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string lhs, arrow;
            ss >> lhs >> arrow;
            lhs = RemoveS(lhs);

            if (first) {
                start = lhs;
                first = false;
            }

            string rhs;
            getline(ss, rhs);
            stringstream rhsStream(rhs);

            vector<string> prod;
            vector<vector<string>> productions;
            string word;

            while (rhsStream >> word) {

                if (word == "->" || word == "|") {
                    if (!prod.empty()) {
                        productions.push_back(prod);
                    }
                    prod.clear();
                    continue;
                }

                string current = "";
                for (int i = 0; i < word.length(); i++) {
                    if (word[i] == '<') {
                        current = "<";
                        i++;
                        while (i < word.length() && word[i] != '>') {
                            current += word[i];
                            i++;
                        }
                        if (i < word.length() && word[i] == '>') {
                            current += '>';
                        }
                        prod.push_back(RemoveS(current));
                        current = "";
                    }
                    else {
                        if (isalpha(word[i])) {
                            current = "";
                            while (isalnum(word[i])) {
                                current += word[i];
                                i++;
                            }
                            i--;
                        }
                        else {
                            current = "";
                            current = string(1, word[i]);
                        }

                        prod.push_back(current);
                    }


                }



            }

            productions.push_back(prod);
            Grammer[lhs] = productions;
        }
    }

    bool isTerminal(string symbol) {
        return Terminal.count(symbol) > 0;
    }


    void readParseTable(string tablePath) {
        ifstream file(tablePath);
        if (!file.is_open()) {
            cout << "Error opening parse_table.txt" << endl;
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string nonTerm, term;
            vector<string> Vresult;

            ss >> nonTerm >> term;

            
            string res;
            while (ss >> res) {
                Vresult.push_back(res);
            }

            parseTable[nonTerm][term] = Vresult;
        }
        file.close();
    }

   


    void readInput(string inputPath) {
        ifstream file(inputPath);
        if (!file.is_open()) {
            cout << "Error opening scanner output.txt" << endl;
            exit(1);
        }

        vector<string> temp;
        string token;

        while (getline(file, token)) {
            temp.push_back(token);
        }

        temp.push_back("$");

        for (int i = temp.size() - 1; i >= 0; --i) {
            inputTokens.push(temp[i]);
        }

        file.close();
    }

    void parse() {
        bool error = false;

        while (!pStack.empty() && !inputTokens.empty() && !error) {
            string top = pStack.top();
            string currentToken = inputTokens.top();

            if (isTerminal(top)) {
                if (top == currentToken) {
                    pStack.pop();
                    inputTokens.pop();
                }
                else {
                    error = true;
                }
            }
            else {
                if (parseTable[top].count(currentToken) > 0) {
                    vector<string> Vres = parseTable[top][currentToken];
                    pStack.pop();

                    if (!Vres.empty()) {
                        for (int i = Vres.size() - 1; i >= 0; i--) {
                            pStack.push(Vres[i]);
                        }
                    }
                }
                else {
                    error = true;
                }
            }
        }

        if (!error) {
            cout << "Accepted" << endl;
        }
        else {
            cout << "Rejected" << endl;
        }
    }
};