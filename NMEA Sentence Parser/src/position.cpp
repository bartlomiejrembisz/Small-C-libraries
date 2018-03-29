/*
 * Created  by rbh, 01/11/14
 * Modified by nas, 09/11/16
 */

#include <cassert>
#include <cmath>
#include <sstream>

#include "position.h"

namespace NMEA
{
    Position::Position(degrees lat, degrees lon, degrees ele)
    {
        latitude  = lat;
        longitude = lon;
        elevation = ele;
    }

    Position::Position(const string & latStr,
                       const string & lonStr,
                       const string & eleStr)
    {
        latitude  = std::stod(latStr);
        longitude = std::stod(lonStr);
        elevation = std::stod(eleStr);
    }

    Position::Position(const string & NMEAlatStr, char northing,
                       const string & NMEAlonStr, char easting)
    {
        assert(northing == 'N' || northing == 'S');
        assert(easting  == 'E' || easting  == 'W');

        degrees absoluteLat = fromNMEAangleString(NMEAlatStr);
        degrees absoluteLon = fromNMEAangleString(NMEAlonStr);

        latitude  = (northing == 'N') ? absoluteLat : -absoluteLat; // 'S' means negative angle
        longitude = (easting  == 'E') ? absoluteLon : -absoluteLon; // 'W' means negative angle
        elevation = 0.0;
    }

    degrees Position::getLatitude() const
    {
        return latitude;
    }

    degrees Position::getLongitude() const
    {
        return longitude;
    }

    metres Position::getElevation() const
    {
        return elevation;
    }

    string Position::toString(bool includeElevation) const
    {
        std::ostringstream oss;

        oss << "lat=\""  << latitude  << "\"";
        oss << " lon=\"" << longitude << "\"";
        if (includeElevation) {
            oss << " ele=\"" << elevation << "\"";
        }

        return oss.str();
    }

    string Position::getElevationString() const
    {
        return std::to_string(elevation);
    }


    metres Position::distanceTo(const Position & target) const
    {
        return distanceBetween(*this, target);
    }

    /* Convert a positive degrees/minutes string representation of an angle to a decimal degrees
       value.
     */
    degrees Position::fromNMEAangleString(const string & NMEAangleSt)
    {
        double angle = std::stod(NMEAangleSt);
        assert(angle >= 0); // all NMEA angles are positive
        double degrees = std::floor(angle / 100);
        double minutes = angle - 100 * degrees;
        return degrees + minutes / 60.0; // converts minutes (1/60th) to decimal fractions of a degree
    }

    /* This anonymous namespace contains constants and functions local to this compilation unit.
     *
     * Note: In modern C++ it is preferable to use "anonymous namespaces" rather than the "static"
     * keyword (in the sense of static linkage) for local definitions.  One reason for this is that
     * they are more general.  Another reason is to avoid confusion, as the "static" keyword is used
     * for several other concepts, including static member functions of classes (in the object-
     * oriented sense), and local variables that persist between function calls.
     */
    namespace
    {
        using radians = double;

        const metres earth_mean_radius = 6371008.8;
        const double pi = 3.141592653589793;

        radians degToRad(degrees d)
        {
            return d * pi / 180;
        }

        // Sine squared function: sin^2(x)
        double sinSqr(double x)
        {
            int sx = sin(x);
            return sx * sx;
        }
    }

    /** Computes an approximation of the distance between two Positions.
     *
     *  See: http://en.wikipedia.org/wiki/Law_of_haversines
     */
    metres distanceBetween(const Position & p1, const Position & p2)
    {
        const radians lat1 = degToRad(p1.getLatitude());
        const radians lat2 = degToRad(p2.getLatitude());
        const radians lon1 = degToRad(p1.getLongitude());
        const radians lon2 = degToRad(p2.getLongitude());

        double h = sinSqr((lat2-lat1)/2) + cos(lat1)*cos(lat2)*sinSqr((lon2-lon1)/2);
        return 2 * earth_mean_radius * asin(sqrt(h));
    }
}
