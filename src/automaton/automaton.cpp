#include "automaton/automaton.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

Automaton::Automaton(const std::vector<State>& states, ID initial_state, ID id) : states_{states}, initial_state_{initial_state}, id_{id} {}

Automaton::Automaton(char symbol, ID initial_state, ID id) : initial_state_{initial_state}, id_{id} {
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
    std::vector<bool> reached(states_.size());
    depth_first_search(reached, initial_state_);

    return std::none_of(states_.begin(), states_.end(), [&reached](const State& x) {
        return reached[x.id()] && x.accepting();
    });
}

bool Automaton::deterministic() const {
    return std::all_of(states_.begin(), states_.end(), [](const State& x) {
        return x.deterministic();
    });
}

bool Automaton::recognises(const std::string& word) const {
    std::set<std::pair<ID, std::size_t>> visited;
    std::stack<std::pair<ID, std::size_t>> pending;

    pending.push({initial_state_, 0});

    while (!pending.empty()) {
        auto [curr, i] = pending.top();
        pending.pop();

        if (!visited.insert({curr, i}).second) {
            continue;
        }

        if (i == word.size() && states_[curr].accepting()) {
            return true;
        }

        if (i < word.size() && word[i] == 'E') {
            pending.push({curr, i + 1});
        }

        for (const Transition& x : states_[curr].transitions()) {
            if (x.epsilon()) {
                pending.push({x.endpoint(), i});
            }
            if (i < word.size() && x.symbol() == word[i]) {
                pending.push({x.endpoint(), i + 1});
            }
        }
    }

    return false;
}

ulong Automaton::transition_count() const {
    ulong count{0};

    for (const State& x : states_) {
        count += x.transition_count();
    }

    return count;
}

std::unordered_map<ID, ID> Automaton::get_updated_ids() const {
    std::unordered_map<ID, ulong> index_of;
    index_of.reserve(states_.size());
    for (ulong pos{0}; pos < states_.size(); pos++) {
        index_of[states_[pos].id()] = pos;
    }

    std::unordered_map<ID, ID> ids;
    std::stack<ID> not_visited;
    ID i{0};

    ids.reserve(states_.size());
    ids[initial_state_] = i++;
    not_visited.push(initial_state_);

    while (!not_visited.empty()) {
        ID curr{not_visited.top()};
        not_visited.pop();

        for (const Transition& transition : states_[index_of[curr]].transitions()) {
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

    for (auto transitions{states_[curr].transitions()}; const Transition& transition : transitions) {
        if (transition.epsilon()) {
            remove_epsilons_util(start, transition.endpoint(), visited, accepting);
        } else {
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

    for (const Transition& transition : states_[curr].transitions()) {
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
            transitions.end());

        std::sort(transitions.begin(), transitions.end());

        state.set_transitions(transitions);
    }

    std::sort(states_.begin(), states_.end());
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

void Automaton::convert() {
    remove_epsilons();
    remove_unreachable_states();
    normalise();

    if (deterministic()) {
        return;
    }

    std::set<char> alphabet{get_alphabet(states_)};
    std::map<std::set<ID>, ID> subset_ids;
    std::vector<std::set<ID>> subsets;
    std::queue<ID> pending;

    std::set<ID> initial_subset{initial_state_};
    subset_ids[initial_subset] = 0;
    subsets.push_back(initial_subset);
    pending.push(0);

    std::vector<State> new_states;

    while (!pending.empty()) {
        ID id{pending.front()};
        pending.pop();

        std::set<ID> subset{subsets[id]};
        bool accepting{std::any_of(subset.begin(), subset.end(), [this](ID s) {
            return states_[s].accepting();
        })};

        State state{accepting, id};

        for (char symbol : alphabet) {
            std::set<ID> neighbours;

            for (ID s : subset) {
                for (const Transition& transition : states_[s].transitions()) {
                    if (transition.symbol() == symbol) {
                        neighbours.insert(transition.endpoint());
                    }
                }
            }

            if (neighbours.empty()) {
                continue;
            }

            auto [it, inserted] = subset_ids.try_emplace(neighbours, subsets.size());

            if (inserted) {
                subsets.push_back(neighbours);
                pending.push(it->second);
            }

            state.add_transition(symbol, it->second);
        }

        new_states.push_back(std::move(state));
    }

    states_ = std::move(new_states);
    initial_state_ = 0;
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

void Automaton::print(std::ostream& out) const {
    out << "Automaton " << id_ << '\n';
    out << "Initial state: " << initial_state_ << '\n';
    out << "States:\n";

    for (const State& state : states_) {
        out << "  " << state.id();

        if (state.id() == initial_state_) {
            out << " [initial]";
        }

        if (state.accepting()) {
            out << " [accepting]";
        }

        out << '\n';

        for (const Transition& transition :
             state.transitions()) {
            out << "  "
                << state.id()
                << " --"
                << transition.symbol()
                << "--> "
                << transition.endpoint()
                << '\n';
        }
    }
}
