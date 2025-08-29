#include "transport_catalogue.h"
#include "geo.h"

#include <string>
#include <vector>
#include <cstdint>

struct Stop {
    std::string name;
    Coordinates coords;
};

struct Bus {
    std::string name;
    std::vector<Stop*> stops;
};

class DistanceHasher {
public:
    size_t operator()(const std::pair<Stop*, Stop*>& key){
        uintptr_t addr1 = reinterpret_cast<uintptr_t>(key.first), 
                  addr2 = reinterpret_cast<uintptr_t>(key.second);

        return addr1 * hash_multiplier_ + addr2;
    }

    static const size_t hash_multiplier_ = 17;
};