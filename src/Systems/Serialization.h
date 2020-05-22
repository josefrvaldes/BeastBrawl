/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Antonio Jose Martinez Garcia
 * @author Jose Valdés Sirvent
 * 
 * 
 */
 
 
 #pragma once

#include "../../include/glm/vec3.hpp"
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include "../Components/CPowerUp.h"
#include "../Constants.h"

using namespace std;


class Serialization{
  public:
    template <typename T>
    static void Serialize(unsigned char* buff, T* item, size_t& currentSize) {
        size_t itemSize = sizeof(*item);
        memcpy(&buff[currentSize], item, itemSize);
        currentSize += itemSize;
        for (size_t i = 0; i < currentSize; i++) {
            // cout << "item " << i << " = " << unsigned(buff[i]) << endl;
        }
    }

    static void SerializeVec3(unsigned char* buff, const glm::vec3& item, size_t& currentSize) {
        Serialize(buff, &item.x, currentSize);
        Serialize(buff, &item.y, currentSize);
        Serialize(buff, &item.z, currentSize);
    }

    static glm::vec3 DeserializeVec3(unsigned char* buff, size_t& currentIndex) {
        float vec3X, vec3Y, vec3Z;
        size_t itemSize = sizeof(vec3X);
        Deserialize(&vec3X, itemSize, buff, currentIndex);
        Deserialize(&vec3Y, itemSize, buff, currentIndex);
        Deserialize(&vec3Z, itemSize, buff, currentIndex);

        return glm::vec3(vec3X, vec3Y, vec3Z);
    }

    // se almacena el tipo de powerUp en los primeros 3 bits, y luego si tiene totem el mismo, y si existe en el mapa
    static void SerializePowerUpTotem(unsigned char* buff, const typeCPowerUp& typePU, bool haveTotem, bool totemInMap, size_t& currentSize) {
        unsigned char byte{0};
        byte = SerializePowerUp(typePU);

        if (haveTotem)
            byte |= 1UL << 3;
        if (totemInMap)
            byte |= 1UL << 4;

        Serialize(buff, &byte, currentSize);
    }

    static unsigned char SerializePowerUp(const typeCPowerUp& typePU){
        unsigned char byte{0};

        switch (typePU) {
            case typeCPowerUp::RoboJorobo:
                byte |= 1UL << 0;  // 001
                break;
            case typeCPowerUp::SuperMegaNitro:
                byte |= 1UL << 1;  // 010
                break;
            case typeCPowerUp::PudinDeFrambuesa:
                byte |= 1UL << 0;
                byte |= 1UL << 1;  // 011
                break;
            case typeCPowerUp::EscudoMerluzo:
                byte |= 1UL << 2;  // 100
                break;
            case typeCPowerUp::TeleBanana:
                byte |= 1UL << 0;
                byte |= 1UL << 2;  // 101
                break;
            case typeCPowerUp::MelonMolon:
                byte |= 1UL << 1;
                byte |= 1UL << 2;  // 110
                break;
            default:
                // si no tiene PU se mantiene a 000
                break;
        }
        return byte;
    }

    static void SerializePowerUpOnly(unsigned char* buff, const typeCPowerUp& typePU, size_t& currentSize){
        unsigned char byte{0};
        byte = SerializePowerUp(typePU);

        Serialize(buff, &byte, currentSize);
    }

