#pragma once

#include <cmath>
#include <vector>

struct Coordinates {
    double lat;
    double lng;
    bool operator==(const Coordinates& other) const {
        return lat == other.lat && lng == other.lng;
    }
    bool operator!=(const Coordinates& other) const {
        return !(*this == other);
    }
};

inline double ComputeDistance(Coordinates from, Coordinates to) {
    using namespace std;
    if (from == to) {
        return 0;
    }
    static const double dr = 3.1415926535 / 180.;
    return acos(sin(from.lat * dr) * sin(to.lat * dr)
                + cos(from.lat * dr) * cos(to.lat * dr) * cos(abs(from.lng - to.lng) * dr))
        * 6371000;
}

// Compute distance between the coords in vector
inline double ComputeDistance(const std::vector<Coordinates>& points) {
    double sum = 0.0;
    if (points.size() < 2) {
        return sum;
    }
    for (size_t i = 1; i < points.size(); ++i) {
        sum += ComputeDistance(points[i - 1], points[i]);
    }
    return sum;
}