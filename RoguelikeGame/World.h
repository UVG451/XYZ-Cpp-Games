#pragma once

#include <array>
#include <functional>
#include <random>
#include <string_view>
#include <vector>

namespace dungeon
{
constexpr int Width = 25;
constexpr int Height = 17;
constexpr int Floors = 3;

struct Cell
{
    int x;
    int y;
    bool operator==(Cell other) const
    {
        return x == other.x && y == other.y;
    }
    Cell operator+(Cell other) const
    {
        return {x + other.x, y + other.y};
    }
};

struct Health
{
    int maximum;
    int armor;
    int current = maximum;

    int hit(int power);
    int heal(int amount);
};

struct Enemy
{
    Cell cell;
    Health health;
    bool guardian;
    bool angry = false;
};

enum class Outcome
{
    Exploring,
    Defeat,
    Victory
};
enum class Action
{
    Move,
    Wait,
    Heal
};
enum class Tile
{
    Wall,
    Floor
};
using Log = std::function<void(std::string_view)>;

class World
{
  public:
    World(unsigned seed, Log log);
    bool act(Action action, Cell direction = {0, 0});
    bool walkable(Cell cell) const;
    bool exitOpen() const;
    bool verify() const;

    Cell player() const
    {
        return player_;
    }
    Cell exit() const
    {
        return exit_;
    }
    const Health &health() const
    {
        return health_;
    }
    int floor() const
    {
        return floor_;
    }
    int supplies() const
    {
        return supplies_;
    }
    int turn() const
    {
        return turn_;
    }
    Outcome outcome() const
    {
        return outcome_;
    }
    const auto &enemies() const
    {
        return enemies_;
    }
    const auto &pickups() const
    {
        return pickups_;
    }
    std::string_view message() const
    {
        return message_;
    }

  private:
    using Distances = std::array<int, Width * Height>;
    Distances distances(Cell start, bool avoidEnemies = false) const;
    void generate();
    void enemyTurn();
    void note(std::string_view message);

    Log log_;
    std::mt19937 random_;
    std::array<Tile, Width * Height> map_{};
    Cell player_{1, 1};
    Cell exit_{1, 1};
    Health health_{30, 1};
    std::vector<Enemy> enemies_;
    std::vector<Cell> pickups_;
    int floor_ = 1;
    int supplies_ = 1;
    int turn_ = 0;
    Outcome outcome_ = Outcome::Exploring;
    std::string_view message_;
};
} // namespace dungeon
