#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string.h>
#include <SFML/Graphics.hpp>

class Planet
{
public:
    double mass;
    double accx;
    double accy;
    double posx;
    double posy;
    void set(double, double, double, double, double);
    double dist(Planet);
    void move(Planet, float);
};

double Planet::dist(Planet self2)
{
    return sqrt((pow(abs(self2.posx - posx), 2.0)) + (pow(abs(self2.posy - posy), 2.0)));
};

void Planet::move(Planet self2, float sigma)
{
    double distance = dist(self2);
    double force = ((0.0000000000667 * ((self2.mass) / (pow(distance, 2.0)))));
    accx -= ((((posx - self2.posx) / distance) * force) / 2 * sigma);
    accy -= ((((posy - self2.posy) / distance) * force) / 2 * sigma);
};

void Planet::set(double m, double accelerationx, double accelerationy, double positionx, double positiony)
{
    mass = m;
    accx = accelerationx;
    accy = accelerationy;
    posx = positionx;
    posy = positiony;
}

std::string save(std::string filename, std::vector<Planet> planetlist)
{
    std::string content = "";
    for (int i = 0; i < planetlist.size(); i++)
    {
        content.append(std::to_string(planetlist[i].posx));
        content.append(std::string(","));
        content.append(std::to_string(planetlist[i].posy));
        if (i != planetlist.size() - 1)
        {
            content.append(std::string(","));
        };
    };
    content.append(std::string("\n"));
    return content;
};

void saveToFile(std::string content, std::string filename, int numofplan)
{
    std::ofstream file;
    file.open(filename);
    for (int i = 0; i < numofplan; i++)
    {
        file << "posx" << i + 1 << ",";
        file << "posy" << i + 1;
        if (i != numofplan - 1)
        {
            file << ",";
        };
    };
    file << "\n";
    file << content;
    file.close();
};

std::vector<Planet> update(double sigma, int time, int numofplan, std::vector<Planet> planetlist)
{
    for (int j = 0; j < numofplan; j++)
    {
        for (int k = 0; k < numofplan; k++)
        {
            if (j != k)
            {
                planetlist[j].move(planetlist[k], sigma);
            };
        };
    };
    for (int l = 0; l < numofplan; l++)
    {
        planetlist[l].posx += (planetlist[l].accx * sigma);
        planetlist[l].posy += (planetlist[l].accy * sigma);
    };

    return planetlist;
};

void draw(sf::RenderWindow &window, std::vector<Planet> planetlist, int scale, int offsetX, int offsetY, int gameHeight, int gameWidth)
{
    int i = 1;

    for (auto planet : planetlist)
    {
        int scaledPosx = planet.posx / scale + offsetX;
        int scaledPosy = planet.posy / scale + offsetY;

        if ((scaledPosx < gameWidth && scaledPosx > 0) && (scaledPosy < gameHeight && scaledPosy > 0))
        {
            sf::CircleShape pixel(1);
            pixel.setFillColor(sf::Color(255 % i * (254 / i), 255 % i + 1 * (254 / i), 255 % i + 2 * (254 / i)));
            pixel.setPosition(scaledPosx, scaledPosy);
            window.draw(pixel);
        }

        i++;
    }
}

int main()
{
    // Init planetlist
    Planet planet1;
    planet1.set(1000000000000000000000000000000.0, 0.0, 0.0, 1.0, 2.0);
    Planet planet2;
    planet2.set(1000000000000000000000000000.0, 0.0, -100000.0, 1000000000.0, 10.0);
    Planet planet3;
    planet3.set(1000000000000000000000000000.0, 0.0, 100000.0, -1000000000.0, 10.0);
    std::vector<Planet> planetlist;
    planetlist = {planet1, planet2, planet3};

    const int gameHeight = 1000;
    const int gameWidth = 1000;

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "SFML Gravitational simulation",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Initialize the pause message
    sf::Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(40);
    pauseMessage.setPosition(gameHeight / 8, gameWidth / 2);
    pauseMessage.setFillColor(sf::Color::White);
    pauseMessage.setString("Press space to start the game \n Press again to pause \n To show traces, press return");

    // Initialize the time message
    sf::Text timeMessage;
    timeMessage.setFont(font);
    timeMessage.setCharacterSize(30);
    timeMessage.setPosition(1, 1);
    timeMessage.setFillColor(sf::Color::White);
    timeMessage.setString("Iteration: ");

    // Initialize the scale message
    sf::Text scaleMessage;
    scaleMessage.setFont(font);
    scaleMessage.setCharacterSize(30);
    scaleMessage.setPosition(1, 50);
    scaleMessage.setFillColor(sf::Color::White);
    scaleMessage.setString("Zoom level: ");

    // Initialize the offset message
    sf::Text offsetMessage;
    offsetMessage.setFont(font);
    offsetMessage.setCharacterSize(30);
    offsetMessage.setPosition(1, 100);
    offsetMessage.setFillColor(sf::Color::White);
    offsetMessage.setString("Offset: ");

    // GAME LOOP
    sf::Clock clock;
    bool isPlaying = false;
    bool showTraces = false;

    int time = 0;
    int scale = 8000000;
    int offsetX = 500;
    int offsetY = 500;

    // Clear the window
    window.clear(sf::Color(12, 12, 12));

    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Space key pressed: play
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
            {
                isPlaying = !isPlaying;
            }

            // Return key pressed: show Traces
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return))
            {
                showTraces = !showTraces;
            }

            // Control scale
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up))
            {
                scale *= 1.07;
            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down))
            {
                scale *= 0.97;
            }

            // Control offset
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W))
            {
                offsetX += 10;
            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::A))
            {
                offsetY += 10;
            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
            {
                offsetX -= 10;
            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::D))
            {
                offsetY -= 10;
            }
        }

        if (isPlaying)
        {
            planetlist = update(10, time, planetlist.size(), planetlist);
            time++;
        }

        if (!showTraces)
        {
            // Clear the window
            window.clear(sf::Color(12, 12, 12));
        }

        if (isPlaying)
        {
            if (!showTraces)
            {
                timeMessage.setString("Iteration: " + std::to_string(time));
                window.draw(timeMessage);

                scaleMessage.setString("Zoom: " + std::to_string(scale));
                window.draw(scaleMessage);

                offsetMessage.setString("Offsets: " + std::to_string(offsetX) + " , " + std::to_string(offsetY));
                window.draw(offsetMessage);
            }

            draw(window, planetlist, scale, offsetX, offsetY, gameHeight, gameWidth);
        }
        else
        {
            window.draw(pauseMessage);
        }

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
};