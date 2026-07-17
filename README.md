# Nondeterministic Finite Automaton :robot:

This is an interactive command-line tool that empowers you to create, manipulate, and explore nondeterministic finite automata (NFAs). NFAs are fundamental mathematical models used in computer science and linguistics for processing strings and recognizing patterns. With this tool, you can define custom automata and perform various operations on them, such as finding their union, concatenation, Kleene closure, and more. Additionally, you have the flexibility to convert NFAs to deterministic finite automata (DFAs), export them as Graphviz DOT files, and save your automata to files for later use.

## Table of Contents

- [Finite Automata](#finite-automata)
- [Alphabet](#alphabet)
- [Automata Data Format](#automata-data-format)
- [Regular Expressions](#regular-expressions)
- [Supported Commands](#supported-commands)
- [Contributing](#contributing)
- [License](#license)

## Finite Automata

Formally, a finite automaton can be represented as:

```math
A=(Q,\Sigma,\delta,q_0,F)
```

where:

- $Q$ is the finite set of states;
- $\Sigma$ is the alphabet;
- $\delta : Q \times \Sigma \to \mathcal{P}(Q)$ is the transition function;
- $q_0 \in Q$ is the initial state;
- $F \subseteq Q\$ is the set of accepting states.

## Alphabet

The alphabet represents the set of input symbols that can be processed by the automata. The tool supports the following symbols:

* Lowercase Latin letters: `a – z` :abcd:
* Digits: `0 – 9` :1234:
* Substitute for ε (epsilon): `_` :large_blue_circle:

`_` is not part of an automaton's alphabet — it denotes "no symbol." It can be used as an atom inside a regular expression (e.g. `a+_` means "a or nothing"), and as the `<word>` argument to `recognize` to test whether an automaton accepts the empty string.

## Automata Data Format

A data file holds one or more automata.

### First line

The number of automata stored in the file.

```
<automaton_count>
```

### Per automaton

Each automaton is written as six fields, in order:

1. **Alphabet** — every symbol in the automaton's alphabet, concatenated with no separators (an automaton with an empty alphabet writes an empty line).
2. **Initial state** — the id of the start state.
3. **State count** — the total number of states.
4. **Acceptance flags** — `state_count` values of `0`/`1`, one per state in id order, marking which states are accepting.
5. **Transition count** — the number of transitions.
6. **Transitions** — that many lines of `<source> <symbol> <destination>`. Use `_` as `<symbol>` for an ε-transition.

```
<alphabet>
<initial_state>
<state_count>
<0/1> <0/1> ... <0/1>
<transition_count>
<source_1> <symbol_1> <destination_1>
<source_2> <symbol_2> <destination_2>
...
```

Fields are whitespace-delimited, so exact line breaks aren't load-bearing — but `save` always writes one field group per line, as shown above.

### Example

A single automaton, alphabet `{a, b}`, that accepts any string of alternating `a`/`b` starting with `a` and returning to the start state (including the empty string):

```
1
ab
0
2
1 0
2
0 a 1
1 b 0
```

Note: *A file can contain multiple automata; each is loaded and assigned its own id.*

## Regular Expressions

Regular expressions are built from alphabet symbols (and `_` for ε) using:

| Operator      | Symbol            | Meaning                                |
|---------------|-------------------|----------------------------------------|
| Union         | `+`               | `a+b` matches `a` or `b`               |
| Concatenation | `.` (or implicit) | `a.b` / `ab` matches `a` then `b`      |
| Kleene star   | `*`               | `a*` matches zero or more `a`          |
| Grouping      | `( )`             | `(a+b).c` matches `a` or `b`, then `c` |

## Supported Commands

The tool provides a set of commands to interact with the automata. Use the following commands to perform specific actions:

| Name          | Params              | Usage                                         |
|---------------|---------------------|-----------------------------------------------|
| list          | —                   | List all automaton IDs                        |
| print         | \<id\>              | Print an automaton                            |
| export        | \<id\> \<path\>     | Export an automaton as DOT                    |
| empty         | \<id\>              | Check whether the language is empty           |
| deterministic | \<id\>              | Check whether an automaton is deterministic   |
| recognize     | \<id\> \<word\>     | Check whether an automaton recognizes a word  |
| regex         | \<regex\>           | Create an automaton from a regular expression |
| union         | \<id1\> \<id2\>     | Create the union of two automata              |
| concat        | \<id1\> \<id2\>     | Concatenate two automata                      |
| kleene        | \<id\>              | Create the Kleene star of an automaton        |
| determinize   | \<id\>              | Determinize an NFA                            |
| load          | \<filename\>        | Load automata into the current workspace      |
| save          | \<filename\>        | Save all automata to a file                   |
| save          | \<id\> \<filename\> | Save one automaton to a file                  |
| remove        | \<id\>              | Remove an automaton from the workspace        |
| clear         | —                   | Remove all automata from the workspace        |
| help          | —                   | Display this list                             |
| exit          | —                   | Exit the program                              |

## Contributing

Contributions are welcome! If you have suggestions or find any issues, please feel free to submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
