#include "stat_reader.h"
#include <iostream>
#include <format>
#include <iomanip>

void ParseAndPrintStat(const TransportCatalogue& transport_catalogue, std::string_view request,
                       std::ostream& output) 
{
    std::string_view bus_name = request.substr(request.find(" ", 0)+1);
    output << request << ": ";
    BusInfo bus_info;
    try{
        bus_info = transport_catalogue.GetBusInfo(bus_name);
    }
    catch(std::out_of_range){
        output << "not found" << std::endl;
        return;
    }

    output << std::setprecision(6) << bus_info.stops_count << " stops on route, " << bus_info.unique_stops_count << " unique stops, " << bus_info.route_length << " route length" << std::endl;
}