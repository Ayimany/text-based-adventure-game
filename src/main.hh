#include <cstdint>
#include <string>

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
        constexpr double   PLAYER_INITIAL_STRENGTH           = 1.0;
    } // namespace constants

    class player {
    public:
        explicit player(std::string name);

        [[nodiscard]] auto
        get_level() const noexcept -> uint16_t;

        auto
        add_experience(double experience) noexcept -> void;

        [[nodiscard]] auto
        get_max_health() const noexcept -> double;

        [[nodiscard]] auto
        get_health() const noexcept -> double;

        [[nodiscard]] auto
        get_strength() const noexcept -> double;

        [[nodiscard]] auto
        get_name() const noexcept -> std::string;

        auto
        set_name(std::string name) noexcept -> void;

    private:
        uint16_t    _level;
        double      _experience_to_lvup;
        double      _experience;
        double      _max_health;
        double      _health;
        double      _strength;
        std::string _name;
    };

} // namespace ag
