#include "automaton/automaton.hpp"

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <utility>
#include <vector>

Automaton::Automaton(char symbol) : alphabet_{symbol}, states_{State{false}, State{true}}, initial_state_{0} {
    states_[0].add_transition(symbol, 1);
}

Automaton::Automaton(Alphabet alphabet, std::vector<State> states, StateId initial_state) : alphabet_{std::move(alphabet)}, states_{std::move(states)}, initial_state_{initial_state} {}

const Automaton::Alphabet& Automaton::alphabet() const noexcept {
    return alphabet_;
}

const std::vector<State>& Automaton::states() const noexcept {
    return states_;
}

StateId Automaton::initial_state() const noexcept {
    return initial_state_;
}

std::size_t Automaton::transition_count() const noexcept {
    auto count{0};

    for (const State& state : states_) {
        count += state.transition_count();
    }

    return count;
}

bool Automaton::empty() const {
    std::vector<bool> reached(states_.size());
    std::stack<StateId> pending;
    pending.push(initial_state_);

    while (!pending.empty()) {
        const StateId current{pending.top()};
        pending.pop();

        if (reached[current]) {
            continue;
        }

        reached[current] = true;

        if (states_[current].is_accepting_) {
            return false;
        }

        for (const Transition& transition : states_[current].transitions_) {
            pending.push(transition.destination_);
        }
    }

    return true;
}

bool Automaton::deterministic() const {
    for (const State& state : states_) {
        std::set<char> used_symbols;

        for (const Transition& transition : state.transitions_) {
            if (transition.is_epsilon() || !used_symbols.insert(transition.symbol_).second) {
                return false;
            }
        }

        if (used_symbols.size() != alphabet_.size()) {
            return false;
        }
    }

    return true;
}

bool Automaton::recognizes(std::string_view word) const {
    std::set<std::pair<StateId, std::string::size_type>> visited;
    std::stack<std::pair<StateId, std::string::size_type>> pending;
    pending.push({initial_state_, 0});

    while (!pending.empty()) {
        auto [curr, i]{pending.top()};
        pending.pop();

        if (!visited.insert({curr, i}).second) {
            continue;
        }

        if (i == word.size() && states_[curr].is_accepting_) {
            return true;
        }

        for (const Transition& transition : states_[curr].transitions_) {
            if (transition.is_epsilon()) {
                pending.push({transition.destination_, i});
            } else if (i < word.size() && transition.symbol_ == word[i]) {
                pending.push({transition.destination_, i + 1});
            }
        }
    }

    return false;
}

Automaton Automaton::determinized() const {
    Automaton result{*this};
    result.determinize();

    return result;
}

Automaton Automaton::operator+(const Automaton& other) const {
    constexpr StateId new_initial_count{1};
    const StateId lhs_offset{new_initial_count};
    const StateId rhs_offset{new_initial_count + states_.size()};

    std::vector<State> new_states;
    new_states.reserve(new_initial_count + states_.size() + other.states_.size());

    State new_initial{false};
    new_initial.add_epsilon_transition(initial_state_ + lhs_offset);
    new_initial.add_epsilon_transition(other.initial_state_ + rhs_offset);
    new_states.push_back(std::move(new_initial));

    for (State state : states_) {
        state.shift_ids(lhs_offset);
        new_states.push_back(std::move(state));
    }

    for (State state : other.states_) {
        state.shift_ids(rhs_offset);
        new_states.push_back(std::move(state));
    }

    return Automaton{combine_alphabets(alphabet_, other.alphabet_), std::move(new_states), 0};
}

Automaton Automaton::operator*(const Automaton& other) const {
    const StateId rhs_offset{states_.size()};
    const StateId rhs_initial{other.initial_state_ + rhs_offset};

    std::vector<State> new_states;
    new_states.reserve(states_.size() + other.states_.size());

    for (State state : states_) {
        if (state.is_accepting_) {
            state.is_accepting_ = false;
            state.add_epsilon_transition(rhs_initial);
        }

        new_states.push_back(std::move(state));
    }

    for (State state : other.states_) {
        state.shift_ids(rhs_offset);
        new_states.push_back(std::move(state));
    }

    return Automaton{combine_alphabets(alphabet_, other.alphabet_), std::move(new_states), initial_state_};
}

Automaton Automaton::operator*() const {
    constexpr StateId offset{1};
    const StateId old_initial{initial_state_ + offset};

    std::vector<State> new_states;
    new_states.reserve(states_.size() + offset);

    State new_initial{true};
    new_initial.add_epsilon_transition(old_initial);
    new_states.push_back(std::move(new_initial));

    for (State state : states_) {
        state.shift_ids(offset);

        if (state.is_accepting_) {
            state.add_epsilon_transition(old_initial);
        }

        new_states.push_back(std::move(state));
    }

    return Automaton{alphabet_, std::move(new_states), 0};
}

void Automaton::print(std::ostream& out) const {
    out << "Alphabet: ";

    for (const char symbol : alphabet_) {
        out << symbol;
    }

    out << "\nInitial state: " << initial_state_ << "\nStates:\n";

    for (auto i{0}; i < states_.size(); i++) {
        out << "  " << i;

        if (i == initial_state_) {
            out << " [initial]";
        }

        if (states_[i].is_accepting_) {
            out << " [accepting]";
        }

        out << '\n';

        for (const Transition& transition : states_[i].transitions_) {
            out << "  " << i << " --" << transition.symbol_ << "--> " << transition.destination_ << '\n';
        }
    }
}

