/*
 * Created  by rbh, 01/11/14
 * Modified by nas, 09/11/16
 */

#ifndef PARSENMEA_H_RBH011114
#define PARSENMEA_H_RBH011114

#include <string>
#include <list>
#include <vector>
#include <utility>

#include "position.h"

namespace NMEA
{
    using std::string;
    using std::vector;
    using std::pair;

    /* The first component of the pair is a NMEA sentence type (excluding the $).
     * The second component is a vector of sentence fields, excluding the checksum.
     * The elements of the vector should not include the separating commas.
     * E.g. the first component of the pair could be "GPGLL", the first element
     * of the vector could be "5425.32", and the second element of the vector could
     * be "N".
     */
    using NMEAPair = pair<string, vector<string>>;

    /* Takes a NMEA sentence string and validates the checksum.
     */
    bool isValidSentence(const string &);

    /* Takes a (valid) NMEA sentence string and splits it into component parts.
     */
    NMEAPair decomposeSentence(const string &);

    /* Compute a Position from a NMEAPair.
     *
     * For ill-formed or unrecognised sentence types,
     * returns a Position of latitude 0.0 and longitude 0.0.
     */
    Position extractPosition(NMEAPair);

    /* Takes the name of a file containing NMEA sentences.
     * Reads the file, and returns a vector of Positions extracted from the *valid*
     * sentences.
     */
    vector<Position> routeFromNMEALog(const string & logFileName);
}

#endif
