#pragma once
#include <SFML/Graphics.hpp>

namespace RoguelikeGame
{
    class Collider
    {
    public:
        virtual ~Collider() = default;

        virtual sf::FloatRect GetBounds() const = 0;

        bool Intersects(const Collider& other) const
        {
            return GetBounds().intersects(other.GetBounds());
        }
    };
}
