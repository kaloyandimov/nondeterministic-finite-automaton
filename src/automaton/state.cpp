#include "automaton/state.hpp"

#include <algorithm>
#include <array>

State::State(bool accepting, ID id) : accepting_{accepting}, id_{id} {}

bool State::accepting() const {
    return accepting_;
}

ID State::id() const {
    return id_;
}

const std::vector<Transition>& State::transitions() const {
    return transitions_;
}

void State::set_accepting(bool accepting) {
    accepting_ = accepting;
}

void State::set_id(ID id) {
    id_ = id;
}

void State::set_transitions(const std::vector<Transition>& transitions) {
    transitions_ = transitions;
}

bool State::deterministic() const {
    std::array<bool, 256> seen;

    for (const Transition& x : transitions_) {
        if (x.epsilon()) {
            return false;
        }

        unsigned char symbol{static_cast<unsigned char>(x.symbol())};

        if (seen[symbol]) {
            return false;
        }

        seen[symbol] = true;
    }

    return true;
}

ulong State::transition_count() const {
    return transitions_.size();
}

void State::add_transition(const Transition& x) {
    auto pos{std::lower_bound(transitions_.begin(), transitions_.end(), x)};

    if (pos == transitions_.end() || *pos != x) {
        transitions_.insert(pos, x);
    }
}

void State::add_transition(char symbol, ID endpoint) {
    add_transition({symbol, endpoint});
}

void State::add_epsilon_transition(ID endpoint) {
    add_transition({'E', endpoint});
}

void State::add_to_ids(ulong n) {
    id_ += n;

    for (Transition& transition : transitions_) {
        transition.set_endpoint(transition.endpoint() + n);
    }
}

bool State::operator==(const State& other) const {
    return id_ == other.id_;
}

bool State::operator<(const State& other) const {
    return id_ < other.id_;
}

std::ostream& operator<<(std::ostream& out, const State& state) {
    for (const Transition& x : state.transitions_) {
        out << state.id_ << " " << x << "\n";
    }

    return out;
}
