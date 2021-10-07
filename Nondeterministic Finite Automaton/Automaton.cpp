//
//  Automaton.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#include "Automaton.hpp"

Automaton::Automaton(const std::vector<State>& states, ID initial_state, ID id):
    states_{states}, initial_state_{initial_state}, id_{id} {}

Automaton::Automaton(char symbol, ID initial_state, ID id):
    initial_state_{initial_state}, id_{id} {
        State s{false, initial_state};
        State c{true, initial_state + 1};
        s.add_transition(symbol, initial_state + 1);
        
        states_ = {std::move(s), std::move(c)};
}

std::vector<State> Automaton::states() const {
    return states_;
}

ID Automaton::initial_state() const {
    return initial_state_;
}

ID Automaton::id() const {
    return id_;
}

void Automaton::set_states(const std::vector<State>& states) {
    states_ = states;
}

void Automaton::set_initial_state(ID initial_state) {
    initial_state_ = initial_state;
}

void Automaton::set_id(ID id) {
    id_ = id;
}

bool Automaton::empty() const {
    return std::none_of(states_.begin(), states_.end(), [](const State& x) {
        return x.accepting();
    });
}

bool Automaton::deterministic() const {
    return std::all_of(states_.begin(), states_.end(), [](const State& x) {
        return x.deterministic();
    });
}

bool Automaton::recognises_util(const std::string& word, ID curr, ulong i) const {
    if (i == word.size()) {
        return states_[curr].accepting();
    }

    for (const Transition& x : states_[curr].transitions()) {
        if ((x.symbol() == word[i] && recognises_util(word, x.endpoint(), i + 1)) ||
            (x.epsilon()           && recognises_util(word, x.endpoint(), i))) {
            return true;
        }
    }
    
    return false;
}

bool Automaton::recognises(const std::string& word) const {
    if (word == "E") {
        return states_[initial_state_].accepting();
    }
    
    return recognises_util(word, initial_state_, 0);
}

ID Automaton::max_state_id() const {
    return std::max_element(states_.begin(), states_.end())->id();
}

ulong Automaton::transition_count() const {
    ulong count{0};
    
    for (const State& x : states_) {
        count += x.transition_count();
    }
    
    return count;
}

void Automaton::normalise() {
    std::vector<std::optional<ID>> new_ids(max_state_id() + 1);
    std::stack<ID> not_visited;
    ulong i{0};
    
    new_ids[initial_state_] = i++;
    not_visited.push(initial_state_);
    
    while (!not_visited.empty()) {
        ID curr{not_visited.top()};
        not_visited.pop();
        
        for (const Transition& x: states_[curr].transitions()) {
            if (!new_ids[x.endpoint()]) {
                new_ids[x.endpoint()] = i++;
                not_visited.push(x.endpoint());
            }
        }
    }

    initial_state_ = new_ids[initial_state_].value();

    for (State& state : states_) {
        std::vector<Transition> new_transitions{state.transitions()};
        
        state.set_id(new_ids[state.id()].value());
        
        for (Transition& x : new_transitions) {
            x.set_endpoint(new_ids[x.endpoint()].value());
        }
        
        new_transitions.erase(
            std::unique(new_transitions.begin(), new_transitions.end()),
            new_transitions.end()
        );
        
        std::sort(new_transitions.begin(), new_transitions.end());
        
        state.set_transitions(new_transitions);
    }
    
    std::sort(states_.begin(), states_.end());
}

Automaton Automaton::operator+(const Automaton& other) const {
    ulong lhs_incr{1};
    ulong rhs_incr{other.states_.size() + lhs_incr};
    
    std::vector<State> lhs_states{states_};
    for (State& state : lhs_states) {
        state.add_to_ids(lhs_incr);
    }
    
    std::vector<State> rhs_states{other.states_};
    for (State& state : rhs_states) {
        state.add_to_ids(rhs_incr);
    }
    
    std::vector<State> new_states;
    std::merge(lhs_states.begin(), lhs_states.end(),
               rhs_states.begin(), rhs_states.end(),
               std::back_inserter(new_states));
    
    State new_initial{recognises("E") || other.recognises("E")};
    ID lhs_old_initial{initial_state_ + lhs_incr};
    ID rhs_old_initial{other.initial_state_ + rhs_incr};
    
    new_states[lhs_old_initial - lhs_incr].set_accepting(false);
    new_states[rhs_old_initial - rhs_incr].set_accepting(false);
    new_initial.add_epsilon_transition(lhs_old_initial);
    new_initial.add_epsilon_transition(rhs_old_initial);
    
    new_states.insert(new_states.begin(), new_initial);
    
    return Automaton{new_states};
}


Automaton Automaton::operator*(const Automaton& other) const {
    ulong incr{states_.size()};
    ID rhs_initial_state{other.initial_state_ + incr};
    
    std::vector<State> new_states{states_};
    for (State& state : new_states) {
        if (state.accepting()) {
            state.set_accepting(false);
            state.add_epsilon_transition(rhs_initial_state);
        }
    }
    
    std::vector<State> rhs_states{other.states_};
    for (State& state : rhs_states) {
        state.add_to_ids(incr);
    }
    
    std::move(rhs_states.begin(), rhs_states.end(), std::back_inserter(new_states));
    
    return Automaton{new_states};
}

Automaton Automaton::operator*() const {
    std::vector<State> new_states{states_};
    ulong incr{1};
    
    for (State& state : new_states) {
        state.add_to_ids(incr);
    }
    
    State new_initial{true};
    ID old_initial{initial_state_ + incr};
    new_states[old_initial - incr].set_accepting(false);
    new_initial.add_epsilon_transition(old_initial);
    
    for (State& state : new_states) {
        if (state.accepting()) {
            state.add_epsilon_transition(old_initial);
        }
    }
    
    new_states.insert(new_states.begin(), new_initial);
    
    return Automaton{new_states};
}

std::istream& operator>>(std::istream& in, Automaton& automaton) {
    ulong states_size{};
    
    in >> automaton.initial_state_ >> states_size;
    
    automaton.states_.clear();
    automaton.states_.reserve(states_size);
    
    bool accepting{};
    for (ulong i{0}; i < states_size; i++) {
        in >> accepting;
        
        automaton.states_.emplace_back(accepting, i);
    }
    
    ulong transition_count{};
    ID head{}, tail{};
    char symbol{};
    
    in >> transition_count;
    
    for (ulong i{0}; i < transition_count; i++) {
        in >> head >> symbol >> tail;
        
        automaton.states_[head].add_transition(symbol, tail);
    }
    
    return in;
}

std::ostream& operator<<(std::ostream& out, const Automaton& automaton) {
    out << automaton.initial_state_ << " " << automaton.states_.size() << "\n";
    
    for (const State& state : automaton.states_) {
        out << state.accepting() << " ";
    }
    
    out << "\n" << automaton.transition_count() << "\n";
    
    for (const State& state : automaton.states_) {
        out << state;
    }
    
    return out;
}
