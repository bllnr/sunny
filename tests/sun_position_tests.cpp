#include <gtest/gtest.h>

#include "solar/sun_position.hpp"
#include "core/core.hpp"

namespace {
  constexpr double kAngleToleranceDeg = 0.5;
  constexpr double kMinuteTolerance   = 1.0;
}

struct Washington {
    Location loc = Location(38.88, -77.03); 
    };
    
struct Svalbard {
    Location loc = Location(78.9067, 11.8883); 
    };

struct Sydney {
    Location loc = Location(-33.87, 151.22);
    };

struct Jan1Noon {
    DateTime dt = DateTime(1, 12, 0); 
    };
    
struct Dec31Noon {
    DateTime dt = DateTime(365, 12, 0); 
    };

struct SolsticeWDC {
    DateTime dt = DateTime(172, 4, 24); 
    }; 

struct SolsticeSvalbard {
    DateTime dt = DateTime(172, 10, 24); 
    }; 

struct SolsticeSydney {
    DateTime dt = DateTime(172, 6, 24); 
    }; 

// Using composition
class WashingtonJan1Noon : public ::testing::Test {
protected:
  void SetUp() override {
    day_pos = day_position(date.dt.day(), date.dt.hour(), date.dt.min());  
    mins_since_midninght = clock_minutes_since_midnight(date.dt.hour(), date.dt.min()); 
  }
  Washington place;
  Jan1Noon date; 
  double day_pos = 0; 
  double mins_since_midninght = 0;  

  // offset depends on time and place
  double utc_offset = -5;
};

class WashingtonDec31Noon : public ::testing::Test {
protected:
  void SetUp() override {
    day_pos = day_position(date.dt.day(), date.dt.hour(), date.dt.min());  
    mins_since_midninght = clock_minutes_since_midnight(date.dt.hour(), date.dt.min());  
  }
  Washington place;
  Dec31Noon date;
  double day_pos = 0; 
  double mins_since_midninght = 0;  

  // offset depends on time and place
  double utc_offset = -5;
};

class WashingtonSolsticeWDC : public ::testing::Test {
protected:
  void SetUp() override {
  day_pos = day_position(date.dt.day(), date.dt.hour(), date.dt.min());  
  mins_since_midninght = clock_minutes_since_midnight(date.dt.hour(), date.dt.min()); 
  }
  Washington place;
  SolsticeWDC date;
  double day_pos = 0; 
  double mins_since_midninght = 0;  

  // offset depends on time and place
  double utc_offset = -4;
};

class SvalbardJan1Noon : public ::testing::Test {
protected:
  void SetUp() override {
  day_pos = day_position(date.dt.day(), date.dt.hour(), date.dt.min());  
  mins_since_midninght = clock_minutes_since_midnight(date.dt.hour(), date.dt.min()); 
  }
  Svalbard place;
  Jan1Noon date;
  double day_pos = 0; 
  double mins_since_midninght = 0;  

  // offset depends on time and place
  double utc_offset = 1;
};

class SvalbardDec31Noon : public ::testing::Test {
protected:
  void SetUp() override {
  day_pos = day_position(date.dt.day(), date.dt.hour(), date.dt.min());  
  mins_since_midninght = clock_minutes_since_midnight(date.dt.hour(), date.dt.min());  
  }
  Svalbard place;
  Dec31Noon date;
  double day_pos = 0; 
  double mins_since_midninght = 0;  

  // offset depends on time and place
  double utc_offset = 1;
};

class SvalbardSolstice : public ::testing::Test {
protected:
  void SetUp() override {
  day_pos = day_position(date.dt.day(), date.dt.hour(), date.dt.min());  
  mins_since_midninght = clock_minutes_since_midnight(date.dt.hour(), date.dt.min());  
  }
  Svalbard place;
  SolsticeSvalbard date;
  double day_pos = 0; 
  double mins_since_midninght = 0;  

  // offset depends on time and place
  double utc_offset = 2;
};

class SydneyJan1Noon : public ::testing::Test {
protected:
  void SetUp() override {
  day_pos = day_position(date.dt.day(), date.dt.hour(), date.dt.min());  
  mins_since_midninght = clock_minutes_since_midnight(date.dt.hour(), date.dt.min());  
  }
  Sydney place;
  Jan1Noon date;
  double day_pos = 0; 
  double mins_since_midninght = 0;  

  // offset depends on time and place
  double utc_offset = 11;
};

class SydneyDec31Noon : public ::testing::Test {
protected:
  void SetUp() override {
  day_pos = day_position(date.dt.day(), date.dt.hour(), date.dt.min());  
  mins_since_midninght = clock_minutes_since_midnight(date.dt.hour(), date.dt.min());  
  }
  Sydney place;
  Dec31Noon date;
  double day_pos = 0; 
  double mins_since_midninght = 0;  

  // offset depends on time and place
  double utc_offset = 11;
};

