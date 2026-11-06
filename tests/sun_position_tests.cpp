#include <gtest/gtest.h>

#include "solar/sun_position.hpp"

namespace {
  constexpr double kAngleToleranceDeg = 0.5;
  constexpr double kMinuteTolerance   = 1.0;
}

struct NewYork {
    double latitude = 38.88;
    double longitude = -77.03;
    double utc_offset = -5;
    }; 

struct Jan1Noon {
    int day_of_year = 1;
    bool is_leap = false;
    int hour = 12; 
    int minute = 0; 
    }; 

// Using composition
class NewYorkJan1Noon : public ::testing::Test {
protected:
  void SetUp() override {
  day_pos = day_position(date.day_of_year, date.hour, date.minute, date.is_leap);  
  mins_since_midninght = clock_minutes_since_midnight(date.hour, date.minute); 
  }
  NewYork place;
  Jan1Noon date;
  double day_pos = 0; 
  double mins_since_midninght = 0;  
};

TEST_F(NewYorkJan1Noon, EquationOfTime) {
    EXPECT_NEAR(equation_of_time(day_pos), -3.66, kMinuteTolerance);
}

TEST_F(NewYorkJan1Noon, Declination) {
    // 0.5 accuracy for angles
    EXPECT_NEAR(declination(day_pos), -22.96, kAngleToleranceDeg);
}

TEST_F(NewYorkJan1Noon, Elevation) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.longitude, place.utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(elevation(place.latitude, decl, hour_ang), 28.13, kAngleToleranceDeg); 
}

TEST_F(NewYorkJan1Noon, Azimuth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.longitude, place.utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(azimuth(place.latitude, decl, hour_ang), 176.92, kAngleToleranceDeg); 
}

