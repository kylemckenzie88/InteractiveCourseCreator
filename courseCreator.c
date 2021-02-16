#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "courseCreator.h"
#include <assert.h>
#define COURSE_FILE "courses.dat" // Course file name
#define MAX_LENGTH 100            // Max length of buffer

int existsCheck(unsigned long courseNumber, FILE *pCourseFile)
{
    // Open file for read
    pCourseFile = fopen(COURSE_FILE, "rb");
    
    // If file does not exist create it
    // Return 0. no courses exist
    if(pCourseFile == NULL)
    {
        pCourseFile = fopen(COURSE_FILE, "wb+");
        fclose(pCourseFile);
        return 0;
    }

    int seekRV, readRV = 0; // Variables to store return values of fseek/fread
    COURSE course;
    
    //Locate course address and read data
    seekRV = fseek(pCourseFile, (courseNumber * sizeof(COURSE)), SEEK_SET);
    readRV = fread(&course, sizeof(COURSE), 1L, pCourseFile);
    
    //Check if course exists. Return 0 if none. 1 if exists
    if(course.courseName[0] == 0 || course.courseHours == 0)
    {
        fclose(pCourseFile);
        return 0;
    }
    fclose(pCourseFile);
    return 1;
}   

void createCourse(unsigned long courseNum, char *buffer, FILE *pCourseFile)
{
    // Open file. Get data from user to create course
    pCourseFile = fopen(COURSE_FILE, "rb+");
    int seekRV, writeRV = 0; // Variables to store return values of fseek/fread
    COURSE course;
    
    fgets(buffer, MAX_LENGTH, stdin);
    sscanf(buffer, "%100[^\n]", course.courseName);
    
    fgets(buffer, MAX_LENGTH, stdin);
    sscanf(buffer, "%s", course.courseSched);

    fgets(buffer, MAX_LENGTH, stdin);
    sscanf(buffer, "%u", &course.courseHours);
    
    fgets(buffer, MAX_LENGTH, stdin);
    sscanf(buffer, "%u", &course.courseSize);
    
    seekRV = fseek(pCourseFile, courseNum * sizeof(COURSE), SEEK_SET);
    assert(seekRV == 0);
    
    writeRV = fwrite(&course, sizeof(COURSE), 1L, pCourseFile);
    assert(writeRV == 1);
    fclose(pCourseFile);
    return;
}

void readCourseData(unsigned long courseNumber, FILE *pCourseFile)
{
    pCourseFile = fopen(COURSE_FILE, "rb");
    int seekRV, readRV = 0; // Variables to store return values of fseek/fread
    COURSE course;
    
    // Seek to course location in file. Place data in course variable
    seekRV = fseek(pCourseFile, (courseNumber * sizeof(COURSE)), SEEK_SET);
    assert(seekRV == 0);
    readRV = fread(&course, sizeof(COURSE), 1L, pCourseFile);
    
    //If read successful print course data
    if(readRV == 1)
    {
        printf("Course number: %lu\nCourse name: %s\nScheduled days: %s\nCredit hours: %u\nEnrolled students: %u\n"
        , courseNumber
        , course.courseName
        , course.courseSched
        , course.courseHours
        , course.courseSize);
    }
    fclose(pCourseFile);
    return;
}

void updateCourseData(unsigned long courseNumber, char *buffer, FILE *pCourseFile)
{
    pCourseFile = fopen(COURSE_FILE, "rb+");
    int seekRV, readRV, writeRV = 0; // Variables to store return values or fseek/fread/fwrite
    COURSE course;
    
    // Seek to course location in file. Read data into course variable.
    seekRV = fseek(pCourseFile, (courseNumber * sizeof(COURSE)), SEEK_SET);
    assert(seekRV == 0); 
    readRV = fread(&course, sizeof(COURSE), 1L, pCourseFile);
    
    // Get new data to update course from user. Leave origional values
    // If no data entered by user.
    if(fgets(buffer, MAX_LENGTH, stdin) != NULL)
    {
        sscanf(buffer, "%100[^\n]", course.courseName);
    }

    if(fgets(buffer, MAX_LENGTH, stdin) != NULL)
    {
        sscanf(buffer, "%s", course.courseSched);    
    }
    
    if(fgets(buffer, MAX_LENGTH, stdin) != NULL)
    {
        sscanf(buffer, "%u", &course.courseHours);
    }
    
    if(fgets(buffer, MAX_LENGTH, stdin) != NULL)
    {
        sscanf(buffer, "%u", &course.courseSize);
    }
    
    // Seek back to original course address and write data.
    seekRV = fseek(pCourseFile, (courseNumber * sizeof(COURSE)), SEEK_SET);
    assert(seekRV == 0);
    writeRV = fwrite(&course, sizeof(COURSE), 1L, pCourseFile);
    assert(writeRV == 1);
    
    fclose(pCourseFile);
    return;
}

