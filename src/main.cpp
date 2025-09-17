#include <SFML/Graphics.hpp>
#include <optional>

int main() {
    // SFML 3: VideoMode takes a vector size
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Your Game");
    window.setFramerateLimit(60);

    sf::CircleShape circle(50.f);
    circle.setFillColor(sf::Color::Green);
    // SFML 3: pass a vector for positions
    circle.setPosition(sf::Vector2f{100.f, 100.f});

    sf::Vector2f velocity(120.f, 80.f); // pixels per second
    sf::Clock clock;

    while (window.isOpen()) {
        // SFML 3: pollEvent returns std::optional<sf::Event>
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            }
        }

        float dt = clock.restart().asSeconds();

        // Update & bounce
        auto pos = circle.getPosition();
        pos += velocity * dt;

        const auto winSize = window.getSize();          // Vector2u
        const float diameter = circle.getRadius() * 2.f;

        if (pos.x < 0.f) { pos.x = 0.f; velocity.x = -velocity.x; }
        else if (pos.x + diameter > static_cast<float>(winSize.x)) {
            pos.x = static_cast<float>(winSize.x) - diameter; velocity.x = -velocity.x;
        }

        if (pos.y < 0.f) { pos.y = 0.f; velocity.y = -velocity.y; }
        else if (pos.y + diameter > static_cast<float>(winSize.y)) {
            pos.y = static_cast<float>(winSize.y) - diameter; velocity.y = -velocity.y;
        }

        circle.setPosition(pos);

        window.clear(sf::Color{30, 30, 40});
        window.draw(circle);
        window.display();
    }
    return 0;
}