class SydneySolstice : public ::testing::Test {
protected:
  void SetUp() override {
  day_pos = day_position(date.dt.day(), date.dt.hour(), date.dt.min());  
  mins_since_midninght = clock_minutes_since_midnight(date.dt.hour(), date.dt.min());  
  }
  Sydney place;
  SolsticeSydney date;
  double day_pos = 0; 
  double mins_since_midninght = 0;  

  // offset depends on time and place
  double utc_offset = 10;
};

// TEST CASES


TEST_F(WashingtonJan1Noon, EquationOfTime) {
    EXPECT_NEAR(equation_of_time(day_pos), -3.66, kMinuteTolerance);
}

TEST_F(WashingtonJan1Noon, Declination) {
    EXPECT_NEAR(declination(day_pos), -22.96, kAngleToleranceDeg);
}

TEST_F(WashingtonJan1Noon, Elevation) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(elevation(place.loc.lat_deg(), decl, hour_ang), 28.13, kAngleToleranceDeg); 
}

TEST_F(WashingtonJan1Noon, Azimuth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(azimuth(place.loc.lat_deg(), decl, hour_ang), 176.92, kAngleToleranceDeg); 
}

TEST_F(WashingtonJan1Noon, Lit) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    double az = azimuth(place.loc.lat_deg(), decl, hour_ang); 
    double elev = elevation(place.loc.lat_deg(), decl, hour_ang); 
    EXPECT_TRUE(is_lit(180, az, elev)); 
}

TEST_F(WashingtonDec31Noon, EquationOfTime) {
    EXPECT_NEAR(equation_of_time(day_pos), -3.08, kMinuteTolerance);
}

TEST_F(WashingtonDec31Noon, Declination) {
    EXPECT_NEAR(declination(day_pos), -23.06, kAngleToleranceDeg);
}

TEST_F(WashingtonDec31Noon, Elevation) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(elevation(place.loc.lat_deg(), decl, hour_ang), 28.04, kAngleToleranceDeg); 
}

TEST_F(WashingtonDec31Noon, Azimuth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(azimuth(place.loc.lat_deg(), decl, hour_ang), 177.08, kAngleToleranceDeg); 
}

TEST_F(WashingtonSolsticeWDC, EquationOfTime) {
    EXPECT_NEAR(equation_of_time(day_pos), -1.79, kMinuteTolerance);
}

TEST_F(WashingtonSolsticeWDC, Declination) {
    EXPECT_NEAR(declination(day_pos), 23.44, kAngleToleranceDeg);
}

TEST_F(WashingtonSolsticeWDC, Elevation) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(elevation(place.loc.lat_deg(), decl, hour_ang), -12.88, kAngleToleranceDeg); 
}

TEST_F(WashingtonSolsticeWDC, Azimuth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(azimuth(place.loc.lat_deg(), decl, hour_ang), 44.85, kAngleToleranceDeg); 
}

TEST_F(WashingtonSolsticeWDC, LitMins) {
    double sun_mins = sun_mins_day(date.dt.day(), place.loc.lat_deg(), place.loc.lon_deg(), utc_offset, 180); 
    EXPECT_NEAR(sun_mins, 7*60+41, 10); 
}

TEST_F(SvalbardJan1Noon, EquationOfTime) {
    EXPECT_NEAR(equation_of_time(day_pos), -3.55, kMinuteTolerance);
}

TEST_F(SvalbardJan1Noon, Declination) {
    EXPECT_NEAR(declination(day_pos), -22.98, kAngleToleranceDeg);
}

TEST_F(SvalbardJan1Noon, Elevation) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(elevation(place.loc.lat_deg(), decl, hour_ang), -11.88, kAngleToleranceDeg); 
}

TEST_F(SvalbardJan1Noon, Azimuth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(azimuth(place.loc.lat_deg(), decl, hour_ang), 176.24, kAngleToleranceDeg); 
}

TEST_F(SvalbardJan1Noon, Lit) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    double az = azimuth(place.loc.lat_deg(), decl, hour_ang); 
    double elev = elevation(place.loc.lat_deg(), decl, hour_ang); 
    EXPECT_FALSE(is_lit(0, az, elev)); 
    EXPECT_FALSE(is_lit(90, az, elev)); 
    EXPECT_FALSE(is_lit(180, az, elev)); 
    EXPECT_FALSE(is_lit(270, az, elev)); 
}

TEST_F(SvalbardJan1Noon, LitMins) {
    double sun_mins = sun_mins_day(date.dt.day(), place.loc.lat_deg(), place.loc.lon_deg(), utc_offset, 180); 
    EXPECT_NEAR(sun_mins, 0, 0); 
}

TEST_F(SvalbardDec31Noon, EquationOfTime) {
    EXPECT_NEAR(equation_of_time(day_pos), -2.96, kMinuteTolerance);
}

TEST_F(SvalbardDec31Noon, Declination) {
    EXPECT_NEAR(declination(day_pos), -23.08, kAngleToleranceDeg);
}

TEST_F(SvalbardDec31Noon, Elevation) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(elevation(place.loc.lat_deg(), decl, hour_ang), -11.98, kAngleToleranceDeg); 
}

