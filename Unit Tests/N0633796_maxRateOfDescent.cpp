#ifndef BOOST_TEST_MODULE
  #define BOOST_TEST_DYN_LINK
  #define BOOST_TEST_MODULE N0633796_maxRateOfDescent
  #include <boost/test/unit_test.hpp>
#endif

#include "track.h"
#include <string>


BOOST_AUTO_TEST_SUITE( N0633796_maxRateOfDescent )

std::string GPXTracksDir = "../logs/GPX/tracks/";

/* This test case checks if a zero is returned, which means a climb or a faulty track file.
 * In this case the test case reads from a file containing a track with a climb, therefore the
 * returned value is zero, since there is no descent at all.
 */
BOOST_AUTO_TEST_CASE( checkForClimb )
{
    const bool isFileName = true;
    GPXP::Track testTrack(GPXTracksDir + "N0633796_trackWithClimb.gpx", isFileName);

    BOOST_REQUIRE(testTrack.builtOk());

    //Compare the value with 0 and abort if the value is negative.
    GPXP::speed maxRate = testTrack.maxRateOfDescent();
    BOOST_TEST_MESSAGE("Testing for climb, expected result: 0");
    BOOST_TEST_MESSAGE("maxRateOfDescent returned: " << maxRate);
    BOOST_CHECK_CLOSE(maxRate, 0, 0.01);
}

/* This test case checks what happens when there is both climb and descent in the track
 * Expected result: 30
 */
BOOST_AUTO_TEST_CASE( climbAndDescent )
{
    const bool isFileName = true;
    GPXP::Track testTrack(GPXTracksDir + "N0633796_trackWithClimbAndDescent.gpx", isFileName);

    BOOST_REQUIRE(testTrack.builtOk());

    GPXP::speed maxRate = testTrack.maxRateOfDescent();
    BOOST_TEST_MESSAGE("Testing climbAndDescent, expected result: 30");
    BOOST_TEST_MESSAGE("maxRateOfDescent returned: " << maxRate);
    BOOST_CHECK_CLOSE(maxRate, 30, 0.01);
}

/* This test case forces the program to divide by zero and return an error code (-1).
 * The test case reads a file which has no change in time between 2 positions, forcing
 * it to divide by zero.
 */
BOOST_AUTO_TEST_CASE( divideByZero )
{
    const bool isFileName = true;
    GPXP::Track testTrack(GPXTracksDir + "N0633796_divideByZero.gpx", isFileName);

    BOOST_REQUIRE(testTrack.builtOk());

    //Call the function, which will force the division by 0.
    GPXP::speed maxRate = testTrack.maxRateOfDescent();
    BOOST_TEST_MESSAGE("Testing for division by 0, expected result: -1");
    BOOST_TEST_MESSAGE("Actual result: " << maxRate);
    BOOST_REQUIRE_CLOSE(maxRate, -1, 0.01);
}

/* This test case checks if the function returns the desired descend of 30 meters.
 * The test case reads a file containing a track designed by me, in order to achieve
 * a maximum descend of 30 meters.
 */
BOOST_AUTO_TEST_CASE( resultTest )
{
    const bool isFileName = true;
    GPXP::Track testTrack(GPXTracksDir + "N0633796_resultTest.gpx", isFileName);

    BOOST_REQUIRE(testTrack.builtOk());

    //Call the function to get the result and compare it to 30 with a precision level of 0.01%.
    GPXP::speed maxRate = testTrack.maxRateOfDescent();
    BOOST_TEST_MESSAGE("Testing if the result is as expected, expected result: 30");
    BOOST_TEST_MESSAGE("Actual result: " << maxRate);
    BOOST_REQUIRE_CLOSE(maxRate, 30, 0.01);
}

/* This test case tests the function when the track consists of only one trkpt
 * Therefore it should return 0.0
 */
BOOST_AUTO_TEST_CASE( notEnoughTrkpt )
{
    const bool isFileName = true;
    GPXP::Track testTrack(GPXTracksDir + "N0633796_notEnoughTrkpt.gpx", isFileName);

    BOOST_REQUIRE(testTrack.builtOk());

    GPXP::speed maxRate = testTrack.maxRateOfDescent();
    BOOST_TEST_MESSAGE("Testing the function if there is not enough Trkpts, expected result: 0");
    BOOST_TEST_MESSAGE("Actual result: " << maxRate);
    BOOST_REQUIRE_CLOSE(maxRate, 0, 0.01);
}


BOOST_AUTO_TEST_SUITE_END()
