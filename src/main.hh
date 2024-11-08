#include <string_view>

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace ag {

    [[nodiscard]] auto
    calc_exp_tp_lvup(uint16_t level) -> double;

    namespace constants {
        constexpr double EXP_REQ_FACTOR   = 1200.0;
        constexpr double EXP_REQ_INITIAL  = EXP_REQ_FACTOR - 100.0;
        constexpr double EXP_REQ_EXPONENT = 1200.0;

        constexpr uint16_t PLAYER_INITIAL_LEVEL              = 0;
        constexpr double   PLAYER_INITIAL_EXPERIENCE         = 0.0;
        constexpr double   PLAYER_INITIAL_EXPERIENCE_TO_LVUP = 100.0;
        constexpr double   PLAYER_INITIAL_HEALTH             = 100.0;
        constexpr double   PLAYER_INITIAL_MAX_HEALTH         = 100.0;
        constexpr double   PLAYER_INITIAL_STRENGTH           = 20.0;

        constexpr std::uintmax_t INVENTORY_SIZE = 27;
    } // namespace constants

    class inventory {
    public:
        explicit inventory();

        constexpr auto
        add_item(std::string_view name) noexcept -> void;

        [[nodiscard]] constexpr auto
        index_of(std::string_view name) noexcept -> std::uintmax_t;

        [[nodiscard]] constexpr auto
        is_valid_index(std::uintmax_t index) noexcept -> bool;

        constexpr auto
        remove_item(std::uintmax_t index) -> void;

        [[nodiscard]] constexpr auto
        get_item(std::uintmax_t index) -> std::optional<std::string_view>;

        auto
        print_items() noexcept -> void;

    private:
        // TODO: Introduce an Item class.
        std::array<std::optional<std::string_view>, constants::INVENTORY_SIZE>
            _items;
    };

    class living_entity {
    public:
        explicit constexpr living_entity(double max_health, double health);

        [[nodiscard]] constexpr auto
        get_max_health() const noexcept -> double;

        [[nodiscard]] constexpr auto
        get_health() const noexcept -> double;

        constexpr auto
        hurt(double damage) noexcept -> void;

        [[nodiscard]] constexpr auto
        is_dead() const noexcept -> bool;

    private:
        double _max_health;
        double _health;
    };

    class player : public living_entity {
    public:
        explicit player(std::string name);

        [[nodiscard]] auto
        get_level() const noexcept -> uint16_t;

        auto
        add_experience(double experience) noexcept -> void;

        [[nodiscard]] auto
        get_strength() const noexcept -> double;

        [[nodiscard]] auto
        get_name() const noexcept -> std::string;

        [[nodiscard]] auto
        get_inventory() noexcept -> inventory &;

        auto
        set_name(std::string name) noexcept -> void;

    private:
        uint16_t    _level;
        double      _experience_to_lvup;
        double      _experience;
        double      _strength;
        std::string _name;

        inventory _inventory;
    };

    class enemy : public living_entity {
    public:
        explicit constexpr enemy(double health, double strength);

        [[nodiscard]] constexpr auto
        get_strength() const noexcept -> double;

    private:
        double _strength;
    };

} // namespace ag
