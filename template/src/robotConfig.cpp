//INTRODUCTION-------------------------------------------------------------------------------------------------------------------------------------------------
/*
MORE SPECIFIC EXPLANATIONS AND TIPS ARE IN THE PDF FILE!
The robot configuration file is where you initialize the parts of your robot. The parts you have to initialize include the brain, controller, motors, and any
sensors you're using. It is best to write this file with your builder so you can ask them any questions you have.
*/

//STRUCTURE EXPLANATION----------------------------------------------------------------------------------------------------------------------------------------
/*
Your robotConfig.cpp file will start by including vex.h because it includes the data types for the brain, controller, motors, etc. An include follows this format:

#include whatever_file

In this file, it would be:

#include "vex.h"

Brain initialization follows this format:

brain Brain_name;

Controller initialization follows this format:

controller Controller_name = controller(primary);

This indicates that the controller is the primary controller.

12-watt motor initialization follows this format:

motor Motor_name = motor(port_number, gear_ratio, direction);

For port_number, use PORT followed by the port number to which the motor is connected on the brain. For example, port 1 is PORT1, and port 15 is PORT15. For
direction, false is the default, where clockwise rotation is treated as forward (positive direction); true means counterclockwise rotation is treated as
forward (positive direction). For gear_ratio, red is 36:1 (ratio36_1), green is 18:1 (ratio18_1), and blue is 6:1 (ratio6_1).

6-watt motor initialization follows this format:

motor Motor_name = motor(port_number, direction);

Inertial sensor initialization follows this format:

inertial Inertial_name = inertial(port_number);

Distance sensor initialization follows this format:

distance DistanceSensor_name = distance(port_number);

Piston initialization follows this format:

digital_out Piston_name = digital_out(threeWirePort_letter);

For a three-wire port, use Brain.ThreeWirePort.letter. For example, port A is Brain.ThreeWirePort.A, and port D is Brain.ThreeWirePort.D.

Motor group initialization follows this format:

motor_group motorGroup_name(motor, motor, motor);

There is no limit to how many motors can be in a motor group. A motor group allows you to control multiple motors that do the same thing.
*/

//CODE---------------------------------------------------------------------------------------------------------------------------------------------------------
#include "vex.h" //Needed for data types. No need to change this.

using namespace vex; //All functions use the vex namespace (basically a version of the function). No need to change this.

brain Brain; //Example brain initialization.

controller Controller1 = controller(primary); //Example controller initialization.

motor LeftMotor1 = motor(PORT1, ratio6_1, true); //Example 12-watt motor initialization.
motor LeftMotor2 = motor(PORT2, ratio6_1, true); //Example 12-watt motor initialization.
motor LeftMotor3 = motor(PORT3, ratio6_1, true); //Example 12-watt motor initialization.
motor RightMotor1 = motor(PORT4, ratio6_1, false); //Example 12-watt motor initialization.
motor RightMotor2 = motor(PORT5, ratio6_1, false); //Example 12-watt motor initialization.
motor RightMotor3 = motor(PORT6, ratio6_1, false); //Example 12-watt motor initialization.
motor FrontMotor = motor(PORT7, false); //Example 6-watt motor initialization.
motor BackMotor = motor(PORT8, false); //Example 6-watt motor initialization.

inertial Inertial = inertial(PORT9); //Example inertial sensor initialization.
distance DistanceSensor = distance(PORT10); //Example distance sensor initialization.
digital_out Pistons = digital_out(Brain.ThreeWirePort.D); //Example piston initialization.
digital_out Pistons2 = digital_out(Brain.ThreeWirePort.C); //Example piston initialization.

motor_group LeftMotor_group(LeftMotor1, LeftMotor2, LeftMotor3); //Example motor group initialization.
motor_group RightMotor_group(RightMotor1, RightMotor2, RightMotor3); //Example motor group initialization.

//DO NOT CHANGE THIS PART
void vexcodeInit(void) {
  // Nothing to initialize
}

//TROUBLESHOOTING----------------------------------------------------------------------------------------------------------------------------------------------
/*
Q: When I compile, an error occurs citing this file?
A: Make sure your syntax and port numbers are correct, all arguments are filled in, and the names match in robotConfig.cpp and robotConfig.h. If there
   is still an issue, ask on the GitHub Issues page.

Q: What parts are required to be initialized?
A: Generally, parts used in functions, such as motors, inertial sensors, distance sensors, pistons, the brain, and the controller, must be initialized as
   objects before their functions can be used.

Q: A part I want to initialize is not in the structure explanation section?
A: Go to the Smart Port Devices section at https://api.vex.com/v5/home/cpp/index.html, then click the sensor you are trying to initialize.

Q: Radio initialization is not listed in the structure explanation or on the VEX API website?
A: Radio initialization is handled automatically by the system, so you do not have to initialize it manually. Most advanced sensors have special initialization
   procedures, but most beginners do not use them. For more information about those sensors, go to https://api.vex.com/v5/home/cpp/Smart_Port_Devices/index.html.

Q: My problem is not in the troubleshooting section?
A: Go to the GitHub repository, click on the "Issues" tab, and submit a new issue describing your problem. I will respond.
*/
