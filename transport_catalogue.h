#pragma once

#include <iostream>
#include <deque>
#include <unordered_map>
#include <string_view>


struct Stop;
struct Bus;
struct BusInfo;

class DistanceHasher;

class TransportCatalogue {
public:
	void AddBus(std::string_view, const std::vector<std::string_view>&);
	void AddStop(std::string_view, const Coordinates&);
	Bus* FindBusByName(std::string_view);	
	Stop* FindStopByName(std::string_view);
	BusInfo GetBusInfo(std::string_view);

private:
	std::deque<Stop> stops_;
	std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
	std::deque<Bus> buses_;
	std::unordered_map<std::string_view, Bus*> busname_to_bus_;
	std::unordered_map<std::pair<Stop*, Stop*>, double, DistanceHasher> distances;	

};