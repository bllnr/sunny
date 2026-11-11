#pragma once

#include <array>

struct Point2 {
    double x_east_m = 0.0;
    double y_north_m = 0.0; 
}; 

/// @brief Wall
class Wall {
public:
  Point2 start() const { return start_; }
  Point2 end() const { return end_; }
  double height_m() const { return height_m_; }
  double normal_azimuth_deg() const { return normal_azimuth_deg_; } 
private:
Wall(Point2 start, Point2 end, double height_m); 
    friend class Building; 

    Point2 start_;
    Point2 end_;
    double height_m_;
    double normal_azimuth_deg_;
}; 

/// @brief Building
/// @param width_m Width of building (metres, along north by default)
/// @param length_m Length of building (metres, along east by default)
/// @param height_m Height of building (metres)
/// @param rot_deg Rotation of the building in degrees (rotation clockwise from north)
class Building {
public:
  Building(double width_m, double length_m, double height_m, double rot_deg); 
  std::array<Wall, 4> walls() const; 
  double width_m()   const {return width_m_; }
  double length_m()  const {return length_m_; }
  double height_m()  const {return height_m_; }
  double rot_deg() const {return rot_deg_; }

  void set_width_m(double width_m);
  void set_length_m(double length_m);
  void set_height_m(double height_m);
  void set_rot_deg(double rot_deg);

private:
  static double checked_dimension(double value_m, const char* name);
  double width_m_;
  double length_m_;
  double height_m_;
  double rot_deg_;
}; 







