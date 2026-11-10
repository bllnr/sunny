#include <cmath>
#include <stdexcept>
#include <gtest/gtest.h>

#include "model/model.hpp"
#include "core/angles.hpp"

using namespace std;

double AngleDiffDeg(double a, double b) {
    // Values on different sides of the 0/360 boundary may be close enough
    double d = std::fmod(std::fabs(a - b), 360.0);
    return std::min(d, 360.0 - d);
}

void ExpectPointNear(Point2 actual, Point2 expected, double tol = 1e-9) {
    // Helper to compare Point2 objects
    EXPECT_NEAR(actual.x_east_m, expected.x_east_m, tol); 
    EXPECT_NEAR(actual.y_north_m, expected.y_north_m, tol); 
}

TEST(AngleDiff, AngleDiffTest){
    EXPECT_NEAR(AngleDiffDeg(359.999999999999, 0.000000000001), 0, 1e-9); 
}

TEST(Building, RejectsNonPositiveDimensions) {
    EXPECT_THROW(Building(0, 20, 5, 0),  std::invalid_argument);
    EXPECT_THROW(Building(10, -1, 5, 0), std::invalid_argument);
    EXPECT_THROW(Building(10, 20, 0, 0), std::invalid_argument);
    EXPECT_THROW(Building(NAN, 20, 5, 0), std::invalid_argument);   // NaN trap
}

TEST(Building, AcceptsValidDimensions) {
    EXPECT_NO_THROW(Building(10, 20, 5, 0));
}

TEST(Building, RotationNormalisation360) {
    Building b = Building(10, 5, 3, 360);
    EXPECT_DOUBLE_EQ(b.rot_deg(), 0);
}

TEST(Building, RotationNormalisation361) {
    Building b = Building(10, 5, 3, 361);
    EXPECT_DOUBLE_EQ(b.rot_deg(), 1);
}

TEST(Building, RotationNormalisation10) {
    Building b = Building(10, 5, 3, 370);
    EXPECT_DOUBLE_EQ(b.rot_deg(), 10);
}

TEST(Building, RotationNormalisationNeg) {
    Building b = Building(10, 5, 3, -90);
    EXPECT_DOUBLE_EQ(b.rot_deg(), 270);
}

TEST(Building, RotationNormalisationLargeAndFloat) {
    Building b = Building(10, 5, 3, 720.5);
    EXPECT_DOUBLE_EQ(b.rot_deg(), 0.5);
}

TEST(Building, WallGeometryAtRot0Positions) {
    Building b = Building(10, 20, 5, 0);
    Wall wall0 = b.walls()[0];
    Point2 c0_start = wall0.start();
    Point2 c0_end = wall0.end();
    
    EXPECT_EQ(c0_start.x_east_m, -10);
    EXPECT_EQ(c0_start.y_north_m, -5);  
    EXPECT_EQ(c0_end.x_east_m, -10);
    EXPECT_EQ(c0_end.y_north_m, 5); 
    
    Wall wall1 = b.walls()[1];
    Point2 c1_start = wall1.start();
    Point2 c1_end = wall1.end();
    
    EXPECT_EQ(c1_start.x_east_m, -10);
    EXPECT_EQ(c1_start.y_north_m, 5);  
    EXPECT_EQ(c1_end.x_east_m, 10);
    EXPECT_EQ(c1_end.y_north_m, 5);

    Wall wall2 = b.walls()[2];
    Point2 c2_start = wall2.start();
    Point2 c2_end = wall2.end();
    
    EXPECT_EQ(c2_start.x_east_m, 10);
    EXPECT_EQ(c2_start.y_north_m, 5);  
    EXPECT_EQ(c2_end.x_east_m, 10);
    EXPECT_EQ(c2_end.y_north_m, -5);

    Wall wall3 = b.walls()[3];
    Point2 c3_start = wall3.start();
    Point2 c3_end = wall3.end();
    
    EXPECT_EQ(c3_start.x_east_m, 10);
    EXPECT_EQ(c3_start.y_north_m, -5);  
    EXPECT_EQ(c3_end.x_east_m, -10);
    EXPECT_EQ(c3_end.y_north_m, -5); 
}

TEST(Building, WallGeometryAtRot360Positions) {
    Building b = Building(10, 20, 5, 360);
    Wall wall0 = b.walls()[0];
    Point2 c0_start = wall0.start();
    Point2 c0_end = wall0.end();
    
    EXPECT_EQ(c0_start.x_east_m, -10);
    EXPECT_EQ(c0_start.y_north_m, -5);  
    EXPECT_EQ(c0_end.x_east_m, -10);
    EXPECT_EQ(c0_end.y_north_m, 5); 
    
    Wall wall1 = b.walls()[1];
    Point2 c1_start = wall1.start();
    Point2 c1_end = wall1.end();
    
    EXPECT_EQ(c1_start.x_east_m, -10);
    EXPECT_EQ(c1_start.y_north_m, 5);  
    EXPECT_EQ(c1_end.x_east_m, 10);
    EXPECT_EQ(c1_end.y_north_m, 5);

    Wall wall2 = b.walls()[2];
    Point2 c2_start = wall2.start();
    Point2 c2_end = wall2.end();
    
    EXPECT_EQ(c2_start.x_east_m, 10);
    EXPECT_EQ(c2_start.y_north_m, 5);  
    EXPECT_EQ(c2_end.x_east_m, 10);
    EXPECT_EQ(c2_end.y_north_m, -5);

    Wall wall3 = b.walls()[3];
    Point2 c3_start = wall3.start();
    Point2 c3_end = wall3.end();
    
    EXPECT_EQ(c3_start.x_east_m, 10);
    EXPECT_EQ(c3_start.y_north_m, -5);  
    EXPECT_EQ(c3_end.x_east_m, -10);
    EXPECT_EQ(c3_end.y_north_m, -5); 
}

