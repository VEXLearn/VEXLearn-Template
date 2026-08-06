//INTRODUCTION-------------------------------------------------------------------------------------------------------------------------------------------------
/*
The main file is where everything is done. The main function is executed when you run your program.
*/

//STRUCTURE EXPLANATION----------------------------------------------------------------------------------------------------------------------------------------
/*
The pre-auton function is where sensors are calibrated. The autonomous function is where autonomous routines are called. The user-control function is
where the drive function is called. The main function then calls these functions.
*/

//CODE---------------------------------------------------------------------------------------------------------------------------------------------------------
#include "drive.h" //drive.h contains the drive() function, which is integral to the user-control function.
#include "robotConfig.h" //robotConfig.h contains the sensors, which are called in the pre_auton section for calibration.
#include "auto.h" //auto.h contains the autonomous routines, which are integral to the autonomous function.
#include "display.h" //display.h contains the display functions, which are integral to the brain and controller displays.

using namespace vex; //All functions use the vex namespace (basically a version of the function). No need to change this.

competition Competition; //Creates a competition object. Do not change and do not delete.

void pre_auton(void) { //The pre_auton function is where your sensors are calibrated.
  vexcodeInit(); //Does nothing because the function is empty in robotConfig.cpp. If you add something, it is initialized or activated here.
  Inertial.calibrate(); //Calibrates the inertial sensor.
  while(Inertial.isCalibrating()) {wait(25, msec);} //Waits while the inertial sensor is calibrating.
  Pistons.set(false); //Set the state of the piston. This is just an example.
  wait(0.1, sec); //Air takes time to move, so this ensures that the previous command has been executed.
  Pistons2.set(false); //Set the state of the piston. This is just an example.
}

void autonomous(void) { //The autonomous function calls the autonomous functions. Only one can be run at a time, so comment the rest out. You can load
                        //different autonomous functions in different slots.
  BlueRight();
  // BlueLeft();
  // RedRight();
  // RedLeft();
  // Skills();
}

void usercontrol(void) { //The user-control function controls what the robot does during the driver phase of matches and driver skills.
  thread screenThread(BrainRender); //Runs the brain-rendering function in a thread so other things can execute simultaneously.
  thread screenThread2(ControllerRender); //Runs the controller-rendering function in a thread so other things can execute simultaneously.

  drive(); //Calls the drive function for driver control.
}

int main() { //The main function is necessary for the whole program to work. Do not delete it.
  Competition.autonomous(autonomous); //A function of the competition object. Do not change.
  Competition.drivercontrol(usercontrol); //A function of the competition object. Do not change.

  pre_auton(); //Calls the pre_auton function which calibrates the sensor and sets the robot's default state.

  while(true) {wait(100, msec);} //Infinite loop that keeps the program running.
}

//TROUBLESHOOTING----------------------------------------------------------------------------------------------------------------------------------------------
/*
Q: When I compile, an error occurs citing this file?
A: Make sure the functions you call exist in their respective header and cpp files. If there is still an issue, ask on the GitHub Issues page.

Q: My problem is not in the troubleshooting section?
A: Go to the GitHub repository, click on the "Issues" tab, and submit a new issue describing your problem. I will respond.
*/
