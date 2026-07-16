#include "automaton/state.hpp"

#include <algorithm>
#include <utility>

#include "automaton/symbol.hpp"

State::State(bool is_accepting) : is_accepting_{is_accepting} {}

const std::vector<Transition>& State::transitions() const noexcept {
    return transitions_;
}

bool State::is_accepting() const noexcept {
    return is_accepting_;
}

std::vector<Transition>::size_type State::transition_count() const noexcept {
    return transitions_.size();
}

void State::add_transition(const Transition& transition) {
    const auto position{std::ranges::lower_bound(transitions_, transition)};

    if (position == transitions_.end() || *position != transition) {
        transitions_.insert(position, transition);
    }
}

void State::add_transition(char symbol, StateId endpoint) {
    add_transition({symbol, endpoint});
}

void State::add_epsilon_transition(StateId endpoint) {
    add_transition({epsilon_symbol, endpoint});
}

void State::set_transitions(std::vector<Transition> transitions) {
    std::sort(transitions.begin(), transitions.end());

    transitions.erase(std::unique(transitions.begin(), transitions.end()), transitions.end());

    transitions_ = std::move(transitions);
}

void State::set_is_accepting(bool is_accepting) noexcept {
    is_accepting_ = is_accepting;
}

void State::shift_ids(StateId offset) {
    for (Transition& transition : transitions_) {
        transition.destination_ += offset;
    }
}
