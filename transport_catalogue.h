#pragma once

#include <iostream>
#include <deque>
#include <unordered_map>
#include <string_view>
#include <utility>
#include <cstdint>

#include "geo.h"


struct Stop {
    std::string name;
    Coordinates coords;
};

struct Bus {
    std::string name;
    std::vector<Stop*> stops;
};

struct BusInfo {
    size_t stops_count;
    size_t unique_stops_count;
    double_t route_length;
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

class TransportCatalogue {
public:
	TransportCatalogue() = default;
	void AddBus(std::string_view, const std::vector<std::string_view>&);
	void AddStop(std::string_view, const Coordinates&);
	void AddStop(const Stop& stop);
	void AddStop(Stop&& stop);
	Bus* FindBusByName(std::string_view) const;	
	Stop* FindStopByName(std::string_view) const;
	BusInfo GetBusInfo(std::string_view) const;

private:
	std::deque<Stop> stops_;
	std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
	std::deque<Bus> buses_;
	std::unordered_map<std::string_view, Bus*> busname_to_bus_;
	std::unordered_map<std::pair<Stop*, Stop*>, double, DistanceHasher> distances;	

};