#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>

using namespace std;

class PredictiveTable
{
private:
    unordered_map<string, vector<vector<string>>> Grammer;
    unordered_map < string, vector<string>> First;
    unordered_map < string, vector<string>> Follow;
    unordered_map<string, unordered_map<string, vector<string>>> parseTable;

public:
    PredictiveTable(string PathGrammer, string PathOutputTable, string FirstPath, string FollowPath) {
        ReadGrammer(PathGrammer);
        readFirst(FirstPath);
        readFollow(FollowPath);
        GenerateTable();
        WriteOutput(PathOutputTable);
    }


    void readFirst(string FirstPath) {
        ifstream file(FirstPath);
        if (!file.is_open()) {
            cout << "Error opening first file.txt" << endl;
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string nonTerm;
            vector<string> result;

            ss >> nonTerm;


            string res;
            while (ss >> res) {
                result.push_back(res);
            }

            First[nonTerm] = result;
        }
        file.close();
    }

    void readFollow(string FollowPath) {
        ifstream file(FollowPath);
        if (!file.is_open()) {
            cout << "Error opening first file.txt" << endl;
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string nonTerm;
            vector<string> result;

            ss >> nonTerm;


            string res;
            while (ss >> res) {
                result.push_back(res);
            }

            Follow[nonTerm] = result;
        }
        file.close();
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

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string lhs, arrow;
            ss >> lhs >> arrow;
            lhs = RemoveS(lhs);

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



    void GenerateTable() {
        for (auto itG = Grammer.begin(); itG != Grammer.end(); ++itG) {
            vector<vector<string>> prods = itG->second;

            for (auto itP = prods.begin(); itP != prods.end(); ++itP) {
                vector<string> p = *itP;
                bool abs = false;

                for (auto itSym = p.begin(); itSym != p.end(); ++itSym) {
                    string symbol = *itSym;
                    abs = false;

                    if (First.find(symbol) != First.end()) {
                        for (auto itF = First[itG->first].begin(); itF != First[itG->first].end(); ++itF) {
                            string f = *itF;
                            if (f != "$")
                                parseTable[itG->first][f] = p;
                            else
                                abs = true;
                        }
                    }
                    else {
                        if (symbol != "$")
                            parseTable[itG->first][symbol] = p;
                        else
                            abs = true;
                    }

                    if (!abs)
                        break;
                }

                if (abs) {
                    for (auto itF = Follow[itG->first].begin(); itF != Follow[itG->first].end(); ++itF) {
                        string f = *itF;
                        parseTable[itG->first][f] = p;
                    }
                }
            }
        }

    }


        
    

    void WriteOutput(string PathWriteOutput) {
        ofstream file(PathWriteOutput);
        if (file) {
            string words = "";

            for (auto itPT = parseTable.begin(); itPT != parseTable.end(); ++itPT) {
                for (auto itT = itPT->second.begin(); itT != itPT->second.end(); ++itT) {
                    words += itPT->first + " " + itT->first + " ";
                    for (auto itS = itT->second.begin(); itS != itT->second.end(); ++itS) {
                        if (*itS != "$") {
                            words += *itS + " ";
                        }
                        else {
                            words += " ";
                        }
                    }
                    words += "\n";
                }
            }


            file << words;
        }
        else {
            cerr << "Error opening output file " << PathWriteOutput << endl;
        }
    }
};