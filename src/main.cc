#include "main.hh"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <print>

namespace ag {

    auto
    calc_exp_tp_lvup(uint16_t level) -> double {
        return constants::EXP_REQ_FACTOR
                 * std::exp(constants::EXP_REQ_EXPONENT * level)
             - constants::EXP_REQ_INITIAL;
    }

    player::player(std::string name) :
    _level { constants::PLAYER_INITIAL_LEVEL },
    _experience_to_lvup { constants::PLAYER_INITIAL_EXPERIENCE_TO_LVUP },
    _experience { constants::PLAYER_INITIAL_EXPERIENCE },
    _max_health { constants::PLAYER_INITIAL_MAX_HEALTH },
    _health { constants::PLAYER_INITIAL_HEALTH },
    _strength { constants::PLAYER_INITIAL_STRENGTH },
    _name { std::move(name) } { }

    auto
    player::get_level() const noexcept -> uint16_t {
        return this->_level;
    }

    auto
    player::add_experience(double experience) noexcept -> void {
        this->_experience += experience;
        while (this->_experience >= this->_experience_to_lvup) {
            this->_level++;
            this->_experience -= this->_experience_to_lvup;

            this->_experience_to_lvup = calc_exp_tp_lvup(this->_level);
        }
    }

    auto
    player::get_max_health() const noexcept -> double {
        return this->_max_health;
    }

    auto
    player::get_health() const noexcept -> double {
        return this->_health;
    }

    auto
    player::get_strength() const noexcept -> double {
        return this->_strength;
    }

    auto
    player::get_name() const noexcept -> std::string {
        return this->_name;
    }

    auto
    player::set_name(std::string name) noexcept -> void {
        this->_name = std::move(name);
    }

} // namespace ag

auto
main() -> int {
    std::string name;
    std::println("What is your name?");

    std::cin >> name;
    ag::player the_player { std::move(name) };

    std::println("Welcome, {}. Your adventure will begin shortly.",
                 the_player.get_name());

    return 0;
}
