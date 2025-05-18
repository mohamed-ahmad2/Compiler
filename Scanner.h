#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <regex>

using namespace std;

class Scanner {
private:
    string Input = "";
    unordered_set<string> States;
    string StartState;
    unordered_set<string> FinalStates;
    string ErrorState;
    unordered_map<string, unordered_map<string, string>> Transition;
    unordered_map<string, unordered_map<string, string>> Tokens;
    unordered_set<string> Keywords;

public:
    Scanner(string PathInput, string PathWriteOutput, string PathAutomata, string PathTransion, string PathTokens, string PathKeywords) {
        ReadInput(PathInput);
        ReadAutomata(PathAutomata);
        ReadTransion(PathTransion);
        ReadTokens(PathTokens);
        ReadKeywords(PathKeywords);

        WriteOutput(PathWriteOutput, GetTokens());
    }

    void ReadInput(const string& PathInput) {
        ifstream file(PathInput);

        if (!file.is_open()) {
            cerr << "Error opening the file " << PathInput << endl;
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            //line.erase(remove(line.begin(), line.end(), ' '), line.end());
            Input += line;
        }
    }

    void ReadAutomata(const string& PathAutomata) {
        ifstream file(PathAutomata);
        if (!file) {
            cerr << "Error opening the file " << PathAutomata << endl;
            exit(1);
        }

        string line;
        vector<string> lines;
        while (getline(file, line)) {
            lines.push_back(line);
        }

        if (lines.size() < 4) {
            cerr << "Invalid format in file " << PathAutomata << endl;
            exit(1);
        }

        stringstream ss(lines[0]);
        string state;
        while (getline(ss, state, ',')) {
            States.insert(state);
        }

        StartState = lines[1];

        stringstream fs(lines[2]);
        while (getline(fs, state, ',')) {
            FinalStates.insert(state);
        }

        ErrorState = lines[3];
    }

    void ReadTransion(const string& PathTransion) {
        ifstream file(PathTransion);
        if (!file) {
            cerr << "Error opening the file " << PathTransion << endl;
            exit(1);
        }

        string fromState, inputSymbol, arrow, toState;
        while (file >> fromState >> inputSymbol >> arrow >> toState) {
            if (arrow != "->") {
                cerr << "Invalid transition format in file" << endl;
                continue;
            }
            Transition[fromState][inputSymbol] = toState;
        }
    }

    void ReadTokens(const string& PathTokens) {
        ifstream file(PathTokens);
        if (!file) {
            cerr << "Error opening the file " << PathTokens << endl;
            exit(1);
        }

        string state, token, value;
        while (file >> state >> value >> token) {
            Tokens[state][value] = token;
        }
    }

    void ReadKeywords(const string& PathKeywords) {
        ifstream file(PathKeywords);
        if (!file) {
            cerr << "Error opening the file " << PathKeywords << endl;
            exit(1);
        }

        string word;
        while (file >> word) {
            Keywords.insert(word);
        }
    }

    vector<string> split(string st, char d) {
        vector<string> words;
        stringstream ss(st);
        string word;

        while (getline(ss, word, d)) {
            words.push_back(word);
        }
        return words;
    }

    string RegexMatch(string Str) {
        if (regex_match(Str, regex("^[a-z]+$")))
            Str = "[a-z]";
        else if (regex_match(Str, regex("^[A-Z]+$")))
            Str = "[A-Z]";

        else if (regex_match(Str, regex("^[0-9]+$")))
            Str = "[0-9]";

        else if (regex_match(Str, regex("^[a-zA-Z]+$")))
            Str = "[a-zA-Z]";
        return Str;
    }

    string GetTokens() {
        string StrTokens = "", Str = "", CurrentState = StartState;
       
        bool flag = false;
        auto words = split(Input, '|');

        for (int w = 0; w < words.size(); ++w) {
            string word = words[w];
            int n = word.length();
            for (int i = 0; i < n; i++) {
                if (isalnum(word[i])) {
                    Str = "";
                    while (isalnum(word[i])) {
                        Str += word[i];
                        i++;
                    }
                    i--;    
                }
                else {
                    
                    if (word[i] == ' ') {
                        Str = "space";
                        
                        
                    }
                    else {

                        Str = "";
                        Str = string(1, word[i]);
                    }
                }

                string areKey = Str;
                Str = RegexMatch(Str);

                for (auto it = States.begin(); it != States.end(); ++it) {
                    string Css = *it;
                    flag = false;
                    if (Transition[Css].count(Str)) {
                        Css = Transition[Css][Str];
                        CurrentState = Css;

                        if (Keywords.count(areKey)) {
                            flag = true;
                            StrTokens += areKey + "\n";
                            break;
                        }
                        else {
                            flag = true;
                            StrTokens += Tokens[Css][Str] + "\n";
                            break;
                        }
                    }
                }

            }
        }


        if (FinalStates.find(CurrentState) != FinalStates.end())
            return StrTokens;
        else
            return "Error";
    }

    void WriteOutput(const string& PathWriteOutput, const string& words) {
        ofstream file(PathWriteOutput);
        if (file) {
            file << words;
        }
        else {
            cerr << "Error opening output file " << PathWriteOutput << endl;
        }
    }
};
