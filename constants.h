#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
    Define all specific constants, like
    number of robots, number of environment objects, etc.:
*/

#define PERFORM_ACTION_FREQUENCY    30      //how many times robot performs his actions per second
#define REAL_PIXEL_SIZE             60      //number of cells in real pixel
#define ENV_OBJECTS                 255     //number of environment objects
#define CUSTOM_PARAMETERS_QUANTITY  16      //number of custom robot params

enum Intersection { Allowed, Denied, AllowedForSameColor };

/* Define some non-crossplatform math constants: */

#define PI 3.14159265358979323846

#endif // CONSTANTS_H

/* Limit line length to 100 characters; highlight 99th column
 * vim: set textwidth=100 colorcolumn=-1:
 */
