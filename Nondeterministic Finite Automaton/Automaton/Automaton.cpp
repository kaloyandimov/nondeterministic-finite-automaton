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

std::unordered_map<ID, ID> Automaton::get_updated_ids() const {
    std::unordered_map<ID, ID> ids;
    std::stack<ID> not_visited;
    ID i{0};
    
    ids.reserve(max_state_id() + 1);
    ids[initial_state_] = i++;
    not_visited.push(initial_state_);
    
    while (!not_visited.empty()) {
        ID curr{not_visited.top()};
        not_visited.pop();
        
        for (const Transition& transition: states_[ids[curr]].transitions()) {
            if (!ids.contains(transition.endpoint())) {
                ids[transition.endpoint()] = i++;
                not_visited.push(transition.endpoint());
            }
        }
    }
    
    return ids;
}

void Automaton::remove_epsilons_util(ID start, ID curr, std::vector<bool>& visited, bool& accepting) {
    if (visited[curr]) {
        return;
    }
    
    visited[curr] = true;
    
    if (states_[curr].accepting()) {
        accepting = true;
    }
    
    for (auto transitions{states_[curr].transitions()}; const Transition& transition: transitions) {
        if (transition.epsilon()) {
            remove_epsilons_util(start, transition.endpoint(), visited, accepting);
        }
        else {
            states_[start].add_transition(transition);
        }
    }
}

void Automaton::remove_epsilons() {
    for (State& state : states_) {
        std::vector<bool> visited(states_.size());
        bool accepting{false};
        
        remove_epsilons_util(state.id(), state.id(), visited, accepting);
        state.set_accepting(accepting);
        
        std::vector<Transition> transitions{state.transitions()};
        
        auto pos{std::remove_if(transitions.begin(), transitions.end(), [](const Transition& transition) {
            return transition.epsilon();
        })};
        
        transitions.erase(pos, transitions.end());
        state.set_transitions(transitions);
    }
}

void Automaton::depth_first_search(std::vector<bool>& reached, ID curr) const {
    reached[curr] = true;

    for (const Transition& transition: states_[curr].transitions()) {
        if (!reached[transition.endpoint()]) {
            depth_first_search(reached, transition.endpoint());
        }
    }
}

void Automaton::remove_unreachable_states() {
    std::vector<bool> reached(states_.size());
    
    depth_first_search(reached);
    
    auto pos{std::remove_if(states_.begin(), states_.end(), [&reached](const State& state) {
        return reached[state.id()] == false;
    })};
    
    states_.erase(pos, states_.end());
}

void Automaton::normalise() {
    std::unordered_map<ID, ID> ids = get_updated_ids();
    initial_state_ = ids[initial_state_];

    for (State& state : states_) {
        state.set_id(ids[state.id()]);
        
        std::vector<Transition> transitions{state.transitions()};
        
        for (Transition& transition : transitions) {
            transition.set_endpoint(ids[transition.endpoint()]);
        }
        
        transitions.erase(
            std::unique(transitions.begin(),
                        transitions.end()),
            transitions.end()
        );
        
        std::sort(transitions.begin(), transitions.end());
        
        state.set_transitions(transitions);
    }
    
    std::sort(states_.begin(), states_.end());
}

void get_power_set_util(ulong i, ulong n, std::set<ID>& set, std::vector<std::set<ID>>& power_set) {
    if (i == n) {
        power_set.push_back(set); return;
    }
    
    set.insert(i);
    get_power_set_util(i + 1, n, set, power_set);
    set.erase(i);
    get_power_set_util(i + 1, n, set, power_set);
}

std::vector<std::set<ID>> get_power_set(ulong n) {
    std::vector<std::set<ID>> power_set;
    std::set<ID> set;
    
    get_power_set_util(0, n, set, power_set);
    
    return power_set;
}

std::set<char> Automaton::get_alphabet(const std::vector<State>& states) const {
    std::set<char> alphabet;
    
    for (const State& state : states) {
        for (const Transition& transition : state.transitions()) {
            alphabet.insert(transition.symbol());
        }
    }
    
    return alphabet;
}

std::map<std::set<ID>, State> Automaton::get_determined_states(const std::vector<std::set<ID>>& power_set) const {
    std::map<std::set<ID>, State> determined_states;
    
    for (const std::set<ID>& set : power_set) {
        bool accepting{std::any_of(set.begin(), set.end(), [this](ID id) {
            return states_[id].accepting();
        })};
        
        determined_states[set] = State{accepting, determined_states.size()};
    }
    
    return determined_states;
}

void Automaton::convert() {
    remove_epsilons();
    remove_unreachable_states();
    normalise();
    
    if (deterministic()) {
        return;
    }
    
    std::vector<std::set<ID>> power_set = get_power_set(states_.size());
    std::map<std::set<ID>, State> determined_states = get_determined_states(power_set);
    
    for (auto& set : determined_states) {
        for (char symbol : get_alphabet(states_)) {
            std::set<ID> neighbours;

            for (ID id : set.first) {
                for (const Transition& transition : states_[id].transitions()) {
                    if (transition.symbol() == symbol) {
                        neighbours.insert(transition.endpoint());
                    }
                }
            }

            set.second.add_transition(determined_states[neighbours].id(), symbol);
        }
    }
    
    std::vector<State> new_states;
    new_states.reserve(determined_states.size());
    
    for (auto&& pair : determined_states) {
        new_states.push_back(std::move(pair.second));
    }
    
    states_ = std::move(new_states);
    remove_unreachable_states();
    normalise();
}

Automaton Automaton::operator+(const Automaton& other) const {
    ulong lhs_incr{1};
    ulong rhs_incr{states_.size() + lhs_incr};
    
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
    
    new_states[0].set_accepting(false);
    new_states[states_.size()].set_accepting(false);
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
