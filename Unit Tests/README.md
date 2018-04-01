# Unit Tests for a GPX Framework

The following unit tests test a function that returns the maximum rate of descent.
Developed using the BOOST Unit Testing libary.

**Function:** Track::maxRateOfDescent
**Test Case:** checkForClimb
**Description:** This test case checks what happens when the function parses a track that contains only
climb. It reads from a track file that has only climb. The function I have created records only
change in descent, any change in climb is omitted, therefore it will return the default value.
**Expected result:** 0.0

**Test Case:** climbAndDescent
**Description:** This test case checks what happens when the function parses a track that has both a
climb and a descent. The function I created only recorded the descent, omitting any climb, therefore
it only returns the maximum descent.
**Expected result:** 30

**Test Case:** divideByZero
**Description:** This function forces the function to divide by 0 by comparing two positions with no
time change between them.
**Expected result:** -1
**Reasoning:** The expected result is -1, because I made the function check whether the denominator
is 0 and I return a specific error code. The error code is negative because the function can only
return positive values, therefore I can be sure that there will be no clash between the actual result
and the error code.

**Test Case:** resultTest
**Description:** This test case checks if the function returns the expected result. It reads from a track
file designed by me so I can check whether it returns the value it should return.
**Expected result:** 30.0

**Test Case:** notEnoughTrkpt
**Description:** This test case forces the function to read from a file that contains only one trkpt tag.
Since there is only one position on the track, there is nothing to compare it to, therefore there is no
climb or descent so the function returns a default value.
**Expected result:** 0.0
