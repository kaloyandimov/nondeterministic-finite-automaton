#ifndef AUTOMATON_SYMBOL_HPP
#define AUTOMATON_SYMBOL_HPP

constexpr char epsilon_symbol{'_'};

constexpr bool is_input_symbol(char symbol) noexcept {
    return ('a' <= symbol && symbol <= 'z') || ('0' <= symbol && symbol <= '9'); 
}

#endif // AUTOMATON_SYMBOL_HPP
