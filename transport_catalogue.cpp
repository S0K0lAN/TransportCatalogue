#include "transport_catalogue.h"
#include "geo.h"

#include <string>
#include <vector>
#include <cstdint>
#include <set>

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

// You can add bus only after you added all stops
void TransportCatalogue::AddBus(std::string_view name, const std::vector<std::string_view>& stopnames) {
    Bus bus{
        .name = std::string(name)
    };

    // For each stopname finds stop
    std::vector<Stop*> stops;
    for(const auto& name : stopnames){
        Stop* stop = FindStopByName(name);
        
        if(stop != nullptr){
            stops.push_back(stop);
        }
    }

    bus.stops = std::move(stops);

    buses_.push_back(std::move(bus));
    busname_to_bus_[buses_.back().name] = &buses_.back();
}

void TransportCatalogue::AddStop(std::string_view name, const Coordinates& coords) {
    stops_.push_back(Stop{ std::string(name), coords });
    stopname_to_stop_[name] = &stops_.back();
}

Bus* TransportCatalogue::FindBusByName(std::string_view name) {
    try{
        Bus* bus = busname_to_bus_.at(name);
        return bus;
    } 
    catch(std::out_of_range){
        throw;
    }
}

Stop* TransportCatalogue::FindStopByName(std::string_view name) {
    try{
        Stop* stop = stopname_to_stop_.at(name);
        return stop;
    } 
    catch(std::out_of_range){
        throw;
    }
}

BusInfo TransportCatalogue::GetBusInfo(std::string_view name) {
    const Bus* bus;
    try {
        bus = busname_to_bus_.at(name);
    } 
    catch (std::out_of_range&) {
        throw;
    }

    BusInfo info;
    info.stops_count = bus->stops.size();

    std::set<Stop*> unique_stops(bus->stops.begin(), bus->stops.end());
    info.unique_stops_count = unique_stops.size();

    std::vector<Coordinates> coords;
    for (const auto& stop : bus->stops) {
        coords.push_back(stop->coords);
    }

    info.route_length = ComputeDistance(coords);

    return info;
}