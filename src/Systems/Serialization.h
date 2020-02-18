#pragma once

#include "../../include/glm/vec3.hpp"
#include <vector>
#include "../Components/CPowerUp.h"
#include "../Constants.h"

using namespace std;

namespace Serialization{
  template <typename T>
  static void Serialize(unsigned char* buff, T* item, size_t& currentSize);
  template <typename T>
  void SerializeVector(unsigned char* buff, vector<T>& vector, size_t& currentSize);
  void SerializeVec3(unsigned char* buff, const glm::vec3 &item, size_t& currentSize);
  void SerializePowerUpTotem(unsigned char* buff, const typeCPowerUp &typePU, bool haveTotem, bool totemInMap, size_t& currentSize);
  void SerializeInputs(unsigned char* buff, vector<Constants::InputTypes>& inputs, size_t& currentSize);


  template <typename T>
  void Deserialize(T* item, unsigned char* buff, size_t& currentIndex);
  template <typename T>
  void Deserialize(T* item, size_t itemSize, unsigned char* buff, size_t& currentIndex);
  template <typename T>
  void DeserializeVector(vector<T>& vector, unsigned char* buff, size_t& currentIndex);
  template <typename T>
  void DeserializeVector(vector<T>& vector, size_t numElements, unsigned char* buff, size_t& currentIndex);
  glm::vec3 DeserializeVec3(unsigned char* buff, size_t& currentIndex);
  void DeserializePowerUpTotem(unsigned char* buff, typeCPowerUp &typePU, bool &haveTotem, bool &totemInMap, size_t& currentIndex);
  vector<Constants::InputTypes> DeserializeInputs(unsigned char* buff, size_t& currentIndex);


}
