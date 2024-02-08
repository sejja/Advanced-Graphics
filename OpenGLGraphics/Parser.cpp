#include "Parser.h"

#include <iostream>
#include <fstream>

float SceneParser::ReadFloat(std::ifstream& f)
{
    std::string str;
    f >> str;
    return static_cast<float>(std::atof(str.c_str()));
}

int SceneParser::ReadInt(std::ifstream& f)
{
    std::string str;
    f >> str;
    return std::atoi(str.c_str());
}

glm::vec3 SceneParser::ReadVec3(std::ifstream& f)
{
    float x = ReadFloat(f);
    float y = ReadFloat(f);
    float z = ReadFloat(f);

    return glm::vec3(x, y, z);
}

void SceneParser::LoadDataFromFile(const char* filename)
{
    std::ifstream inFile(filename);

    if (!inFile.is_open())
    {
        std::cout << "Could not open input file" << std::endl;
        exit(0);
    }

    objects.clear();

    std::string str;

    enum class LastAdded
    {
        NONE,
        OBJECT,
        LIGHT
    };
    LastAdded last = LastAdded::NONE;

    while (!inFile.eof())
    {
        str = "";
        inFile >> str;
        auto comment = str.find_first_of("#");

        if (comment == 0)
        {
            std::getline(inFile, str);
            continue;
        }

        std::string id = str;

        if (id == "fovy")
        {
            fovy = ReadFloat(inFile);
        }
        else if (id == "width")
        {
            width = ReadFloat(inFile);
        }
        else if (id == "height")
        {
            height = ReadFloat(inFile);
        }
        else if (id == "near")
        {
            nearPlane = ReadFloat(inFile);
        }
        else if (id == "far")
        {
            farPlane = ReadFloat(inFile);
        }
        else if (id == "camPosition")
        {
            camPos = ReadVec3(inFile);
        }
        else if (id == "camTarget")
        {
            camTarget = ReadVec3(inFile);
        }
        else if (id == "camUp")
        {
            camUp = ReadVec3(inFile);
        }
        else if (id == "object")
        {
            Transform newObj;
            inFile >> newObj.name;
            objects.push_back(newObj);
            last = LastAdded::OBJECT;
        }
        else if (id == "translate")
        {
            glm::vec3 pos = ReadVec3(inFile);

            if (last == LastAdded::OBJECT)
            {
                if (objects.size() > 0)
                {
                    objects.back().pos = pos;
                }
            }
            else if (last == LastAdded::LIGHT)
            {
                if (lights.size() > 0)
                {
                    lights.back().pos = pos;
                }
            }
        }
        else if (id == "rotation")
        {
            glm::vec3 rot = ReadVec3(inFile);

            if (objects.size() > 0)
            {
                objects.back().rot = rot;
            }
        }
        else if (id == "scale")
        {
            glm::vec3 sca = ReadVec3(inFile);

            if (objects.size() > 0)
            {
                objects.back().sca = sca;
            }
        }
        else if (id == "mesh")
        {
            std::string mesh;
            inFile >> mesh;
            if (objects.size() > 0)
            {
                objects.back().mesh = mesh;
            }
        }
        else if (id == "normalMap")
        {
            std::string normalMap;
            inFile >> normalMap;
            if (objects.size() > 0)
            {
                objects.back().normalMap = normalMap;
            }
        }
        else if (id == "shininess")
        {
            float ns = ReadFloat(inFile);

            if (objects.size() > 0)
            {
                objects.back().ns = ns;
            }
        }
        else if (id == "light")
        {
            Light newLight;

            lights.push_back(newLight);
            last = LastAdded::LIGHT;
        }
        else if (id == "color")
        {
            glm::vec3 col = ReadVec3(inFile);

            if (lights.size() > 0)
            {
                lights.back().col = col;
            }
        }
        else if (id == "ambient")
        {
            float ambient = ReadFloat(inFile);

            if (lights.size() > 0)
            {
                lights.back().amb = ambient;
            }
        }
        else if (id == "lightType")
        {
            std::string type;
            inFile >> type;
            if (lights.size() > 0)
            {
                lights.back().type = type;
            }
        }
        else if (id == "attenuation")
        {
            glm::vec3 att = ReadVec3(inFile);

            if (lights.size() > 0)
            {
                lights.back().att = att;
            }
        }
        else if (id == "direction")
        {
            glm::vec3 dir = ReadVec3(inFile);

            if (lights.size() > 0)
            {
                lights.back().dir = dir;
            }
        }
        else if (id == "spotAttenuation")
        {
            glm::vec3 spotAtt = ReadVec3(inFile);

            if (lights.size() > 0)
            {
                lights.back().inner = spotAtt.x;
                lights.back().outer = spotAtt.y;
                lights.back().falloff = spotAtt.z;
            }
        }
        else if (id == "bias")
        {
            float bias = ReadFloat(inFile);

            if (lights.size() > 0)
            {
                lights.back().bias = bias;
            }
        }
        else if (id == "pcf")
        {
            int pcf = ReadInt(inFile);

            if (lights.size() > 0)
            {
                lights.back().pcf = pcf;
            }
        }
    }
}