#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui-SFML.h>
#include <imgui.h>
int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML and imgui work");

    ImGui::SFML::Init(window);
    sf::CircleShape shape(100.0f);
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowDemoWindow();
        ImGui::Begin("Hello, World!");
        ImGui::Button("Look at this button");
        ImGui::End();
        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();

    return 0;
}
