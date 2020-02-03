#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <src/Constants.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <cstring>
#include <include/glm/vec3.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

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

    template <typename T>
    static void Serialize(unsigned char* buff, T& item, size_t& currentSize) {
        size_t itemSize = sizeof(item);
        memcpy(buff + currentSize, &item, itemSize);
        currentSize += itemSize;
        for (size_t i = 0; i < currentSize; i++) {
            // cout << "item " << i << " = " << unsigned(buff[i]) << endl;
        }
    }

    // template <typename T>
    // static void SerializeVector(unsigned char* buff, vector<T> &vector, size_t& currentSize) {
    //     for (T elem : vector)
    //         Utils::Serialize(buff, elem, currentSize);
    // }

    template <typename T>
    static void Deserialize(T& item, unsigned char* buff, size_t& currentIndex) {
        size_t itemSize = sizeof(item);
        memcpy(&item, buff + currentIndex, itemSize);
        currentIndex += itemSize;
        // for(size_t i = 0; i < currentSize; i++) {
        //     cout << "item " << i << " = " << unsigned(buff[i]) << endl;
        // }
    }

    template <typename T>
    static void Serialize(std::shared_ptr<unsigned char[]> buff, T& item, size_t& currentSize) {
        size_t itemSize = sizeof(item);
        // unsigned char* auxBuff = buff.get();
        // memcpy(auxBuff + currentSize, &item, itemSize);
        // currentSize += itemSize;
        // for (size_t i = 0; i < currentSize; i++) {
        //     cout << "item " << i << " = " << unsigned(buff[i]) << endl;
        // }
        Utils::Serialize(buff, item, itemSize, currentSize);
    }

    template <typename T>
    static void Serialize(std::shared_ptr<unsigned char[]> buff, T& item, size_t itemSize, size_t& currentSize) {
        // size_t itemSize = sizeof(item);
        unsigned char* auxBuff = buff.get();
        memcpy(auxBuff + currentSize, &item, itemSize);
        currentSize += itemSize;
        for (size_t i = 0; i < currentSize; i++) {
            // cout << "item " << i << " = " << unsigned(buff[i]) << endl;
        }
    }

    template <typename T>
    static void SerializeVector(std::shared_ptr<unsigned char[]> buff, vector<T> vector, size_t& currentSize) {
        for (T elem : vector)
            Utils::Serialize(buff, elem, currentSize);
    }

    template <typename T>
    static void SerializeVector(unsigned char* buff, vector<T>& vector, size_t& currentSize) {
        for (T elem : vector)
            Utils::Serialize(buff, elem, currentSize);
    }

    // template <typename T>
    // static void TestTemplate(std::shared_ptr<unsigned char[]> buff, T otro) {
    // }

    template <typename T>
    static void Deserialize(T& item, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
        size_t itemSize = sizeof(item);
        Utils::Deserialize(item, itemSize, buff, currentIndex);
    }

    template <typename T>
    static void Deserialize(T& item, size_t itemSize, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
        unsigned char* auxBuff = buff.get();
        memcpy(&item, auxBuff + currentIndex, itemSize);
        currentIndex += itemSize;
    }

    template <typename T>
    static void DeserializeVector(vector<T>& vector, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
        size_t numElements = vector.size();
        DeserializeVector(vector, numElements, buff, currentIndex);
    }

    template <typename T>
    static void DeserializeVector(vector<T>& vector, size_t numElements, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
        for (size_t i = 0; i < numElements; i++) {
            T elem;
            size_t itemSize = sizeof(elem);
            Utils::Deserialize(elem, itemSize, buff, currentIndex);
            // vector[i] = elem;
            vector.push_back(elem);
        }
    }

    static void RunSerializationTest1() {
        // enviar
        unsigned char buff[512];
        size_t currentBuffSize = 0;
        uint16_t numShort = 1578;
        uint32_t numInt = 1578578310;
        uint64_t numLong = 56781578578310;
        float numFloat = 26.15;
        Utils::Serialize(buff, numShort, currentBuffSize);
        Utils::Serialize(buff, numInt, currentBuffSize);
        Utils::Serialize(buff, numLong, currentBuffSize);
        Utils::Serialize(buff, numFloat, currentBuffSize);

        // recibir
        size_t currentIndex = 0;
        uint16_t unserializedNumShort;
        uint32_t unserializedNumInt;
        uint64_t unserializedNumLong;
        float unserializedNumFloat;
        Utils::Deserialize(unserializedNumShort, buff, currentIndex);
        Utils::Deserialize(unserializedNumInt, buff, currentIndex);
        Utils::Deserialize(unserializedNumLong, buff, currentIndex);
        Utils::Deserialize(unserializedNumFloat, buff, currentIndex);

        cout << "El num deserializado es " << unserializedNumShort << endl;
        cout << "El num deserializado es " << unserializedNumInt << endl;
        cout << "El num deserializado es " << unserializedNumLong << endl;
        cout << "El num deserializado es " << unserializedNumFloat << endl;
    }

    static void RunSerializationTest2() {
        std::shared_ptr<unsigned char[]> buff = make_shared<unsigned char[Constants::ONLINE_BUFFER_SIZE]>();

        size_t currentBuffSize = 0;
        uint16_t idPlayer = 31263;
        vector<uint16_t> idEnemies;
        // idEnemies.reserve(4);
        idEnemies.push_back(12345);
        idEnemies.push_back(54321);
        idEnemies.push_back(12321);
        idEnemies.push_back(32123);
        // unique_ptr<uint16_t[]> arrayIdEnemies (new uint16_t[idEnemies.size()]);
        // uint16_t *arrayIdEnemies = idEnemies.data(); // hay que convertir los vectores a array
        // std::copy(idEnemies.begin(), idEnemies.end(), arrayIdEnemies.get());
        // for(size_t i = 0; i < idEnemies.size(); i++)
        //     arrayIdEnemies[i] = idEnemies[i];
        // idEnemies.clear();
        uint16_t numEnemies = idEnemies.size();
        // uint16_t vectorSize = numEnemies * sizeof(uint16_t);

        Utils::Serialize(buff, idPlayer, currentBuffSize);
        Utils::Serialize(buff, numEnemies, currentBuffSize);
        Utils::SerializeVector(buff, idEnemies, currentBuffSize);

        size_t currentIndex = 0;
        uint16_t deserializedIdPlayer;
        uint16_t deserializedNumEnemies;
        vector<uint16_t> deserializedIdEnemies;
        Utils::Deserialize(deserializedIdPlayer, buff, currentIndex);
        Utils::Deserialize(deserializedNumEnemies, buff, currentIndex);
        // deserializedIdEnemies.resize(deserializedNumEnemies);
        Utils::DeserializeVector(deserializedIdEnemies, deserializedNumEnemies, buff, currentIndex);

        cout << "El id player deserializado es " << deserializedIdPlayer << endl;
        for (uint16_t i = 0; i < deserializedNumEnemies; i++)
            cout << "El id enemy deserializado es " << deserializedIdEnemies[i] << endl;
        cout << "C'est fini" << endl;
    }
};