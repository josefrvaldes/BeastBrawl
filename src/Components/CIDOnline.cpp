#include "CIDOnline.h"


CIDOnline::CIDOnline(uint16_t idPlayer) : idOnline{static_cast<uint16_t>(std::stoi(to_string(idPlayer) + to_string(++id)))} {

}