    static void DeserializePowerUpTotem(unsigned char* buff, typeCPowerUp& typePU, bool& haveTotem, bool& totemInMap, size_t& currentIndex) {
        unsigned char byte;
        size_t itemSize = sizeof(byte);

        Deserialize(&byte, itemSize, buff, currentIndex);

        if (!((byte >> 2) & 1U) && !((byte >> 1) & 1U) && ((byte >> 0) & 1U)) {  // 001
            typePU = typeCPowerUp::RoboJorobo;
        } else if (!((byte >> 2) & 1U) && ((byte >> 1) & 1U) && !((byte >> 0) & 1U)) {  // 010
            typePU = typeCPowerUp::SuperMegaNitro;
        } else if (!((byte >> 2) & 1U) && ((byte >> 1) & 1U) && ((byte >> 0) & 1U)) {  // 011
            typePU = typeCPowerUp::PudinDeFrambuesa;
        } else if (((byte >> 2) & 1U) && !((byte >> 1) & 1U) && !((byte >> 0) & 1U)) {  // 100
            typePU = typeCPowerUp::EscudoMerluzo;
        } else if (((byte >> 2) & 1U) && !((byte >> 1) & 1U) && ((byte >> 0) & 1U)) {  // 101
            typePU = typeCPowerUp::TeleBanana;
        } else if (((byte >> 2) & 1U) && ((byte >> 1) & 1U) && !((byte >> 0) & 1U)) {  // 110
            typePU = typeCPowerUp::MelonMolon;
        } else {
            typePU = typeCPowerUp::None;
        }

        if ((byte >> 3) & 1U)
            haveTotem = true;
        else
            haveTotem = false;

        if ((byte >> 4) & 1U)
            totemInMap = true;
        else
            totemInMap = false;
    }


    static typeCPowerUp DeserializePowerUpOnly(unsigned char* buff, size_t& currentIndex){
        typeCPowerUp typePU;
        unsigned char byte;
        size_t itemSize = sizeof(byte);

        Deserialize(&byte, itemSize, buff, currentIndex);

        if (!((byte >> 2) & 1U) && !((byte >> 1) & 1U) && ((byte >> 0) & 1U)) {  // 001
            typePU = typeCPowerUp::RoboJorobo;
        } else if (!((byte >> 2) & 1U) && ((byte >> 1) & 1U) && !((byte >> 0) & 1U)) {  // 010
            typePU = typeCPowerUp::SuperMegaNitro;
        } else if (!((byte >> 2) & 1U) && ((byte >> 1) & 1U) && ((byte >> 0) & 1U)) {  // 011
            typePU = typeCPowerUp::PudinDeFrambuesa;
        } else if (((byte >> 2) & 1U) && !((byte >> 1) & 1U) && !((byte >> 0) & 1U)) {  // 100
            typePU = typeCPowerUp::EscudoMerluzo;
        } else if (((byte >> 2) & 1U) && !((byte >> 1) & 1U) && ((byte >> 0) & 1U)) {  // 101
            typePU = typeCPowerUp::TeleBanana;
        } else if (((byte >> 2) & 1U) && ((byte >> 1) & 1U) && !((byte >> 0) & 1U)) {  // 110
            typePU = typeCPowerUp::MelonMolon;
        } else {
            typePU = typeCPowerUp::None;
        }

        return typePU;
    }

    // template <typename T>
    // static void SerializeVector(unsigned char* buff, vector<T> &vector, size_t& currentSize) {
    //     for (T elem : vector)
    //         Serialization::Serialize(buff, elem, currentSize);
    // }

    template <typename T>
    static T Deserialize(unsigned char* buff, size_t& currentIndex) {
        T item;
        size_t itemSize = sizeof(item);
        
        memcpy(&item, &buff[currentIndex], itemSize);
        currentIndex += itemSize;
        return item;
        // for(size_t i = 0; i < currentSize; i++) {
        //     cout << "item " << i << " = " << unsigned(buff[i]) << endl;
        // }
    }

    template <typename T>
    static void Deserialize(T* item, size_t itemSize, unsigned char* buff, size_t& currentIndex) {
        // size_t itemSize = sizeof(*item);
        memcpy(item, &buff[currentIndex], itemSize);
        currentIndex += itemSize;
        // for(size_t i = 0; i < currentSize; i++) {
        //     cout << "item " << i << " = " << unsigned(buff[i]) << endl;
        // }
    }

    template <typename T>
    static void SerializeVector(unsigned char* buff, vector<T>& vector, size_t& currentSize) {
        for (T elem : vector)
            Serialization::Serialize(buff, &elem, currentSize);
    }

    template <typename T>
    static void DeserializeVector(vector<T>& vector, unsigned char* buff, size_t& currentIndex) {
        size_t numElements = vector.size();
        DeserializeVector(vector, numElements, buff, currentIndex);
    }

