#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>
#include <sstream>
#include "../../include/glm/vec3.hpp"
#include <chrono>

using namespace glm;
using namespace std;
using namespace std::chrono;

class MakeString {
   public:
    std::stringstream stream;
    operator std::string() const { return stream.str(); }

    template <class T>
    MakeString& operator<<(T const& VAR) {
        stream << VAR;
        return *this;
    }
};

class Utils {
   public:
    const static bool ACTIVATE_COUT{false};
    static float Vec3MaxValue(const vec3& vec) {
        float maxDistance = vec.x;  // max distancia en alguno de los ejes
        if (vec.y > maxDistance)
            maxDistance = vec.y;
        if (vec.z > maxDistance)
            maxDistance = vec.z;
        return maxDistance;
    }

    static float AngleBetweenTwoAngles(float alpha, float beta) {
        float phi = static_cast<int>(fabs(beta - alpha)) % 360;  // This is either the distance or 360 - distance
        int distance = phi > 180 ? 360 - phi : phi;
        string txt = MakeString() << "El angulo1 es " << alpha << " y el angulo2 es " << beta << " y la distancia entre ambos ángulos es " << distance;
        //Cout(txt);
        return distance;
    }

    static float GetAngleFromVector(vec3& vecDirection) {
        float anguloRad = atan2(vecDirection.x, vecDirection.z);
        return RadToDeg(anguloRad);
    }

    static vec3 GetVectorFromAngle(float angle) {
        vec3 vector;
        vector.x = cos(Utils::DegToRad(angle));
        vector.y = 0.f;
        vector.z = sin(Utils::DegToRad(angle));
        return vector;
    }

    static string GetTime() {
        auto time_point = system_clock::now();
        time_t now_c = system_clock::to_time_t(time_point);
        string salida = ctime(&now_c);
        salida = salida.substr(0, salida.size() - 1);
        return salida;
    }

    static void Cout(string& txt) {
        if (ACTIVATE_COUT)
            cout << txt << endl;
    }

    static float DegToRad(float deg) {
        return deg * M_PI / 180.0;
    }

    static float RadToDeg(float rad) {
        uint32_t auxDeg = rad * (180.0 / M_PI);
        return (auxDeg + 360) % 360;
    }
};