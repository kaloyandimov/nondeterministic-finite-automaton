
# Nondeterministic Finite Automaton :robot:
Write an interactive program that allows the user to work with nondeterministic finite automata with ε-moves.
## Alphabet
* Lowercase Latin letters: `a – z` :abcd:
* Substitute for ε: `E` :up:
* Digits: `0 – 9` :1234:
## Automaton Format
### :one: First line
Initial state (id) and the number of states (state_count)
```
<id> <state_count>
```
### :two: Second line 

\<state_count\> bools corresponding to whether the state is accepting or not
```
<0/1> <0/1> <0/1> ... <0/1>
```
### :three: Third line 
Number of transitions (transition_count)
```
<transition_count>
```
### :hash: Next \<transition_count\> lines 
Source state (id1), symbol \(c\) and destination state (id2)
```
<id1> <c> <id2>
<id1> <c> <id2>
...
<id1> <c> <id2>
```
## :keyboard: The following commands are supported:
| Name | Params | Usage |
|-|-|-|
| list | --- | list all ids |
| print | \<id\> | print automaton |
| empty | \<id\> | check if automaton's language is empty |
| deterministic | \<id\> | check if automaton is deterministic |
| recognise | \<id\> \<word\> | check if automaton recognises word |
| reg | \<regexpr\> | create automaton from regular expression |
| union | \<id1\> \<id2\> | find the union of two automata |
| concat | \<id1\> \<id2\> | find the concatenation of two automata |
| kleene | \<id\> | find the kleene closure of automaton |
| open | \<filename\> | open file |
| save | --- | save data to current file |
| saveas | \<filename\> | save data to file |
| saveone | \<id\> \<filename\> | save automaton to file |
| close | --- | close current file |
| help | --- | display this list |
| exit | --- | exit the program |
