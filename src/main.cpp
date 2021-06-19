#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string.h>

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

std::string simulate(double sigma, int totaltime, int numofplan, std::vector<Planet> planetlist, std::string filename, std::string content)
{
    for (int i = 0; i < totaltime; i++)
    {
        if (i % 1000 == 0)
        {
            std::cout << i << std::string("\n");
        };
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
        content.append(save(filename, planetlist));
    };
    return content;
};

int main()
{
    Planet planet1;
    planet1.set(1000000000000000000000000000000.0, 0.0, 0.0, 1.0, 2.0);
    Planet planet2;
    planet2.set(1000000000000000000000000000.0, 0.0, -100000.0, 1000000000.0, 10.0);
    Planet planet3;
    planet3.set(1000000000000000000000000000.0, 0.0, 100000.0, -1000000000.0, 10.0);

    std::vector<Planet> planetlist = {planet1, planet2, planet3};

    std::string filename = "gravdata.csv";
    std::string content = "";

    content.append(simulate(0.1, 1000000, planetlist.size(), planetlist, filename, content));

    saveToFile(content, filename, planetlist.size());

    return 0;
};