void deleteCourse(unsigned long courseNumber, char *buffer, FILE *pCourseFile)
{
    int seekRV, writeRV = 0; // Variables to store return values of fseek/fwrite
    COURSE course;
    
    // Seek to course location in file and set memory the size of a course
    // to 0.
    pCourseFile = fopen(COURSE_FILE, "wb");
    seekRV = fseek(pCourseFile, (courseNumber * sizeof(COURSE)), SEEK_SET);
    assert(seekRV == 0);
    memset(&course, 0, sizeof(COURSE));
    writeRV = fwrite(&course, sizeof(COURSE), 1L, pCourseFile);
    assert(writeRV ==1);
    printf("%lu was successfully deleted\n", courseNumber); // Success message
    fclose(pCourseFile);
    return;
}

int main()
{
    FILE *pCourseFile; // Course file pointer
    char buffer[MAX_LENGTH]; // Buffer for user input
    
    // Infinte loop.
    while(1)
    {
        // Menu
        printf("%s%s%s%s%s"
            , "Enter one of the following actions or press CTRL-D to exit\n"
            , "C - create a new course record\n"
            , "R - read an existing course record\n"
            , "U - update an existing course record\n"
            , "D - delete an existing course record\n");
        
        //Terminate loop when user enters CTRL-D
        if(fgets(buffer, MAX_LENGTH, stdin) ==  NULL)
        {
                return 0;
        }

        switch(toupper(buffer[0]))
        {
            case 'C': // Create a course
                
                fgets(buffer, MAX_LENGTH, stdin);
                
                // Check if course exists already. Otherwise create it.
                if((existsCheck(strtoul(buffer, 0, 0), pCourseFile)) == 1)
                {
                    printf("ERROR: course already exists!\n");
                }
                else
                {
                    createCourse(strtoul(buffer, 0, 0), buffer, pCourseFile);
                }
                
                break;
            
            case 'R': // Read data for an existing course.
                
                fgets(buffer, MAX_LENGTH, stdin);
                
                // If course exists read the data. Otherwise display error message.
                if((existsCheck(strtoul(buffer, 0, 0), pCourseFile)) == 1)
                {
                    readCourseData(strtoul(buffer, 0, 0), pCourseFile);
                }
                else
                {
                    printf("ERROR: course not found!\n");    
                }

                break;
            
            case 'U': // Update data for an existing course.
                
                fgets(buffer, MAX_LENGTH, stdin);
                
                // If course exists update it. Otherwise display error message.
                if((existsCheck(strtoul(buffer, 0, 0), pCourseFile)) == 1)
                {
                    updateCourseData(strtoul(buffer, 0, 0), buffer, pCourseFile);    
                }
                else
                {
                    printf("ERROR: course not found!\n");    
                }
                
                break;
            
            case 'D': // Delete an existing course.
                
                fgets(buffer, MAX_LENGTH, stdin);
                
                // If course exists delete it. Otherwise display error message
                if((existsCheck(strtoul(buffer, 0, 0), pCourseFile)) == 1)
                {
                    deleteCourse(strtoul(buffer, 0, 0), buffer, pCourseFile);
                }
                else
                {
                    printf("ERROR: course not found!\n");    
                }

                break;

            default: // Invalid entry by user. Print error message
                
                printf("ERROR: invalid option\n");
                
                break;
        }
    }
    
    return 0;
}