TEST(Building, WallGeometryAtRot180Positions) {
    Building b = Building(10, 20, 5, 180);
    Wall wall0 = b.walls()[0];
    Point2 c0_start = wall0.start();
    Point2 c0_end = wall0.end();
    
    EXPECT_NEAR(c0_start.x_east_m, 10, 0.01);
    EXPECT_NEAR(c0_start.y_north_m, 5, 0.01);  
    EXPECT_NEAR(c0_end.x_east_m, 10, 0.01);
    EXPECT_NEAR(c0_end.y_north_m, -5, 0.01); 
    
    Wall wall1 = b.walls()[1];
    Point2 c1_start = wall1.start();
    Point2 c1_end = wall1.end();
    
    EXPECT_NEAR(c1_start.x_east_m, 10, 0.01);
    EXPECT_NEAR(c1_start.y_north_m, -5, 0.01);  
    EXPECT_NEAR(c1_end.x_east_m, -10, 0.01);
    EXPECT_NEAR(c1_end.y_north_m, -5, 0.01);

    Wall wall2 = b.walls()[2];
    Point2 c2_start = wall2.start();
    Point2 c2_end = wall2.end();
    
    EXPECT_NEAR(c2_start.x_east_m, -10, 0.01);
    EXPECT_NEAR(c2_start.y_north_m, -5, 0.01);  
    EXPECT_NEAR(c2_end.x_east_m, -10, 0.01);
    EXPECT_NEAR(c2_end.y_north_m, 5, 0.01);

    Wall wall3 = b.walls()[3];
    Point2 c3_start = wall3.start();
    Point2 c3_end = wall3.end();
    
    EXPECT_NEAR(c3_start.x_east_m, -10, 0.01);
    EXPECT_NEAR(c3_start.y_north_m, 5, 0.01);  
    EXPECT_NEAR(c3_end.x_east_m, 10, 0.01);
    EXPECT_NEAR(c3_end.y_north_m, 5, 0.01); 
}

TEST(Building, WallGeometryAtRot90Positions) {
    Building b = Building(10, 20, 5, 90);
    Wall wall0 = b.walls()[0];
    Point2 c0_start = wall0.start();
    Point2 c0_end = wall0.end();
    
    EXPECT_NEAR(c0_start.x_east_m, -5, 0.01);
    EXPECT_NEAR(c0_start.y_north_m, 10, 0.01);  
    EXPECT_NEAR(c0_end.x_east_m, 5, 0.01);
    EXPECT_NEAR(c0_end.y_north_m, 10, 0.01); 
    
    Wall wall1 = b.walls()[1];
    Point2 c1_start = wall1.start();
    Point2 c1_end = wall1.end();
    
    EXPECT_NEAR(c1_start.x_east_m, 5, 0.01);
    EXPECT_NEAR(c1_start.y_north_m, 10, 0.01);  
    EXPECT_NEAR(c1_end.x_east_m, 5, 0.01);
    EXPECT_NEAR(c1_end.y_north_m, -10, 0.01);

    Wall wall2 = b.walls()[2];
    Point2 c2_start = wall2.start();
    Point2 c2_end = wall2.end();
    
    EXPECT_NEAR(c2_start.x_east_m, 5, 0.01);
    EXPECT_NEAR(c2_start.y_north_m, -10, 0.01);  
    EXPECT_NEAR(c2_end.x_east_m, -5, 0.01);
    EXPECT_NEAR(c2_end.y_north_m, -10, 0.01);

    Wall wall3 = b.walls()[3];
    Point2 c3_start = wall3.start();
    Point2 c3_end = wall3.end();
    
    EXPECT_NEAR(c3_start.x_east_m, -5, 0.01);
    EXPECT_NEAR(c3_start.y_north_m, -10, 0.01);  
    EXPECT_NEAR(c3_end.x_east_m, -5, 0.01);
    EXPECT_NEAR(c3_end.y_north_m, 10, 0.01); 
}

