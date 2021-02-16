#include <stdio.h>

typedef struct
{
    char courseName[64];        // Stores course name
    char courseSched[4];        // Stores course schedule 'MWF' or 'TR' or 'TH'
    unsigned int courseHours;   // Stores course credit hours
    unsigned int courseSize;    // Stores course enrollment size
} COURSE;
