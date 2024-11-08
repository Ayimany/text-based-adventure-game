#include "main.hh"

#include <string_view>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <print>
#include <random>
#include <stdexcept>

namespace ag {

    auto
    calc_exp_tp_lvup(uint16_t level) -> double {
        return constants::EXP_REQ_FACTOR
                 * std::exp(constants::EXP_REQ_EXPONENT * level)
             - constants::EXP_REQ_INITIAL;
    }

    inventory::inventory() :
    _items {} {
        std::fill(this->_items.begin(), this->_items.end(), std::nullopt);
    }

    constexpr auto
    inventory::add_item(std::string_view name) noexcept -> void {
        std::uintmax_t first_available_index = 0;

        while (!this->_items.at(first_available_index)->empty()) {
            first_available_index++;
        }

        this->_items.at(first_available_index) = name;
    }

    constexpr auto
    inventory::index_of(std::string_view name) noexcept -> std::uintmax_t {
        std::uintmax_t index = 0;

        while (this->_items.at(index).value_or("") != name) {
            index++;
        }

        // Returns _items.size() if not found.
        return index;
    }

    constexpr auto
    inventory::is_valid_index(std::uintmax_t index) noexcept -> bool {
        return index < _items.size();
    }

    constexpr auto
    inventory::remove_item(std::uintmax_t index) -> void {
        if (!this->is_valid_index(index)) {
            throw std::runtime_error {
                "Index out of bounds for inventory slot access."
            };
        }

        _items.at(index) = std::nullopt;
    }

    [[nodiscard]] constexpr auto
    inventory::get_item(std::uintmax_t index)
        -> std::optional<std::string_view> {
        if (!this->is_valid_index(index)) {
            throw std::runtime_error {
                "Index out of bounds for inventory slot assignment."
            };
        }

        return _items.at(index);
    }

    auto
    inventory::print_items() noexcept -> void {
        std::println("Items in inventory:");
        for (const auto &item : _items) {
            if (!item.has_value()) {
                continue;
            }
            const std::string_view &item_name = item.value();

            std::println("Item index: {}, Name: {}",
                         this->index_of(item_name),
                         item_name);
        }
    }

    constexpr living_entity::living_entity(double max_health, double health) :
    _max_health(max_health),
    _health { health } { }

    constexpr auto
    living_entity::get_max_health() const noexcept -> double {
        return this->_max_health;
    }

    constexpr auto
    living_entity::get_health() const noexcept -> double {
        return this->_health;
    }

    constexpr auto
    living_entity::hurt(double damage) noexcept -> void {
        this->_health -= damage;
    }

    constexpr auto
    living_entity::is_dead() const noexcept -> bool {
        return this->_health <= 0.0;
    }

    player::player(std::string name) :
    living_entity { constants::PLAYER_INITIAL_MAX_HEALTH,
                    constants::PLAYER_INITIAL_HEALTH },
    _level { constants::PLAYER_INITIAL_LEVEL },
    _experience_to_lvup { constants::PLAYER_INITIAL_EXPERIENCE_TO_LVUP },
    _experience { constants::PLAYER_INITIAL_EXPERIENCE },
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

    auto
    player::get_inventory() noexcept -> inventory & {
        return this->_inventory;
    }

    constexpr enemy::enemy(double health, double strength) :
    living_entity { health, health },
    _strength { strength } { }

    constexpr auto
    enemy::get_strength() const noexcept -> double {
        return this->_strength;
    }

} // namespace ag

/*
A lot is missing. This is just a demo.
*/
auto
main() -> int {
    // Luck stuff
    std::random_device                     rdev;
    std::mt19937                           generator(rdev());
    std::uniform_real_distribution<double> distro { 0.0, 2.0 };

    std::string name;
    std::println("What is your name?");

    std::cin >> name;
    ag::player the_player { std::move(name) };

    std::println("Welcome, {}. You will be faced by a monster.",
                 the_player.get_name());

    ag::enemy an_enemy { 100.0, 15.0 };

    while (!an_enemy.is_dead() && !the_player.is_dead()) {
        std::println("[STATS] Your Health: {}", the_player.get_health());
        std::println("[STATS] Enemy Health: {}", an_enemy.get_health());

        std::println("What wil you do?");
        std::println("a) Attack");
        std::println("b) Protect");
        std::println("c) Run");

        bool is_blocking = false;

        char option = '\0';
        std::cin >> option;

        switch (option) {
        case 'a' : {
            const double rfactor = distro(generator);
            const double damage  = the_player.get_strength() * rfactor;
            an_enemy.hurt(damage);
            std::println(">>> You deal {:.3f} damage.", damage);
        } break;
        case 'b' : {
            is_blocking = true;
            std::println(">>> You are now blocking, reducing incoming damage.");
        } break;
        case 'c' : {
            if (distro(generator) < 0.2) {
                std::println("You have escaped.");
                return 0;
            }
            std::println("You didn't manage to run away.");
        } break;
        default : {
            std::println("Invalid option. Try again.");
        }
            continue;
        }

        const double rfactor = distro(generator);
        const double damage
            = an_enemy.get_strength() * rfactor * (is_blocking ? 0.25 : 1.0);
        the_player.hurt(damage);
        std::println(">>> The enemy attacks you for {:.3f} damage.", damage);
    }

    if (an_enemy.is_dead()) {
        std::println(
            "Congratulations. You earned a luck potion and a wooden shield.");

        ag::inventory &inv = the_player.get_inventory();
        inv.add_item("Luck Potion");
        inv.add_item("Wooden Shield");

        inv.print_items();
    } else {
        std::println("You have lost. Better luck next time!");
    }

    std::println("This is only a demo.");
    return 0;
}