    template <typename T>
    static vector<T> DeserializeVector(size_t numElements, unsigned char* buff, size_t& currentIndex) {
        vector<T> vector;
        for (size_t i = 0; i < numElements; i++) {
            T elem;
            size_t itemSize = sizeof(elem);
            Serialization::Deserialize(&elem, itemSize, buff, currentIndex);
            // vector[i] = elem;
            vector.push_back(elem);
        }
        return vector;
    }

    static void SerializeInputs(unsigned char* buff, const vector<Constants::InputTypes>& inputs, size_t& currentSize) {
        unsigned char byteInputs{0};
        for (const auto& input : inputs) {
            switch (input) {
                case Constants::InputTypes::FORWARD:
                    byteInputs |= 1UL << 0;
                    break;
                case Constants::InputTypes::BACK:
                    byteInputs |= 1UL << 1;
                    break;
                case Constants::InputTypes::LEFT:
                    byteInputs |= 1UL << 2;
                    break;
                case Constants::InputTypes::RIGHT:
                    byteInputs |= 1UL << 3;
                    break;
                case Constants::InputTypes::LAUNCH_PU:
                    byteInputs |= 1UL << 4;
                    break;
                case Constants::InputTypes::CLAXON:
                    byteInputs |= 1UL << 5;
                    break;
                case Constants::InputTypes::DRIFT:
                    byteInputs |= 1UL << 6;
                    break;
                default:
                    break;
            }
        }
        Serialize(buff, &byteInputs, currentSize);
    }

    static vector<Constants::InputTypes> DeserializeInputs(unsigned char* buff, size_t& currentIndex) {
        vector<Constants::InputTypes> inputsVector;
        unsigned char input;
        size_t itemSize = sizeof(input);

        Deserialize(&input, itemSize, buff, currentIndex);

        if ((input >> 0) & 1U) {
            inputsVector.push_back(Constants::InputTypes::FORWARD);
        }
        if ((input >> 1) & 1U) {
            inputsVector.push_back(Constants::InputTypes::BACK);
        }
        if ((input >> 2) & 1U) {
            inputsVector.push_back(Constants::InputTypes::LEFT);
        }
        if ((input >> 3) & 1U) {
            inputsVector.push_back(Constants::InputTypes::RIGHT);
        }
        if ((input >> 4) & 1U) {
            inputsVector.push_back(Constants::InputTypes::LAUNCH_PU);
        }
        if ((input >> 5) & 1U) {
            inputsVector.push_back(Constants::InputTypes::CLAXON);
        }
        if ((input >> 6) & 1U) {
            inputsVector.push_back(Constants::InputTypes::DRIFT);
        }

        return inputsVector;
    }

};




    // template <typename T>
    // void SerializeSh(std::shared_ptr<unsigned char[]> buff, T* item, size_t& currentSize) {
    //     size_t itemSize = sizeof(*item);
    //     SerializeSh(buff, item, itemSize, currentSize);
    // }

    // template <typename T>
    // void SerializeSh(std::shared_ptr<unsigned char[]> buff, T* item, size_t itemSize, size_t& currentSize) {
    //     unsigned char* auxBuff = buff.get();
    //     memcpy(&auxBuff[currentSize], item, itemSize);
    //     currentSize += itemSize;
    // }

    // template <typename T>
    // void DeserializeSh(T* item, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
    //     size_t itemSize = sizeof(*item);
    //     DeserializeSh(item, itemSize, buff, currentIndex);
    // }

    // template <typename T>
    // void DeserializeSh(T* item, size_t itemSize, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
    //     unsigned char* auxBuff = buff.get();
    //     memcpy(item, &auxBuff[currentIndex], itemSize);
    //     currentIndex += itemSize;
    // }

    // template <typename T>
    // void SerializeVectorSh(std::shared_ptr<unsigned char[]> buff, vector<T> vector, size_t& currentSize) {
    //     for (T elem : vector)
    //         SerializeSh(buff, &elem, currentSize);
    // }

    // template <typename T>
    // void DeserializeVectorSh(vector<T>& vector, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
    //     size_t numElements = vector.size();
    //     DeserializeVectorSh(vector, numElements, buff, currentIndex);
    // }

    // template <typename T>
    // void DeserializeVectorSh(vector<T>& vector, size_t numElements, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
    //     for (size_t i = 0; i < numElements; i++) {
    //         T elem;
    //         size_t itemSize = sizeof(elem);
    //         DeserializeSh(&elem, itemSize, buff, currentIndex);
    //         // vector[i] = elem;
    //         vector.push_back(elem);
    //     }
    // }
    /*
    void RunSerializationTest1() {
        // enviar
        unsigned char buff[512];
        size_t currentBuffSize = 0;
        uint16_t numShort = 1578;
        uint32_t numInt = 1578578310;
        uint64_t numLong = 56781578578310;
        float numFloat = 26.15;
        Serialize(buff, &numShort, currentBuffSize);
        Serialize(buff, &numInt, currentBuffSize);
        Serialize(buff, &numLong, currentBuffSize);
        Serialize(buff, &numFloat, currentBuffSize);

        // recibir
        size_t currentIndex = 0;
        uint16_t unserializedNumShort;
        uint32_t unserializedNumInt;
        uint64_t unserializedNumLong;
        float unserializedNumFloat;
        Deserialize(&unserializedNumShort, buff, currentIndex);
        Deserialize(&unserializedNumInt, buff, currentIndex);
        Deserialize(&unserializedNumLong, buff, currentIndex);
        Deserialize(&unserializedNumFloat, buff, currentIndex);

        cout << "El num deserializado es " << unserializedNumShort << endl;
        cout << "El num deserializado es " << unserializedNumInt << endl;
        cout << "El num deserializado es " << unserializedNumLong << endl;
        cout << "El num deserializado es " << unserializedNumFloat << endl;
    }

    void RunSerializationTest2() {
        std::shared_ptr<unsigned char[]> buff ( new unsigned char[Constants::ONLINE_BUFFER_SIZE] );
        // unsigned char buff[512];

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

        Serialize(buff.get(), &idPlayer, currentBuffSize);
        Serialize(buff.get(), &numEnemies, currentBuffSize);
        SerializeVector(buff.get(), idEnemies, currentBuffSize);

        size_t currentIndex = 0;
        uint16_t deserializedIdPlayer;
        uint16_t deserializedNumEnemies;
        vector<uint16_t> deserializedIdEnemies;
        Deserialize(&deserializedIdPlayer, buff.get(), currentIndex);
        Deserialize(&deserializedNumEnemies, buff.get(), currentIndex);
        // deserializedIdEnemies.resize(deserializedNumEnemies);
        DeserializeVector(deserializedIdEnemies, deserializedNumEnemies, buff.get(), currentIndex);

        cout << "El id player deserializado es " << deserializedIdPlayer << endl;
        for (uint16_t i = 0; i < deserializedNumEnemies; i++)
            cout << "El id enemy deserializado es " << deserializedIdEnemies[i] << endl;
        cout << "C'est fini" << endl;
    }
    */
   


