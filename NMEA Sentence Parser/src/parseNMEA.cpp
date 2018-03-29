/*
 * Created by nas, 09/11/16
 * Modified by Bartlomiej Rembisz, 25/11/16
 */

#include <sstream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include "parseNMEA.h"

namespace NMEA
{
    /* Takes a NMEA sentence string and validates the checksum.
     *
     * The checksum value is a 2-digit hexadecimal number after the '*'
       character.
     * For a NMEA sentence to be valid, the checksum value should equal
       the byte-wise XOR
     * reduction of the characters codes of the raw sentence elements
       between '$' and '*'.
     */
    bool isValidSentence(const string & NMEAsentence)
    {
            std::string sentence = NMEAsentence;
            std::string checksum;
            std::string XORstring;

            for(unsigned int i = 0; i < sentence.size(); i++)
            {
                    if(sentence[i] == '*')
                    {
                            // Loop searches for the asterisk.
                            // Checksum is equal to what comes after the asterisk
                            // Sentence is equal to the sentence without the checksum
                            checksum = sentence.substr(i + 1, sentence.size());
                            sentence = sentence.substr(0, i);
                    }
            }
            // Algorithm deleting the $ sign from the sentence.
            sentence.erase(std::remove(sentence.begin(), sentence.end(), '$'), sentence.end());

            // Byte-wise XOR reduction loop
            int XORreduction = 0;
            for (unsigned int i = 0; i < sentence.size(); i++)
            {
                XORreduction = XORreduction ^ sentence[i];
            }

            // Converting the XORreduction into a hexadecimal value
            // then converting it to a string
            std::stringstream stream;
            stream << std::hex << XORreduction;
            XORstring = stream.str();

            // Capitalizing both the checksum and XORstring to avoid case-sensitive errors
            for (unsigned int i = 0; i < checksum.size(); i++)
                checksum[i] = std::toupper(checksum[i]);

            for(unsigned int i = 0; i < XORstring.size(); i++)
                XORstring[i] = std::toupper(XORstring[i]);


            // Check if the XORstring is equal to checksum,
            // True if yes, false if no
			return XORstring == checksum;
    }

    /* Takes a (valid) NMEA sentence string and splits it into component parts.
     * Returns a NMEAPair (see header file).
     */
    NMEAPair decomposeSentence(const string & NMEAsentence)
    {
            std::vector<string> sentenceVect;
            std::vector<string> tempVect;

            std::stringstream ss(NMEAsentence);
            std::string element;

            // Splitting the string with a comma as the delimiter
            while(std::getline(ss, element, ','))
            {
                    // Searching function looking for the asterisk
                    for (unsigned int i = 0; i < element.size(); i++)
                    {
                            // If the function finds the asterisk, slices the string up to the asterisk
                            if(element[i] == '*')
                            {
                                    element = element.substr(0, i);
                            }
                    }
                    // Pushing the string element into a temporary vector
                    tempVect.push_back(element);
            }

            // Searching algorithm looking for the $ sign and deleting it
            tempVect[0].erase(std::remove(tempVect[0].begin(), tempVect[0].end(), '$'), tempVect[0].end());

            // Loop which passes all elements from tempVect except for the sentence type to the sentenceVect
            for (unsigned int i = 1; i < tempVect.size(); i++) sentenceVect.push_back(tempVect[i]);

            // Returning the sentence type string and sentenceVect vector
            return make_pair(tempVect[0], sentenceVect);
    }

    /* Takes a NMEAPair, returns a Position.
     *
     * For ill-formed or unrecognized sentence types,
     * returns a Position of latitude 0.0 and longitude 0.0.
     */
    Position extractPosition(NMEAPair pair)
    {
            // Northing and easting variable will be equal to the first letter
            // of the decomposed string which holds the directions for each sentence type
            char northing;
            char easting;

            // Parsing the GPGLL sentence type
            if((pair.first == "GPGLL") &&
               (pair.second[1][0] == 'N' || pair.second[1][0] == 'S') &&
               (pair.second[3][0] == 'E' || pair.second[3][0] == 'W'))
            {
                    try
                    {
                            northing = pair.second[1][0];
                            easting = pair.second[3][0];

                            Position GPGLLPos(pair.second[0],
                                              northing,
                                              pair.second[2],
                                              easting);

                            // Returning the extracted position
                            return GPGLLPos;
                    }
                    // Returning Latitude = 0.0, Longitude = 0.0 and Elevation 0.0
                    catch(...)
                    {
                            return Position(0.0, 0.0, 0.0);
                    }
            }
            // Parsing the GPGGA sentence type
            else if(pair.first == "GPGGA" &&
                   (pair.second[2][0] == 'N' || pair.second[2][0] == 'S') &&
                   (pair.second[4][0] == 'E' || pair.second[4][0] == 'W'))
            {
                    try
                    {
                            northing = pair.second[2][0];
                            easting = pair.second[4][0];

                            Position GPGGAPos(pair.second[1],
                                              northing,
                                              pair.second[3],
                                              easting);

                            // Returning the extracted position
                            return GPGGAPos;
                    }
                    // Returning Latitude = 0.0, Longitude = 0.0 and Elevation 0.0
                    catch(...)
                    {
                            return Position(0.0, 0.0, 0.0);
                    }
            }
            // Parsing the GPRMC sentence type
            else if(pair.first == "GPRMC" &&
                   (pair.second[3][0] == 'N' || pair.second[3][0] == 'S') &&
                   (pair.second[5][0] == 'E' || pair.second[5][0] == 'W'))
            {
                    try
                    {
                            northing = pair.second[3][0];
                            easting = pair.second[5][0];

                            Position GPRMCPos(pair.second[2],
                                              northing,
                                              pair.second[4],
                                              easting);

                            // Returning the extracted position
                            return GPRMCPos;
                    }
                    // Returning Latitude = 0.0, Longitude = 0.0 and Elevation 0.0
                    catch(...)
                    {
                        return Position(0.0, 0.0, 0.0);
                    }
            }
            // Returning Latitude = 0.0, Longitude = 0.0 and Elevation 0.0 for
            // any unrecognized types
            else
                return Position(0.0, 0.0, 0.0);
    }

    /* Takes the name of a file containing NMEA sentences.
     * Reads the file.
     * Discards any blank lines.
     * Filters out any sentences that fail the checksum.
     * Returns a vector of Positions extracted from the valid sentences.
     */
    vector<Position> routeFromNMEALog(const string & logFileName)
    {
            std::string line;
            std::vector<Position> temp;
            std::ifstream file(logFileName);

            //Loop to load every line from the file
            while(std::getline(file, line))
            {

                    // Loop doesn't continue if the line is empty
                    if(line.empty()) continue;

                    std::istringstream iss(line);
                    std::string tempStr;

                    // Passing the string from the stringstream to tempStr to avoid
                    // symbols not related to the string which cause errors in the validation function.
                    while((iss >> tempStr))
                    {
                            // Checking if the checksum of the string is correct
                            if(isValidSentence(tempStr))
                                temp.push_back(extractPosition(decomposeSentence(tempStr))); // Pushing the extracted position into the temp vector.
                    }

            }
            return temp;
    }

}
