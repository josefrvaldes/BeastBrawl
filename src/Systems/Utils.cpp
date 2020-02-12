#include "Utils.h"

// template <class T>
// void Utils::Serialize(unsigned char* buff, T& item, size_t& currentSize) {
//     size_t itemSize = sizeof(item);
//     memcpy(buff + currentSize, &item, itemSize);
//     currentSize += itemSize;
//     for (size_t i = 0; i < currentSize; i++) {
//         cout << "item " << i << " = " << unsigned(buff[i]) << endl;
//     }
// }

// template <class T>
// void Utils::Unserialize(T& item, unsigned char* buff, size_t& currentIndex) {
//     size_t itemSize = sizeof(item);
//     memcpy(&item, buff + currentIndex, itemSize);
//     currentIndex += itemSize;
//     // for(size_t i = 0; i < currentSize; i++) {
//     //     cout << "item " << i << " = " << unsigned(buff[i]) << endl;
//     // }
// }