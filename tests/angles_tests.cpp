#include <cmath>
#include <stdexcept>
#include <gtest/gtest.h>

#include "core/angles.hpp"

using namespace std;

TEST(NormalizeDeg, NormalizeZero) {
    EXPECT_NEAR(normalize_deg(0), 0, 0.0001); 
}

TEST(NormalizeDeg, Normalize360) {
    EXPECT_NEAR(normalize_deg(360), 0, 0.0001); 
}

TEST(NormalizeDeg, Normalize720) {
    EXPECT_NEAR(normalize_deg(720), 0, 0.0001); 
}

TEST(NormalizeDeg, Normalize450) {
    EXPECT_NEAR(normalize_deg(450), 90, 0.0001); 
}

TEST(NormalizeDeg, NormalizeNeg90) {
    EXPECT_NEAR(normalize_deg(-90), 270, 0.0001); 
}

TEST(NormalizeDeg, NormalizeNegZero) {
    EXPECT_NEAR(normalize_deg(-0), 0, 0.0001); 
}

TEST(NormalizeDeg, NearBoundary) {
    EXPECT_LT(normalize_deg(-1e-14), 360.0);
    EXPECT_GE(normalize_deg(-1e-14), 0.0);
}

TEST(AzimuthDeg, AzimuthEast) {
    EXPECT_EQ(azimuth_deg(1,0), 90); 
}

TEST(AzimuthDeg, AzimuthNorth) {
    EXPECT_EQ(azimuth_deg(0,1), 0); 
}

TEST(AzimuthDeg, AzimuthWest) {
    EXPECT_EQ(azimuth_deg(-1,0), 270); 
}

TEST(AzimuthDeg, AzimuthSouth) {
    EXPECT_EQ(azimuth_deg(0,-1), 180); 
}

TEST(AzimuthDeg, AzimuthSouthEast) {
    EXPECT_EQ(azimuth_deg(1,-1), 135); 
}
