# Compiler
Graph Language Compiler
Overview
This project implements a compiler for a simple graph-like language, capable of processing input strings representing graph structures, such as (41 55)-(44 red)-(6 8). The compiler includes a lexical analyzer (scanner), a syntax analyzer (parser), and utilities to compute FIRST and FOLLOW sets and generate a predictive parsing table. The grammar is defined in a context-free format, and the parser uses an LL(1) predictive parsing approach to validate input.
Project Structure
The project consists of several C++ header files and configuration files, each serving a specific purpose in the compilation process:
Source Files

Scanner.h: Implements a lexical analyzer that tokenizes input strings based on a finite automaton defined in automaton.txt, transition.txt, and tokens.txt. It reads input from input.txt and produces tokenized output in output.txt.
First.h: Computes the FIRST sets for the grammar's non-terminals, reading from Grammer.txt and writing to First.txt.
Follow.h: Computes the FOLLOW sets for the grammar's non-terminals, using Grammer.txt and First.txt, and writes to Follow.txt.
PredictiveTable.h: Generates an LL(1) predictive parsing table based on the grammar, FIRST, and FOLLOW sets, writing to Table.txt.
ParserQ6New.h: Implements an LL(1) predictive parser that uses the parsing table (Table.txt) to validate tokenized input from output.txt against the grammar in Grammer.txt.
ParserQ5.h: An incomplete recursive descent parser (not used in the main workflow).
ParserQ6Old.h: An older recursive descent parser for a different grammar (not used in the main workflow).

Configuration Files

Grammer.txt: Defines the context-free grammar for the graph language:<Graph> -> <Tuple><G>
<G> -> -<Graph> | $
<Tuple> -> (int space<T>)
<T> -> int | color


input.txt: Contains the input string to be processed, e.g., (41 55)-(44 red)-(6 8).
output.txt: The tokenized output from the scanner, e.g.:(
int
space
int
)
-
(
int
space
color
)
-
(
int
space
int
)


First.txt: Contains the computed FIRST sets for each non-terminal, e.g.:Graph (
G - $
Tuple (
T int color


Follow.txt: Contains the computed FOLLOW sets for each non-terminal, e.g.:Graph $
G $
Tuple - $
T )


Table.txt: The LL(1) predictive parsing table, e.g.:Graph ( Tuple G
G - - Graph
G $
Tuple ( ( int space T )
T int int
T color color


automaton.txt: Defines the states, start state, final states, and error state for the scanner's finite automaton.
transition.txt: Specifies state transitions for the scanner, e.g., q0 ( -> q1.
tokens.txt: Maps states and inputs to token types, e.g., q2 [0-9] int.
keywords.txt: Lists keywords for the language, e.g., space.

Grammar Description
The grammar describes a graph structure composed of tuples connected by dashes (-). Each tuple is of the form (int space <T>), where <T> is either int or color. The grammar supports recursive structures through the <G> non-terminal, which allows for multiple tuples connected by dashes or an empty production ($).
Compilation Workflow

Lexical Analysis:

The Scanner class reads the input string from input.txt.
Using the finite automaton defined in automaton.txt, transition.txt, and tokens.txt, it tokenizes the input.
Keywords (e.g., space) are identified using keywords.txt.
The tokenized output is written to output.txt.


FIRST and FOLLOW Sets:

The First class computes the FIRST sets for all non-terminals and writes them to First.txt.
The Follow class computes the FOLLOW sets using the FIRST sets and writes them to Follow.txt.


Predictive Parsing Table:

The PredictiveTable class generates an LL(1) parsing table based on the grammar, FIRST, and FOLLOW sets.
The table is written to Table.txt.


Syntax Analysis:

The ParserQ6New class implements an LL(1) predictive parser.
It reads the tokenized input from output.txt and the parsing table from Table.txt.
It validates the input against the grammar, outputting Accepted if the input is valid or Rejected otherwise.



How to Use
Prerequisites

A C++ compiler (e.g., g++).
The input files (input.txt, Grammer.txt, automaton.txt, transition.txt, tokens.txt, keywords.txt) must be prepared in the correct format.

Steps

Prepare Input:

Create an input.txt file with the graph structure to parse, e.g., (41 55)-(44 red)-(6 8).
Ensure Grammer.txt, automaton.txt, transition.txt, tokens.txt, and keywords.txt are correctly configured.


Compile the Code:

Compile the C++ files using a command like:g++ -o compiler Scanner.h First.h Follow.h PredictiveTable.h ParserQ6New.h main.cpp


Note: You need a main.cpp file to instantiate and run the classes. An example is provided below.


Run the Compiler:

Execute the compiled program, specifying the input and output file paths as needed.
The program will:
Tokenize the input and write to output.txt.
Compute FIRST and FOLLOW sets, writing to First.txt and Follow.txt.
Generate the parsing table, writing to Table.txt.
Parse the tokenized input and output Accepted or Rejected.





Example main.cpp
#include "Scanner.h"
#include "First.h"
#include "Follow.h"
#include "PredictiveTable.h"
#include "ParserQ6New.h"

int main() {
    // Run the scanner
    Scanner scanner("input.txt", "output.txt", "automaton.txt", "transition.txt", "tokens.txt", "keywords.txt");

    // Compute FIRST sets
    First first("Grammer.txt", "First.txt");

    // Compute FOLLOW sets
    Follow follow("Grammer.txt", "Follow.txt", "First.txt");

    // Generate predictive parsing table
    PredictiveTable table("Grammer.txt", "Table.txt", "First.txt", "Follow.txt");

    // Run the parser
    ParserQ6New parser("Grammer.txt", "Table.txt", "output.txt");

    return 0;
}

Example Run
For the input (41 55)-(44 red)-(6 8) in input.txt:

The scanner produces the tokenized output in output.txt.
The FIRST and FOLLOW sets are computed and saved.
The parsing table is generated.
The parser validates the input and outputs Accepted.

Limitations

The grammar must be LL(1) (no left recursion or ambiguity).
The scanner assumes a specific format for input and automaton definitions.
Error handling is basic; invalid inputs may cause the program to exit with error messages.
The ParserQ5.h and ParserQ6Old.h files are not used in the main workflow and are likely deprecated.

Future Improvements

Add error recovery mechanisms in the parser.
Support more complex grammars (e.g., LR parsing).
Enhance the scanner to handle a wider range of input formats.
Include a semantic analysis phase to validate the meaning of the graph structure.

License
This project is unlicensed. Feel free to use and modify it as needed.