/*unsigned char InputForward(unsigned char byteIn){
    return byteIn |= 1UL << 0;
}
unsigned char InputBack(unsigned char byteIn){
    return byteIn |= 1UL << 0;
}
unsigned char InputLeft(unsigned char byteIn){
    return byteIn |= 1UL << 0;
}
unsigned char InputRight(unsigned char byteIn){
    return byteIn |= 1UL << 0;
}
unsigned char InputLaunchPU(unsigned char byteIn){
    return byteIn |= 1UL << 0;
}
unsigned char InputClaxon(unsigned char byteIn){
    return byteIn |= 1UL << 0;
}
unsigned char InputDrift(unsigned char byteIn){
    return byteIn |= 1UL << 0;
}

struct TInput2Func {
    Constants::InputTypes inputType;
    unsigned char (*pfunc)(unsigned char byteIn);
};

TInput2Func mapping[] = { 
    {Constants::InputTypes::FORWARD ,  Serialization::InputForward },
    {Constants::InputTypes::BACK    ,  InputBack },
    {Constants::InputTypes::LEFT    ,  InputLeft },
    {Constants::InputTypes::RIGHT   ,  InputRight },
    {Constants::InputTypes::LAUNCH_PU ,  InputLaunchPU },
    {Constants::InputTypes::CLAXON  ,  InputClaxon },
    {Constants::InputTypes::DRIFT   ,  InputDrift }
};
*/




