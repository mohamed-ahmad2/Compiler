# Graph Language Compiler

## Overview

This project implements a compiler for a simple graph-like language. It processes input strings representing graph structures such as:

```plaintext
(41 55)-(44 red)-(6 8)
```

The compiler consists of the following phases:

* **Lexical Analysis**
* **Syntax Analysis**
* **FIRST and FOLLOW Set Computation**
* **LL(1) Predictive Parsing Table Generation**

It uses a context-free grammar and follows the LL(1) predictive parsing strategy to validate the input.

---

## Project Structure

### Source Files

* **`Scanner.h`**: Tokenizes the input string using a finite automaton. It reads from `input.txt` and produces tokens in `output.txt`.
* **`First.h`**: Computes FIRST sets using `Grammer.txt` and writes results to `First.txt`.
* **`Follow.h`**: Computes FOLLOW sets using `Grammer.txt` and `First.txt`, writing results to `Follow.txt`.
* **`PredictiveTable.h`**: Generates the LL(1) parsing table using the grammar, FIRST, and FOLLOW sets, and outputs to `Table.txt`.
* **`ParserQ6New.h`**: Implements an LL(1) predictive parser that reads the parsing table and token stream to validate the input.
* **`ParserQ5.h` and `ParserQ6Old.h`**: Incomplete/legacy recursive descent parsers (not used in the main compilation flow).

### Configuration Files

* **`Grammer.txt`**:

```plaintext
<Graph> -> <Tuple><G>
<G> -> -<Graph> | $
<Tuple> -> (int space<T>)
<T> -> int | color
```

* **`input.txt`**: Input to be compiled, e.g.,

```plaintext
(41 55)-(44 red)-(6 8)
```

* **`output.txt`**: Tokenized form of the input, e.g.,

```plaintext
(
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
```

* **`First.txt`**:

```plaintext
Graph    (
G        - $
Tuple    (
T        int color
```

* **`Follow.txt`**:

```plaintext
Graph    $
G        $
Tuple    - $
T        )
```

* **`Table.txt`**:

```plaintext
Graph    (       -> Tuple G
G        -       -> - Graph
         $       -> ε
Tuple    (       -> ( int space T )
T        int     -> int
         color   -> color
```

* **`automaton.txt`**: Defines states and final/error states for the DFA.
* **`transition.txt`**: DFA state transitions, e.g., `q0 ( -> q1`
* **`tokens.txt`**: Maps input characters and states to token types.
* **`keywords.txt`**: Contains keywords like `space`.

---

## Compilation Workflow

### 1. Lexical Analysis

* The scanner reads `input.txt`
* Uses automaton definitions (`automaton.txt`, `transition.txt`, `tokens.txt`, `keywords.txt`)
* Produces tokens in `output.txt`

### 2. FIRST and FOLLOW Sets

* `First.h` computes FIRST sets and writes to `First.txt`
* `Follow.h` computes FOLLOW sets and writes to `Follow.txt`

### 3. Predictive Parsing Table

* `PredictiveTable.h` builds the LL(1) parsing table using grammar, FIRST, and FOLLOW sets
* Table is saved in `Table.txt`

### 4. Syntax Analysis

* `ParserQ6New.h` uses the parsing table and tokens to parse the input
* Prints `Accepted` if valid, otherwise `Rejected`

---

## How to Use

### Prerequisites

* A C++ compiler (e.g., g++)
* Properly configured input and config files:

  * `input.txt`
  * `Grammer.txt`
  * `automaton.txt`
  * `transition.txt`
  * `tokens.txt`
  * `keywords.txt`

### Steps

#### 1. Prepare Input

* Create `input.txt` with the graph-like input.
* Ensure the grammar and DFA files are correct.

#### 2. Compile

```bash
g++ -o compiler Scanner.h First.h Follow.h PredictiveTable.h ParserQ6New.h main.cpp
```

#### 3. Run

```bash
./compiler
```

#### 4. Output

* `output.txt`: Tokenized input
* `First.txt`, `Follow.txt`: FIRST and FOLLOW sets
* `Table.txt`: Predictive parsing table
* Console output: `Accepted` or `Rejected`

---

## Example `main.cpp`

```cpp
#include "Scanner.h"
#include "First.h"
#include "Follow.h"
#include "PredictiveTable.h"
#include "ParserQ6New.h"

int main() {
    Scanner scanner("input.txt", "output.txt", "automaton.txt", "transition.txt", "tokens.txt", "keywords.txt");
    First first("Grammer.txt", "First.txt");
    Follow follow("Grammer.txt", "Follow.txt", "First.txt");
    PredictiveTable table("Grammer.txt", "Table.txt", "First.txt", "Follow.txt");
    ParserQ6New parser("Grammer.txt", "Table.txt", "output.txt");
    return 0;
}
```

---

## Limitations

* Grammar must be LL(1) and free of left recursion.
* Error handling is basic.
* Legacy parser files are unused.

---

## Future Improvements

* Add robust error recovery.
* Support more powerful parsing strategies (e.g., LR).
* Improve scanner flexibility.
* Add semantic analysis phase.
