#pragma once

#include "Component.h"

#include <string>
#include <iostream>
#include <memory>

#include <glm/vec3.hpp>

using namespace std;

class CWheel : public Component
{
public:
    CWheel(string pathTopLeft,string pathTopRight,string pathBottomLeft,string pathBottomRight,glm::vec3 _offsetTopLeft,glm::vec3 _offsetTopRight,glm::vec3 _offsetBottomLeft,glm::vec3 _offsetBottomRight,glm::vec3 _rotationTopLeft,glm::vec3 _rotationTopRight,glm::vec3 _rotationBottomLeft,glm::vec3 _rotationBottomRight,glm::vec3 _scaleTopLeft,glm::vec3 _scaleTopRight,glm::vec3 _scaleBottomLeft,glm::vec3 _scaleBottomRight);
    ~CWheel(){};

    friend ostream &operator<<(ostream &out, CWheel &c)
    {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }


    string meshTopLeft = "sharky_wheel1.obj";
    string meshTopRight = "sharky_wheel1.obj";
    string meshBottomLeft = "sharky_wheel1.obj";
    string meshBottomRight = "sharky_wheel1.obj";

    uint32_t IdWheelTopLeft;
    uint32_t IdWheelTopRight;
    uint32_t IdWheelBottomLeft;
    uint32_t IdWheelBottomRight;

    glm::vec3 offsetTopLeft;
    glm::vec3 offsetTopRight;
    glm::vec3 offsetBottomLeft;
    glm::vec3 offsetBottomRight;

    glm::vec3 rotationTopLeft;
    glm::vec3 rotationTopRight;
    glm::vec3 rotationBottomLeft;
    glm::vec3 rotationBottomRight;

    glm::vec3 scaleTopLeft;
    glm::vec3 scaleTopRight;
    glm::vec3 scaleBottomLeft;
    glm::vec3 scaleBottomRight;

};