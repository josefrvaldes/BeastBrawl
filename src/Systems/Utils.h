#pragma once

#include <math.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include "../../include/date/date.h"
#include "../../include/glm/vec3.hpp"
#include "../Components/CPowerUp.h"
#include "../Constants.h"

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

    static string GetFullDateTime() {
        auto time_point = system_clock::now();
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(time_point.time_since_epoch()).count();
        string microsString = to_string(micros % 1000000);
        time_t now_c = system_clock::to_time_t(time_point);
        string salida = ctime(&now_c);
        salida = salida.substr(0, salida.size() - 1);
        salida = salida.substr(0, 19) + "." + microsString + " " + salida.substr(20);
        return salida;
    }

    template <class Precision>
    static string getISOCurrentTimestamp() {
        auto now = chrono::system_clock::now();
        return date::format("%FT%TZ", date::floor<Precision>(now));
    }

    static string getISOCurrentTimestampMillis() {
        return getISOCurrentTimestamp<std::chrono::milliseconds>();
    }

    static string getISOCurrentTimestampMicros() {
        return getISOCurrentTimestamp<std::chrono::microseconds>();
    }

    static string getISOCurrentTimestampSeconds() {
        return getISOCurrentTimestamp<std::chrono::seconds>();
    }

    static int64_t getMicrosSinceEpoch() {
        return std::chrono::duration_cast<std::chrono::microseconds>(system_clock::now().time_since_epoch()).count();
    }

    static int64_t getMillisSinceEpoch() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(system_clock::now().time_since_epoch()).count();
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


    // se usa para que los grados se encuentren entre 0 y 360
    template <typename T>
    static T GetAdjustedDegrees(T number){
        while(number < 0.0){
            number += 360.0;
        }
        while(number >= 360.0){
            number -= 360.0;
        }
        return number;
    }

    /**
     * Valdés: lo hice pero ni lo he probado porque en seguida me tuve
     * que poner con otra cosa. 
     * Quien lo leas: si lo pruebas y lo confirmas o lo arreglas o lo que sea,
     *      borra este comentario. Si haces uno mejor, borra todo el método.
     */
    static tm* GetUTCTime() {
        time_t rawtime;
        struct tm* ptm;

        time(&rawtime);

        ptm = gmtime(&rawtime);

        return ptm;
    }

    static float IrrlichtAngleToOpenGL(float angle){
        return (angle + 180 - (angle*2));
    }
};

    