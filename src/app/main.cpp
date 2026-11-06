#include <iostream>

#include "solar/sun_position.hpp"
using namespace std;

int main() {
  
    double day_pos = day_position(1, 12, 0, false);
    cout << "Day position: " << day_pos << endl;

    day_pos = day_position(1, 0, 0, false);
    cout << "Day position: " << day_pos << endl;

    day_pos = day_position(183.5, 12, 0, false);
    cout << "Day position: " << day_pos << endl;

    day_pos = day_position(366, 12, 0, true);
    cout << "Day position: " << day_pos << endl;

    double eq_time = equation_of_time(day_pos);
    double decl = declination(day_pos);
    cout << "Equation of time: " << eq_time << endl;
    cout << "Declination: " << decl << endl;

    double clock_min = clock_minutes_since_midnight(12, 0);
    double solar_time = true_solar_time(eq_time, 0, 0, clock_min); 
    cout << "Clock minutes since midnight: " << clock_min << endl;
    cout << "Solar time: " << solar_time << endl;

    double hour_ang = hour_angle(solar_time);
    cout << "Hour angle: " << hour_ang << endl;

    double elev = elevation(solar_time, decl, hour_ang);
    double azim = azimuth(solar_time, decl, hour_ang);
    cout << "Elevation: " << elev << endl;
    cout << "Azimuth: " << azim << endl;

    double lit = is_lit(270, azim, elev);

    if (lit) {
        cout << "The sun is shining on the surface!" << endl; 
    }
    else {
        cout << "The sun is NOT shining on the surface!" << endl; 
    }

    return 0; 
}