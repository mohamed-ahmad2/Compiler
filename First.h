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

class First {
private:
    set<string> NonTerminal;
    set<string> Terminal;
    unordered_map<string, vector<vector<string>>> Grammer;

public:
    First(string PathGrammer, string PathOutputFirst) {
        ReadNonTerminal(PathGrammer);
        ReadTerminal(PathGrammer);
        ReadGrammer(PathGrammer);
        WriteOutput(PathOutputFirst, ResultFirst());
    }

    void ReadNonTerminal(string PathGrammer) {
        ifstream file(PathGrammer);
        if (!file.is_open()) {
            cerr << "Error in file Grammer\n";
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == '<') {
                    i++;
                    string str = "";
                    while (line[i] != '>') {
                        str += line[i];
                        i++;
                    }
                    NonTerminal.insert(str);
                }
            }
        }




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

    vector<string> FindFirst(string str) {
        vector<string> res;

        if (Terminal.find(str) != Terminal.end()) {
            res.push_back(str);
            return res;
        }

        if ((NonTerminal.find(str) != NonTerminal.end()) && (Grammer.find(str) != Grammer.end())) {
            vector<vector<string>> prods = Grammer[str];

            for (int i = 0; i < prods.size(); i++) {
                bool abs = true;

                for (int j = 0; j < prods[i].size() && abs; j++) {
                    abs = false;

                    vector<string> first = FindFirst(prods[i][j]);

                    for (auto it = first.begin(); it != first.end(); ++it) {
                        if (*it == "$") {
                            abs = true;
                        }
                        else if (find(res.begin(), res.end(), *it) == res.end()) {
                            res.push_back(*it);
                        }
                    }

                }

                
                if (abs) {
                    if (find(res.begin(), res.end(), "$") == res.end()) {
                        res.push_back("$");
                    }
                }
            }
        }

        return res;
    }


    string ResultFirst() {
        string words = "";
        for (auto G = Grammer.begin(); G != Grammer.end(); ++G) {
            vector<vector<string> > prods = G->second;
            words += G->first;
            for (int i = 0; i < prods.size(); i++) {
                vector<string> firstSet = FindFirst(prods[i][0]);
                for (auto c = firstSet.begin(); c != firstSet.end(); ++c) {
                    words += " " + *c;
                }
            }
            words += "\n";
        }
        return words;
    }

    void WriteOutput( string PathWriteOutput,  string words) {
        ofstream file(PathWriteOutput);
        if (file) {
            file << words;
        }
        else {
            cerr << "Error opening output file " << PathWriteOutput << endl;
        }
    }

};
