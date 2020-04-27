//
//  State.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "State.hpp"

int State::last_id_ = 0;

State::State(bool is_final):
    id_{last_id_++},
    is_final_{is_final} {}

int State::get_id() const {
    return id_;
}

bool State::get_is_final() const {
    return is_final_;
}

void State::set_id(int id) {
    id_ = id;
}

void State::set_is_final(bool is_final) {
    is_final_ = is_final;
}

bool State::operator<(const State& other) const {
    return id_ < other.id_;
}
