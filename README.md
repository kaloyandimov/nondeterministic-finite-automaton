# Nondeterministic Finite Automaton :robot:

This is an interactive command-line tool that empowers you to create, manipulate, and explore nondeterministic finite automata (NFAs). NFAs are fundamental mathematical models used in computer science and linguistics for processing strings and recognizing patterns. With this tool, you can define custom automata and perform various operations on them, such as finding their union, concatenation, Kleene closure, and more. Additionally, you have the flexibility to convert NFAs to deterministic finite automata (DFAs) and save your automata to files for later use.

## Table of Contents

- [Alphabet](#alphabet)
- [Automata Data Format](#automata-data-format)
- [Supported Commands](#supported-commands)
- [Contributing](#contributing)
- [License](#license)

## Alphabet

The alphabet represents the set of input symbols that can be processed by the automata. The tool supports the following symbols:

* Lowercase Latin letters: `a – z` :abcd:
* Substitute for ε (epsilon): `E` :up:
* Digits: `0 – 9` :1234:

## Automata Data Format

The automata data is stored in a text file with the following format:

### :one: First line

The first line of the file contains the initial state (id) and the total number of states (state_count) in the automaton.

```  
<id> <state_count>  
```  

### :two: Second line

The second line contains `<state_count>` booleans corresponding to whether each state is accepting (1) or not (0).

```  
<0/1> <0/1> <0/1> ... <0/1>  
```  

### :three: Third line

The third line specifies the number of transitions (transition_count) in the automaton.

```  
<transition_count>  
```  

### :hash: Next `<transition_count>` lines

Each of the next `<transition_count>` lines represents a transition in the automaton, consisting of the source state (id1), the symbol (c), and the destination state (id2).

```  
<id11> <c1> <id21>  
<id12> <c2> <id22>  
<id13> <c3> <id23>  
...  
```  

### For example

```  
0 2  
1 0  
2  
0 a 1  
1 b 0  
```  

Note: *There can be multiple automata in a single file.*

## Supported Commands

The tool provides a set of commands to interact with the automata. Use the following commands to perform specific actions:

| Name          | Params              | Usage                                              |  
|---------------|---------------------|----------------------------------------------------|  
| list          | —                   | List all IDs                                       |  
| print         | \<id\>              | Print the automaton                                |  
| empty         | \<id\>              | Check if the automaton's language is empty         |  
| deterministic | \<id\>              | Check if the automaton is deterministic            |  
| recognise     | \<id\> \<word\>     | Check if the automaton recognizes the word         |  
| reg           | \<regexpr\>         | Create an automaton from the regular expression    |  
| union         | \<id1\> \<id2\>     | Find the union of two automata                     |  
| concat        | \<id1\> \<id2\>     | Find the concatenation of two automata             |  
| kleene        | \<id\>              | Find the Kleene closure of the automaton           |  
| convert       | \<id\>              | Convert the NFA to DFA                             |  
| open          | \<filename\>        | Open a file and load automata from it              |  
| save          | —                   | Save data to the current file                      |  
| saveas        | \<filename\>        | Save data to a specific file                       |  
| saveone       | \<id\> \<filename\> | Save the automaton with the specified ID to a file |  
| close         | —                   | Close the current file                             |  
| help          | —                   | Display the list of commands                       |  
| exit          | —                   | Exit the program                                   |  

## Contributing

Contributions are welcome! If you have suggestions or find any issues, please feel free to submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
