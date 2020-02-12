#include "Serialization.h"

#include "../Constants.h"
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>

namespace Serialization{
    template <typename T>
    void Serialize(unsigned char* buff, T* item, size_t& currentSize) {
        size_t itemSize = sizeof(*item);
        memcpy(&buff[currentSize], item, itemSize);
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
    void Deserialize(T* item, unsigned char* buff, size_t& currentIndex) {
        size_t itemSize = sizeof(*item);
        Deserialize(item, itemSize, buff, currentIndex);
        // for(size_t i = 0; i < currentSize; i++) {
        //     cout << "item " << i << " = " << unsigned(buff[i]) << endl;
        // }
    }

    template <typename T>
    void Deserialize(T* item, size_t itemSize, unsigned char* buff, size_t& currentIndex) {
        // size_t itemSize = sizeof(*item);
        memcpy(item, &buff[currentIndex], itemSize);
        currentIndex += itemSize;
        // for(size_t i = 0; i < currentSize; i++) {
        //     cout << "item " << i << " = " << unsigned(buff[i]) << endl;
        // }
    }

    template <typename T>
    void SerializeVector(unsigned char* buff, vector<T>& vector, size_t& currentSize) {
        for (T elem : vector)
            Serialize(buff, &elem, currentSize);
    }

    template <typename T>
    void DeserializeVector(vector<T>& vector, unsigned char* buff, size_t& currentIndex) {
        size_t numElements = vector.size();
        DeserializeVector(vector, numElements, buff, currentIndex);
    }

    template <typename T>
    void DeserializeVector(vector<T>& vector, size_t numElements, unsigned char* buff, size_t& currentIndex) {
        for (size_t i = 0; i < numElements; i++) {
            T elem;
            size_t itemSize = sizeof(elem);
            Deserialize(&elem, itemSize, buff, currentIndex);
            // vector[i] = elem;
            vector.push_back(elem);
        }
    }


}



// template <typename T>
// static void SerializeSh(std::shared_ptr<unsigned char[]> buff, T* item, size_t& currentSize) {
//     size_t itemSize = sizeof(*item);
//     Utils::SerializeSh(buff, item, itemSize, currentSize);
// }

// template <typename T>
// static void SerializeSh(std::shared_ptr<unsigned char[]> buff, T* item, size_t itemSize, size_t& currentSize) {
//     unsigned char* auxBuff = buff.get();
//     memcpy(&auxBuff[currentSize], item, itemSize);
//     currentSize += itemSize;
// }

// template <typename T>
// static void DeserializeSh(T* item, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
//     size_t itemSize = sizeof(*item);
//     Utils::DeserializeSh(item, itemSize, buff, currentIndex);
// }

// template <typename T>
// static void DeserializeSh(T* item, size_t itemSize, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
//     unsigned char* auxBuff = buff.get();
//     memcpy(item, &auxBuff[currentIndex], itemSize);
//     currentIndex += itemSize;
// }

// template <typename T>
// static void SerializeVectorSh(std::shared_ptr<unsigned char[]> buff, vector<T> vector, size_t& currentSize) {
//     for (T elem : vector)
//         Utils::SerializeSh(buff, &elem, currentSize);
// }

// template <typename T>
// static void DeserializeVectorSh(vector<T>& vector, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
//     size_t numElements = vector.size();
//     DeserializeVectorSh(vector, numElements, buff, currentIndex);
// }

// template <typename T>
// static void DeserializeVectorSh(vector<T>& vector, size_t numElements, std::shared_ptr<unsigned char[]> buff, size_t& currentIndex) {
//     for (size_t i = 0; i < numElements; i++) {
//         T elem;
//         size_t itemSize = sizeof(elem);
//         Utils::DeserializeSh(&elem, itemSize, buff, currentIndex);
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