TEST_F(SvalbardDec31Noon, Azimuth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(azimuth(place.loc.lat_deg(), decl, hour_ang), 176.38, kAngleToleranceDeg); 
}

TEST_F(SvalbardSolstice, EquationOfTime) {
    EXPECT_NEAR(equation_of_time(day_pos), -1.79, kMinuteTolerance);
}

TEST_F(SvalbardSolstice, Declination) {
    EXPECT_NEAR(declination(day_pos), 23.44, kAngleToleranceDeg);
}

TEST_F(SvalbardSolstice, Elevation) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(elevation(place.loc.lat_deg(), decl, hour_ang), 31.38, kAngleToleranceDeg); 
}

TEST_F(SvalbardSolstice, Azimuth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(azimuth(place.loc.lat_deg(), decl, hour_ang), 133.39, kAngleToleranceDeg); 
}

TEST_F(SvalbardSolstice, Lit) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    double az = azimuth(place.loc.lat_deg(), decl, hour_ang); 
    double elev = elevation(place.loc.lat_deg(), decl, hour_ang);  
    EXPECT_TRUE(is_lit(180, az, elev)); 
}

TEST_F(SvalbardSolstice, LitMins) {
    double sun_mins = sun_mins_day(date.dt.day(), place.loc.lat_deg(), place.loc.lon_deg(), utc_offset, 180); 
    EXPECT_NEAR(sun_mins, 11*60 + 20, 20); 
}

TEST_F(SydneyJan1Noon, EquationOfTime) {
    EXPECT_NEAR(equation_of_time(day_pos), -3.35, kMinuteTolerance);
}

TEST_F(SydneyJan1Noon, Declination) {
    EXPECT_NEAR(declination(day_pos), -23.01, kAngleToleranceDeg);
}

TEST_F(SydneyJan1Noon, Elevation) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(elevation(place.loc.lat_deg(), decl, hour_ang), 73.21, kAngleToleranceDeg); 
}

TEST_F(SydneyJan1Noon, Azimuth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(azimuth(place.loc.lat_deg(), decl, hour_ang), 53.52, kAngleToleranceDeg); 
}

TEST_F(SydneyDec31Noon, EquationOfTime) {
    EXPECT_NEAR(equation_of_time(day_pos), -2.28, kMinuteTolerance);
}

TEST_F(SydneyDec31Noon, Declination) {
    EXPECT_NEAR(declination(day_pos), -23.18, kAngleToleranceDeg);
}

TEST_F(SydneyDec31Noon, Elevation) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(elevation(place.loc.lat_deg(), decl, hour_ang), 73.5, kAngleToleranceDeg); 
}

TEST_F(SydneyDec31Noon, Azimuth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(azimuth(place.loc.lat_deg(), decl, hour_ang), 53.34, kAngleToleranceDeg); 
}

TEST_F(SydneyDec31Noon, LitNorth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    double az = azimuth(place.loc.lat_deg(), decl, hour_ang); 
    double elev = elevation(place.loc.lat_deg(), decl, hour_ang);  
    EXPECT_TRUE(is_lit(0, az, elev)); 
}

TEST_F(SydneyDec31Noon, LitSouth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    double az = azimuth(place.loc.lat_deg(), decl, hour_ang); 
    double elev = elevation(place.loc.lat_deg(), decl, hour_ang);  
    EXPECT_FALSE(is_lit(180, az, elev)); 
}

TEST_F(SydneySolstice, EquationOfTime) {
    EXPECT_NEAR(equation_of_time(day_pos), -1.68, kMinuteTolerance);
}

TEST_F(SydneySolstice, Declination) {
    EXPECT_NEAR(declination(day_pos), 23.44, kAngleToleranceDeg);
}

TEST_F(SydneySolstice, Elevation) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(elevation(place.loc.lat_deg(), decl, hour_ang), -7.51, kAngleToleranceDeg); 
}

TEST_F(SydneySolstice, Azimuth) {
    double EoT = equation_of_time(day_pos); 
    double solar_time = true_solar_time(EoT, place.loc.lon_deg(), utc_offset, mins_since_midninght); 
    double hour_ang = hour_angle(solar_time); 
    double decl = declination(day_pos); 
    EXPECT_NEAR(azimuth(place.loc.lat_deg(), decl, hour_ang), 66.78, kAngleToleranceDeg); 
}

TEST(SunInFront, IsLit) {
    EXPECT_TRUE(is_lit(180, 180, 30)); 
}

TEST(SunBehind, IsLit) {
    EXPECT_FALSE(is_lit(90, 270, 30)); 
}

TEST(SunBelowHorizon, IsLit) {
    EXPECT_FALSE(is_lit(180, 180, -30)); 
}

TEST(SunOnHorizon, IsLit) {
    EXPECT_FALSE(is_lit(180, 180, 0)); 
}

TEST(SunAtNorth, IsLit) {
    EXPECT_TRUE(is_lit(0, 0, 30)); 
}

TEST(ExactlyParallel, IsLit) {
    EXPECT_FALSE(is_lit(90, 180, 30)); 
}