TEST(Building, WallGeometryAtRot0Normals) {
    Building b = Building(10, 20, 5, 0);
    
    Wall wall0 = b.walls()[0];
    double azimuth0 = wall0.normal_azimuth_deg(); 
    EXPECT_EQ(azimuth0, 270); 
    
    Wall wall1 = b.walls()[1];
    double azimuth1 = wall1.normal_azimuth_deg();
    EXPECT_EQ(azimuth1, 0); 

    Wall wall2 = b.walls()[2];
    double azimuth2 = wall2.normal_azimuth_deg(); 
    EXPECT_EQ(azimuth2, 90); 

    Wall wall3 = b.walls()[3];
    double azimuth3 = wall3.normal_azimuth_deg(); 
    EXPECT_EQ(azimuth3, 180); 
}

TEST(Building, WallGeometryAtRot0Height) {
    Building b = Building(10, 20, 5, 0);
    
    Wall wall0 = b.walls()[0];
    EXPECT_EQ(wall0.height_m(), 5); 
    
    Wall wall1 = b.walls()[1];
    EXPECT_EQ(wall1.height_m(), 5); 

    Wall wall2 = b.walls()[2];
    EXPECT_EQ(wall2.height_m(), 5); 

    Wall wall3 = b.walls()[3];
    EXPECT_EQ(wall3.height_m(), 5);  
}

class BuildingRotation : public ::testing::TestWithParam<double> {};

TEST_P(BuildingRotation, NormalRelationToRotation) {
    // Normal = Rotation + k*{270, 0, 90, 180}, where k is the number of the wall
    double rotation = GetParam();
    
    for (int k : {0, 1, 2, 3}) {
        Building b = Building(10, 20, 5, rotation);
        array<int, 4> choices = {270, 0, 90, 180} ;
        Wall wall = b.walls()[k];
        double n = normalize_deg(rotation + choices[k]); 
        EXPECT_NEAR(AngleDiffDeg(wall.normal_azimuth_deg(), n), 0, 1e-9); 
    }
}

TEST_P(BuildingRotation, WallsFormClosedLoop) {
    double rotation = GetParam();

    for (int k : {0, 1, 2, 3}) {
        Building b = Building(10, 20, 5, rotation);
        Wall first_wall = b.walls()[k];
        
        int next_index =  k == 3 ? 0 : k + 1; 
        Wall second_wall = b.walls()[next_index];
        
        SCOPED_TRACE("wall " + std::to_string(k));
        ExpectPointNear(first_wall.end(), second_wall.start()); 
    }
}

namespace {
    double distance(Point2 p, Point2 q) {
        double first = (p.x_east_m - q.x_east_m); 
        double second = (p.y_north_m - q.y_north_m); 
        
        double d = ::sqrt(first*first + second*second); 
        return d; 
    }
}

TEST_P(BuildingRotation, WallsLengthsPreserved) {
    double rotation = GetParam();

    Building a = Building(10, 20, 5, 0);
    Building b = Building(10, 20, 5, rotation);

    for (int k : {0, 1, 2, 3}) {
        
        Wall aw = a.walls()[k];
        Wall bw = b.walls()[k];

        // Length of both walls are equal (not stretched)
        EXPECT_NEAR(distance(aw.start(), aw.end()), distance(bw.start(), bw.end()), 1e-9); 
    }
}

TEST_P(BuildingRotation, CenteredOnOrigin) {
    double rotation = GetParam();

    Building b = Building(10, 20, 5, rotation);

    Point2 c0 = b.walls()[0].start();
    Point2 c1 = b.walls()[1].start();
    Point2 c2 = b.walls()[2].start();
    Point2 c3 = b.walls()[3].start();
    double x_avg = (c0.x_east_m + c1.x_east_m + c2.x_east_m + c3.x_east_m) / 4.0; 
    double y_avg = (c0.y_north_m + c1.y_north_m + c2.y_north_m + c3.y_north_m) / 4.0;  
    EXPECT_NEAR(x_avg, 0, 1e-9); 
    EXPECT_NEAR(y_avg, 0, 1e-9); 
}

TEST_P(BuildingRotation, NeighbouringNormals90Deg) {
    double rotation = GetParam();

    for (int k : {0, 1, 2, 3}) {
        Building b = Building(10, 20, 5, rotation);
        Wall first_wall = b.walls()[k];
        
        int next_index =  k == 3 ? 0 : k + 1; 
        Wall second_wall = b.walls()[next_index];
        
        double first_az = normalize_deg(first_wall.normal_azimuth_deg() + 90); 
        EXPECT_NEAR(first_az, second_wall.normal_azimuth_deg(), 1e-9); 
    }
}

TEST_P(BuildingRotation, OppositeNormals180Deg) {
    double rotation = GetParam();

    for (int k : {0, 1}) {
        Building b = Building(10, 20, 5, rotation);
        Wall first_wall = b.walls()[k];
        Wall second_wall = b.walls()[k + 2];
        
        double first_az = normalize_deg(first_wall.normal_azimuth_deg() + 180); 
        EXPECT_NEAR(first_az, second_wall.normal_azimuth_deg(), 1e-9); 
    }
}

INSTANTIATE_TEST_SUITE_P(Rotations, BuildingRotation,
    ::testing::Values(0.0, 30.0, 90.0, 180.0, 359.5, -45.0, 720.0));