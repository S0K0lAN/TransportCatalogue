#include "transport_catalogue.h"
#include "geo.h"

#include <string>
#include <vector>
#include <unordered_set>

// You can add bus only after you added all stops
void TransportCatalogue::AddBus(std::string_view name, const std::vector<std::string_view>& stopnames) {
    Bus bus{
        .name = std::string(name),
        .stops = {}
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
    stopname_to_stop_[stops_.back().name] = &stops_.back();
}

void TransportCatalogue::AddStop(const Stop& stop){
    stops_.push_back(stop);
    stopname_to_stop_[stops_.back().name] = &stops_.back();
}

void TransportCatalogue::AddStop(Stop&& stop){
    stops_.push_back(std::move(stop));
    stopname_to_stop_[stops_.back().name] = &stops_.back();
}

Bus* TransportCatalogue::FindBusByName(std::string_view name) const {
    try{
        Bus* bus = busname_to_bus_.at(name);
        return bus;
    } 
    catch(std::out_of_range){
        throw;
    }
}

Stop* TransportCatalogue::FindStopByName(std::string_view name) const {
    try{
        Stop* stop = stopname_to_stop_.at(name);
        return stop;
    } 
    catch(std::out_of_range){
        throw;
    }
}

// exception: std::out_of_range
BusInfo TransportCatalogue::GetBusInfo(std::string_view name) const {
    const Bus* bus;
    bus = busname_to_bus_.at(name);

    BusInfo info;
    info.stops_count = bus->stops.size();

    std::unordered_set<Stop*> unique_stops(bus->stops.begin(), bus->stops.end());
    info.unique_stops_count = unique_stops.size();

    std::vector<Coordinates> coords;
    for (const auto& stop : bus->stops) {
        coords.push_back(stop->coords);
    }

    info.route_length = ComputeDistance(coords);

    return info;
}