Automaton::Alphabet Automaton::combine_alphabets(const Alphabet& lhs, const Alphabet& rhs) {
    Alphabet result{lhs};
    result.insert(rhs.begin(), rhs.end());

    return result;
}

void Automaton::remove_epsilons() {
    const std::vector<State> original{states_};

    for (StateId start{0}; start < states_.size(); ++start) {
        std::vector<bool> in_closure(states_.size());
        std::stack<StateId> pending;
        pending.push(start);

        std::vector<StateId> closure;

        while (!pending.empty()) {
            const StateId current{pending.top()};
            pending.pop();

            if (in_closure[current]) {
                continue;
            }

            in_closure[current] = true;
            closure.push_back(current);

            for (const Transition& transition : original[current].transitions_) {
                if (transition.is_epsilon()) {
                    pending.push(transition.destination_);
                }
            }
        }

        std::vector<Transition> transitions;
        bool is_accepting{false};

        for (const StateId current : closure) {
            is_accepting = is_accepting || original[current].is_accepting_;

            for (const Transition& transition : original[current].transitions_) {
                if (!transition.is_epsilon()) {
                    transitions.push_back(transition);
                }
            }
        }

        states_[start].set_is_accepting(is_accepting);
        states_[start].set_transitions(std::move(transitions));
    }
}

void Automaton::remove_unreachable_states() {
    std::vector<bool> reached(states_.size());
    std::stack<StateId> pending;
    pending.push(initial_state_);

    while (!pending.empty()) {
        const StateId current{pending.top()};
        pending.pop();

        if (reached[current]) {
            continue;
        }

        reached[current] = true;

        for (const Transition& transition : states_[current].transitions_) {
            pending.push(transition.destination_);
        }
    }

    std::vector<StateId> new_ids(states_.size());
    StateId next_id{0};

    for (StateId old_id{0}; old_id < states_.size(); old_id++) {
        if (reached[old_id]) {
            new_ids[old_id] = next_id++;
        }
    }

    std::vector<State> reachable_states;
    reachable_states.reserve(next_id);

    for (StateId old_id{0}; old_id < states_.size(); old_id++) {
        if (!reached[old_id]) {
            continue;
        }

        State state{states_[old_id]};
        std::vector<Transition> transitions{state.transitions_};

        for (Transition& transition : transitions) {
            transition.destination_ = new_ids[transition.destination_];
        }

        state.set_transitions(std::move(transitions));
        reachable_states.push_back(std::move(state));
    }

    initial_state_ = new_ids[initial_state_];
    states_ = std::move(reachable_states);
}

void Automaton::normalize() {
    std::vector<bool> visited(states_.size());
    std::queue<StateId> pending;
    pending.push(initial_state_);

    std::vector<StateId> order;
    order.reserve(states_.size());

    while (!pending.empty()) {
        const StateId current{pending.front()};
        pending.pop();

        if (visited[current]) {
            continue;
        }

        visited[current] = true;
        order.push_back(current);

        for (const Transition& transition : states_[current].transitions_) {
            pending.push(transition.destination_);
        }
    }

    for (StateId id{0}; id < states_.size(); ++id) {
        if (!visited[id]) {
            order.push_back(id);
        }
    }

    std::vector<StateId> new_id(states_.size());

    for (StateId id{0}; id < order.size(); ++id) {
        new_id[order[id]] = id;
    }

    std::vector<State> reordered;
    reordered.reserve(states_.size());

    for (const StateId old_id : order) {
        State state{states_[old_id]};
        std::vector<Transition> transitions{state.transitions_};

        for (Transition& transition : transitions) {
            transition.destination_ = new_id[transition.destination_];
        }

        state.set_transitions(std::move(transitions));
        reordered.push_back(std::move(state));
    }

    initial_state_ = new_id[initial_state_];
    states_ = std::move(reordered);
}

void Automaton::determinize() {
    remove_epsilons();
    remove_unreachable_states();
    normalize();

    if (deterministic()) {
        return;
    }

    std::vector<State> deterministic_states;
    std::set<StateId> initial_subset{initial_state_};
    std::map<std::set<StateId>, StateId> subset_ids;
    std::vector<std::set<StateId>> subsets;
    std::queue<StateId> pending;

    subset_ids[initial_subset] = 0;
    subsets.push_back(initial_subset);
    pending.push(0);

    while (!pending.empty()) {
        const StateId new_state_id{pending.front()};
        pending.pop();

        const std::set<StateId> subset{subsets[new_state_id]};
        const bool is_accepting{std::ranges::any_of(subset, [this](StateId id) {
            return states_[id].is_accepting_;
        })};

        State state{is_accepting};

        for (const char symbol : alphabet_) {
            std::set<StateId> neighbors;

            for (const StateId id : subset) {
                for (const Transition& transition : states_[id].transitions_) {
                    if (transition.symbol_ == symbol) {
                        neighbors.insert(transition.destination_);
                    }
                }
            }

            auto [it, inserted]{subset_ids.try_emplace(neighbors, subsets.size())};

            if (inserted) {
                subsets.push_back(neighbors);
                pending.push(it->second);
            }

            state.add_transition(symbol, it->second);
        }

        deterministic_states.push_back(std::move(state));
    }

    states_ = std::move(deterministic_states);
    initial_state_ = 0;
}
