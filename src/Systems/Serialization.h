#pragma once


#include <vector>

using namespace std;

namespace Serialization{
    template <typename T>
    void Serialize(unsigned char* buff, T* item, size_t& currentSize);
    template <typename T>
    void Deserialize(T* item, unsigned char* buff, size_t& currentIndex);
    template <typename T>
    void Deserialize(T* item, size_t itemSize, unsigned char* buff, size_t& currentIndex);
    template <typename T>
    void SerializeVector(unsigned char* buff, vector<T>& vector, size_t& currentSize);
    template <typename T>
    void DeserializeVector(vector<T>& vector, unsigned char* buff, size_t& currentIndex);
    template <typename T>
    void DeserializeVector(vector<T>& vector, size_t numElements, unsigned char* buff, size_t& currentIndex);
}


