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

class Follow
{
private:
    set<string> NonTerminal;
    set<string> Terminal;
    string startSymbol;
    unordered_map<string, vector<vector<string>>> Grammer;
    unordered_map<string, vector<pair<string, string>>> Elemnts;
    unordered_map < string, vector<string>> First;
    unordered_map < string, vector<string>> ResFollow;

public:
    Follow(string PathGrammer, string PathOutputFollow, string FirstPath) {
        ReadNonTerminal(PathGrammer);
        ReadTerminal(PathGrammer);
        ReadGrammer(PathGrammer);
        readFirst(FirstPath);
        NextElemnt();
        GetFollow();
        WriteOutput(PathOutputFollow);
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

        bool first = true;
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string lhs, arrow;
            ss >> lhs >> arrow;
            lhs = RemoveS(lhs);

            if (first) {
                startSymbol = lhs;
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


    void NextElemnt() {
        for (auto itG = Grammer.begin(); itG != Grammer.end(); ++itG) {
            for (auto itProd = itG->second.begin(); itProd != itG->second.end(); ++itProd) {
                for (int i = 0; i < itProd->size(); i++) {
                    if (NonTerminal.find((*itProd)[i]) != NonTerminal.end()) {

                        if (i + 1 < itProd->size()) {
                            Elemnts[(*itProd)[i]].push_back(make_pair((*itProd)[i + 1], itG->first));
                        }
                        else {
                            Elemnts[(*itProd)[i]].push_back(make_pair("FOLLOW", itG->first));
                        }
                    }
                }
            }
        }

    }

    void GetFollow() {
        ResFollow[startSymbol].push_back("$");

        bool flag = true;
        do {
            flag = false;
            for (auto itG = Grammer.begin(); itG != Grammer.end(); ++itG) {
                vector<string> res = ResFollow[itG->first];
                set<string> seen(res.begin(), res.end());

                for (auto itE = Elemnts[itG->first].begin(); itE != Elemnts[itG->first].end(); ++itE) {

                    if (itE->first != "FOLLOW") {
                        if (Terminal.find(itE->first) != Terminal.end()) {
                            if (seen.find(itE->first) == seen.end()) {
                                res.push_back(itE->first);
                                seen.insert(itE->first);
                            }
                        }
                        else {
                            for (auto itF = First[itE->first].begin(); itF != First[itE->first].end(); ++itF) {
                                if (*itF != "$" && seen.find(*itF) == seen.end()) {
                                    res.push_back(*itF);
                                    seen.insert(*itF);
                                }
                            }

                            if (find(First[itE->first].begin(), First[itE->first].end(), "$") != First[itE->first].end()) {
                                for (auto itF2 = ResFollow[itE->second].begin(); itF2 != ResFollow[itE->second].end(); ++itF2) {
                                    if (seen.find(*itF2) == seen.end()) {
                                        res.push_back(*itF2);
                                        seen.insert(*itF2);
                                    }
                                }
                            }
                        }
                    }
                    else {
                        for (auto itF3 = ResFollow[itE->second].begin(); itF3 != ResFollow[itE->second].end(); ++itF3) {
                            if (seen.find(*itF3) == seen.end()) {
                                res.push_back(*itF3);
                                seen.insert(*itF3);
                            }
                        }
                    }
                }

                if (res != ResFollow[itG->first]) {
                    ResFollow[itG->first] = res;
                    flag = true;
                }
            }

        } while (flag);
    }


    void WriteOutput(string PathWriteOutput) {
        ofstream file(PathWriteOutput);
        if (file) {
            string words = "";
            for (auto itG = ResFollow.begin(); itG != ResFollow.end(); ++itG) {
                words += itG->first;
                for (auto itR = itG->second.begin(); itR != itG->second.end(); ++itR) {
                    words += " " + *itR;
                }
                words += "\n";
            }


            file << words;
        }
        else {
            cerr << "Error opening output file " << PathWriteOutput << endl;
        }
    }
};

