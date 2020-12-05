/**
 * layouts.h from a base code provided by COMP20003, 2020.
 * Implements layouts used for peg solitaire.
 * Retrieved from https://canvas.lms.unimelb.edu.au/courses/87167/files/5465229/download?wrap=1
*/

#ifndef __LAYOUTS__
#define __LAYOUTS__

#define NUM_LAYOUTS 9
int8_t configuration[NUM_LAYOUTS][SIZE][SIZE*2] = {{
            "                  ",
            "                  ",
            "                  ",
            "      o o . o     ",
            "                  ",
            "                  ",
            "                  ",
            "                  ",
            "                  "
        },{
            "                  ",
            "                  ",
            "                  ",
            "      o o .       ",
            "        o         ",
            "        o         ",
            "                  ",
            "                  ",
            "                  "
        },{
            "                  ",
            "                  ",
            "          o       ",
            "      . o o       ",
            "        o o o     ",
            "        o         ",
            "                  ",
            "                  ",
            "                  "
        },{
            "o o o . . .       ",
            "o o o . . .       ",
            ". o o . . .       ",
            "      o o o       ",
            "      o o o       ",
            "      o o o       ",
            "                  ",
            "                  ",
            "                  "
        },{
            "                  ",
            "      o o o       ",
            "      o o o       ",
            "  o o o o o o o   ",
            "  o o o . o o o   ",
            "  o o o o o o o   ",
            "      o o o       ",
            "      o o o       ",
            "                  "
        },{
            "                  ",
            "      o o o       ",
            "    o o o o o     ",
            "  o o o . o o o   ",
            "  o o o o o o o   ",
            "  o o o o o o o   ",
            "    o o o o o     ",
            "      o o o       ",
            "                  "
        },{
            "      o o o       ",
            "      o o o       ",
            "      o o o       ",
            "o o o o o o o o o ",
            "o o o o . o o o o ",
            "o o o o o o o o o ",
            "      o o o       ",
            "      o o o       ",
            "      o o o       "
        },{
            "      o o o       ",
            "      o o o       ",
            "      o o o       ",
            "  o o o o o o o o ",
            "  o o o . o o o o ",
            "  o o o o o o o o ",
            "      o o o       ",
            "      o o o       ",
            "                  "
        },{
            "        o         ",
            "      o o o       ",
            "    o o o o o     ",
            "  o o o o o o o   ",
            "o o o o . o o o o ",
            "  o o o o o o o   ",
            "    o o o o o     ",
            "      o o o       ",
            "        o         "
        }};

    #endif