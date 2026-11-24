#include "Menu.h"

#include <SFML/Graphics.hpp>

int main()
{
    using namespace RoguelikeGame;
    sf::Font font;
    if (!font.loadFromFile("Resources/Fonts/Roboto-Regular.ttf"))
        return 1;

    sf::RenderWindow window(sf::VideoMode(720, 480), "Roguelike");
    window.setVerticalSyncEnabled(true);
    bool playing = false;

    MenuItem options;
    options.childrenAlignment = Alignment::Middle;
    MenuItem start;
    start.text = sf::Text("New game", font, 26);
    start.onPressCallback = [&](MenuItem&) { playing = true; };
    MenuItem quit;
    quit.text = sf::Text("Exit", font, 26);
    quit.onPressCallback = [&](MenuItem&) { window.close(); };
    options.childrens = {start, quit};
    Menu menu;
    menu.Init(options);

    sf::Text title("Roguelike", font, 38);
    title.setPosition(263, 96);
    sf::Text placeholder("TO BE DONE... Press Esc to exit to menu", font, 22);
    placeholder.setPosition(151, 205);

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type != sf::Event::KeyPressed)
                continue;
            if (playing)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    playing = false;
                continue;
            }
            if (event.key.code == sf::Keyboard::Up)
                menu.SwitchToPreviousMenuItem();
            else if (event.key.code == sf::Keyboard::Down)
                menu.SwitchToNextMenuItem();
            else if (event.key.code == sf::Keyboard::Enter)
                menu.PressOnSelectedItem();
        }
        if (!window.isOpen())
            break;
        window.clear(sf::Color(16, 23, 31));
        if (playing)
            window.draw(placeholder);
        else
        {
            window.draw(title);
            menu.Draw(window, {360, 210}, {0.5f, 0.f});
        }
        window.display();
    }
}
