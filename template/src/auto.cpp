//INTRODUCTION-------------------------------------------------------------------------------------------------------------------------------------------------
/*
MORE SPECIFIC EXPLANATIONS AND TIPS ARE IN THE PDF FILE!
An autonomous function is used during a competition's 15-second autonomous period or the skills challenge. Autonomous code makes your robot follow a set path
that you wrote in the function. If you have ever used Scratch, you will know that it has "change x by" and "change y by" blocks. Calling movement functions
inside an autonomous function does the same thing.

If you plan to write skills code, you should use the JAR Template or the LemLib template. These templates have ready-to-use odometry functions. Odometry is
very important in skills because you will cover much longer distances than during the 15-second autonomous period. My advice is to switch to the JAR Template
or LemLib for skills because my template focuses mostly on teaching and simpler tasks.
*/

//STRUCTURE EXPLANATION----------------------------------------------------------------------------------------------------------------------------------------
/*
Your auto.cpp file will start by including any header files that contain movement functions. If you haven't changed my file structure, you will include
these header files. An include follows this format:

#include whatever_file

In this file, it would be:

#include "vex.h"
#include "robotConfig.h"
#include "movement.h"

Function declarations follow the normal declaration format.

return_type function_name(arguments) {
    contents
}

Generally, an autonomous function declaration looks like this:

void blueRight() {
    contents
}

The return type is void because autonomous functions generally don't return anything. You can pass arguments if you need them in your autonomous routine,
but generally, you don't need any. You should also name your autonomous function after what it does. For example, the blueRight name indicates that the
function is the blueRight autonomous routine.
*/

//CODE---------------------------------------------------------------------------------------------------------------------------------------------------------
#include "vex.h" //This allows the wait function and other VEX functions.
#include "robotConfig.h" //This allows motors and pistons to be controlled.
#include "movement.h" //This allows you to call functions from movement.cpp. (Make sure you added the function name to the header file.)

using namespace vex; //All functions use the vex namespace (basically a version of the function). No need to change this.

//Here is what the 15-second autonomous functions would look like.
void BlueRight() {
   //movement code
}

void BlueLeft() {
   //movement code
}

void RedRight() {
   //movement code
}

void RedLeft() {
   //movement code
}
//Generally, a VEX V5 field is symmetrical, meaning you could use the same movement code for BlueRight and RedRight and the same movement code for BlueLeft
//and RedLeft.
//Skills functions also follow the same declaration format.
void Skills() {
    //movement code
}
//Of course, you are free to name your autonomous functions anything; these are just examples. You can also have different autonomous functions for the
//same side to cover a different route.

//Here is some example autonomous code. This is only a demonstration of function calls and doesn't achieve any specific goal when run. This function is not
//in the header file; it's here for demonstration only.
void example() {
   move(100, 0, 8, 6); //Moves 100 centimeters with a maximum voltage of 8 and an exit voltage of 6.
   turn(15, 8); //Turns to 15 degrees relative to the origin with a maximum voltage of 8.
   move(100, 0, 8); //Moves 100 centimeters with a maximum voltage of 8.
   FrontMotor.spin(forward, 12, volt); //Spins the front motor at 12 volts.
   wait(1, sec); //Waits.
   FrontMotor.stop(coast); //Stops the front motor using coast.
   Pistons.set(true); //Fires the pistons.
   move(-100, 0, 8); //Moves 100 centimeters backward with a maximum voltage of 8.
   turn(180, 8); //Turns to a heading of 180 degrees relative to the origin with a maximum voltage of 8.
   move(10, 0, 8); //Moves 10 centimeters with a maximum voltage of 8.
   BackMotor.spin(forward, 12, volt); //Spins the back motor at 12 volts.
}

//TROUBLESHOOTING----------------------------------------------------------------------------------------------------------------------------------------------
/*
Q: I changed my autonomous code but the robot did not change?
A: If your change was very small, you might not be able to see it. Check by making the robot move a massive distance (six meters, for example). If you still
   don't see the robot move that distance, make sure you are running the correct autonomous function in main.cpp.

Q: I can't find my autonomous function in main?
A: Make sure you have added your autonomous function to the header file. Make sure the header file is included in main.cpp. If it isn't, add
   #include "auto.h" to main.cpp. Make sure the function name is spelled correctly and is identical in auto.cpp, auto.h, and main.cpp.

Q: My autonomous code was working perfectly last time I tuned it but now it is a mess?
A: Check whether you accidentally changed anything. If you did not, this is quite normal. Run-to-run differences are common, and as long as the robot doesn't
   deviate too much, you can correct them by tuning the turning angle and distance. Tuning is very common; every team does it, including between matches
   during competitions.

Q: How do I run different routes in between matches without having to load the file onto the VEX Brain every time?
A: Load the code into a different slot on the Brain. For example, slot 1 could be BlueRight (changed in main.cpp), slot 2 could be BlueLeft, etc.

Q: My robot's starting alignment is not 0 degrees, but the robot's alignment is initialized based on its starting position?
A: Add Inertial.setHeading(your starting heading/alignment) at the start of your autonomous function.

Q: My problem is not in the troubleshooting section?
A: Go to the GitHub repository, click on the "Issues" tab, and submit a new issue describing your problem. I will respond.
*/
