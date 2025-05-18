#include <iostream>
#include <string>
#include "Scanner.h"
#include "ParserQ5.h"
#include "ParserQ6Old.h"
#include "ParserQ6New.h"
#include "First.h"
#include "Follow.h"
#include "PredictiveTable.h"
using namespace std;

int main()
{
    string input, output, keywords, automaton, tokens, transition, Grammer, outputFirst, outputFollow, outputTable;
    input = "input.txt";
    output = "output.txt";
    automaton = "automaton.txt";
    transition = "transition.txt";
    tokens = "tokens.txt";
    keywords = "keywords.txt";
    Grammer = "Grammer.txt";
    outputFirst = "First.txt";
    outputFollow = "Follow.txt";
    outputTable = "Table.txt";

    Scanner s = Scanner(input, output, automaton, transition, tokens, keywords);
    //ParserQ5 p = ParserQ5(output);
    //ParserQ6Old p = ParserQ6Old(output);
    
    First f = First(Grammer, outputFirst);
    Follow follow = Follow(Grammer, outputFollow, outputFirst);
    PredictiveTable parseTable = PredictiveTable(Grammer, outputTable, outputFirst, outputFollow);
    ParserQ6New p = ParserQ6New(Grammer, outputTable, output);
}
