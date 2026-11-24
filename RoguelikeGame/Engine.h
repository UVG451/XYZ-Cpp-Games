#pragma once

#include <SFML/Audio.hpp>

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

class Logger
{
  public:
    Logger() : file_("dungeon.log", std::ios::app)
    {
        if (!file_)
            std::cerr << "[engine] Unable to open dungeon.log\n";
    }

    void write(std::string_view area, std::string_view message)
    {
        std::clog << '[' << area << "] " << message << '\n';
        if (file_)
            file_ << '[' << area << "] " << message << std::endl;
    }

  private:
    std::ofstream file_;
};

class Audio
{
  public:
    explicit Audio(Logger &log)
    {
        constexpr std::array<float, 8> melody{110, 0, 130.81f, 146.83f, 164.81f, 0, 146.83f, 130.81f};
        constexpr std::array<float, 1> effect{230};
        if (load(musicBuffer_, melody.data(), melody.size(), 0.46f))
        {
            music_.setBuffer(musicBuffer_);
            music_.setLoop(true);
            music_.setVolume(28);
            music_.play();
        }
        else
            log.write("engine", "Music unavailable");
        if (load(effectBuffer_, effect.data(), effect.size(), 0.09f))
            effect_.setBuffer(effectBuffer_);
        else
            log.write("engine", "Effects unavailable");
    }

    void step()
    {
        if (effect_.getBuffer())
            effect_.play();
    }

  private:
    static bool load(sf::SoundBuffer &buffer, const float *notes, std::size_t count, float duration)
    {
        constexpr unsigned Rate = 22050;
        constexpr float Tau = 6.2831853f;
        const auto length = static_cast<unsigned>(Rate * duration);
        std::vector<sf::Int16> samples;
        samples.reserve(count * length);
        for (std::size_t note = 0; note < count; ++note)
            for (unsigned i = 0; i < length; ++i)
            {
                const float t = static_cast<float>(i) / static_cast<float>(length);
                const float envelope = std::sin(3.14159265f * t);
                const float wave = std::sin(Tau * notes[note] * i / Rate);
                samples.push_back(static_cast<sf::Int16>(wave * envelope * 3800));
            }
        return buffer.loadFromSamples(samples.data(), samples.size(), 1, Rate);
    }

    sf::SoundBuffer musicBuffer_;
    sf::SoundBuffer effectBuffer_;
    sf::Sound music_;
    sf::Sound effect_;
};
