//INTRODUCTION-------------------------------------------------------------------------------------------------------------------------------------------------
/*
This is the header file for robotConfig.cpp. For more information about header files, check the PDF.
*/

//STRUCTURE EXPLANATION----------------------------------------------------------------------------------------------------------------------------------------
/*
The format for initializing a part in the robotConfig header file is:

extern part_type part_name;

The examples in robotConfig.cpp are already initialized for you.
*/

//CODE---------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once //This prevents the header file from being duplicated when included multiple times. For more information, refer to the PDF file. This should be supported
             //by any modern compiler.
#include "vex.h" //Needed for data types. No need to change this.

using namespace vex; //All functions use the vex namespace (basically a version of the function). We use this here for simplicity; in larger non-VEX projects,
                     //avoiding using namespace in headers is recommended. No need to change this.

extern brain Brain; //Example brain initialization.

extern controller Controller1; //Example controller initialization.

extern motor LeftMotor1; //Example motor initialization.
extern motor LeftMotor2; //Example motor initialization.
extern motor LeftMotor3; //Example motor initialization.
extern motor RightMotor1; //Example motor initialization.
extern motor RightMotor2; //Example motor initialization.
extern motor RightMotor3; //Example motor initialization.
extern motor FrontMotor; //Example motor initialization.
extern motor BackMotor; //Example motor initialization.

extern inertial Inertial; //Example inertial sensor initialization.
extern distance DistanceSensor; //Example distance sensor initialization.

extern digital_out Pistons; //Example piston initialization.
extern digital_out Pistons2; //Example piston initialization.

extern motor_group LeftMotor_group; //Example motor group initialization.
extern motor_group RightMotor_group; //Example motor group initialization.

//DO NOT CHANGE THIS
void vexcodeInit(void);

//TROUBLESHOOTING----------------------------------------------------------------------------------------------------------------------------------------------
/*
Q: When I compile, an error occurs citing this file?
A: Make sure your syntax and port numbers are correct, all arguments are filled in, and the names match in robotConfig.cpp and robotConfig.h. If there
   is still an issue, ask on the GitHub Issues page.

Q: My problem is not in the troubleshooting section?
A: Go to the GitHub repository, click on the "Issues" tab, and submit a new issue describing your problem. I will respond.
*/
