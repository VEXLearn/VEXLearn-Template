//INTRODUCTION-------------------------------------------------------------------------------------------------------------------------------------------------
/*
The drive.cpp file declares the drive function, which is called in the user-control function in main.cpp. The drive function contains what the robot does
during driver control. This is where you will write the code for your driver control, such as how the robot moves and what the buttons do. You can also have
other functions in this file if you want, but the drive function is the main one. You can also have different drive functions for different control schemes,
but make sure to call the correct one in main.cpp.
*/

//STRUCTURE EXPLANATION----------------------------------------------------------------------------------------------------------------------------------------
/*
The drive function is a void function, meaning it does not return anything. The drive function contains an infinite loop, which allows the robot to be
controlled indefinitely during driver control. Inside the loop, you can read the controller's joystick and button values to control the robot's movement and
actions. You can also use timers to create delays or to measure how long certain actions take. Since the drive function uses motor and sensor values, you will
need to include the robotConfig.h header file at the top of this file. You can also include other header files that you have written.
*/

//CODE---------------------------------------------------------------------------------------------------------------------------------------------------------
#include "robotConfig.h" //Includes the robotConfig header file so you can use motor functions.
#include "vex.h" //This isn't strictly necessary because robotConfig.h already includes vex.h. It is here in case #include "vex.h" is ever changed in
                 //robotConfig.h.

void drive() { //Declares the drive function.
    timer Timer; //Creates a timer to measure the time since the last joystick input.

    int deadband = 5; //The joystick value allowed before the joystick controls start taking effect.
    bool moved = false; //Boolean that records if there was joystick movement within the last 0.3 seconds.

    //PISTON CONTROL VARIABLES
    bool pistonsState = false; //Example control variable.
    bool pistons2State = false; //Example control variable.

    while (true) { //Infinite loop to ensure continuous and smooth driving.
        double elapsed = Timer.time(seconds); //Assigns the current value of the timer to a variable.

        double a3 = Controller1.Axis3.position() * 1; //Records the position of axis 3 of the controller. Multiply by a number to tune sensitivity.
        double a1 = Controller1.Axis1.position() * 1; //Records the position of axis 1 of the controller. Multiply by a number to tune sensitivity.

        double left  = a3 + a1; //Calculates the volt output of the left chassis.
        double right = a3 - a1; //Calculates the volt output of the right chassis.

        double move = abs(Controller1.Axis3.position()); //Assigns the absolute value of axis 3 to the move variable.

        if (move > deadband) { //Checks if the movement of axis 3 is greater than deadband. This code's purpose is to set the stop type to brake
                               //after 0.3 seconds of no movement. This prevents the floaty feeling for the driver. If a driver prefers the floaty
                               //feel, then comment this if-statement out.
            moved = true; //The robot has moved if the if-statement evaluates to true.
            Timer.clear(); //Clears the timer.
        }

        if (elapsed > 0.3) { //Checks if the time elapsed since the last movement has exceeded 0.3 seconds.
            moved = false; //Changes the moved variable back to false.
        }

        if (abs(left) < deadband) { //Checks if there is input beyond the deadband.
            if (moved == false) { //Checks if there was input within the last 0.3 seconds.
                LeftMotor_group.stop(brake); //Uses brake to stop the robot if there has been no movement input for the last 0.3 seconds.
            } else { //There was input within the last 0.3 seconds.
                LeftMotor_group.stop(coast); //Allows the robot to coast and "float" for a small period of time before braking.
            }
        } else { //There is input.
            LeftMotor_group.spin(forward, left * 0.12, volt); //Spins the motors based on the calculated output.
        }

        if (abs(right) < deadband) { //Checks if there is input beyond the deadband.
            if (moved == false) { //Checks if there was input within the last 0.3 seconds.
                RightMotor_group.stop(brake); //Uses brake to stop the robot if there has been no movement input for the last 0.3 seconds.
            } else { //There was input within the last 0.3 seconds.
                RightMotor_group.stop(coast); //Allows the robot to coast and "float" for a small period of time before braking.
            }
        } else { //There is input.
            RightMotor_group.spin(forward, right * 0.12, volt); //Spins the motors based on the calculated output.
        }

        if (Controller1.ButtonX.pressing()) { //Checks if a button is pressed.
            if (pistonsState == true) {pistonsState = false;} //Sets the variable to false.
            else if (pistonsState == false) {pistonsState = true;} //Sets the variable to true.
            while (Controller1.ButtonX.pressing()) {wait(5, msec);} //Avoids detection again before the finger has been raised.
        }

        if (Controller1.ButtonA.pressing()) { //Checks if a button is pressed.
            if (pistons2State == true) {pistons2State = false;} //Sets the variable to false.
            else if (pistons2State == false) {pistons2State = true;} //Sets the variable to true.
            while (Controller1.ButtonA.pressing()) {wait(5, msec);} //Avoids detection again before the finger has been raised.
        }

        //Uses mutually exclusive button states so pressing both directions safely stops the motors.
        if (Controller1.ButtonR1.pressing() && !Controller1.ButtonR2.pressing()) {
            FrontMotor.spin(reverse, 12, volt);
            BackMotor.spin(reverse, 12, volt);
        } else if (Controller1.ButtonR2.pressing() && !Controller1.ButtonR1.pressing()) {
            FrontMotor.spin(forward, 12, volt);
            BackMotor.spin(forward, 12, volt);
        } else {
            FrontMotor.stop(coast);
            BackMotor.stop(coast);
        }

        //Activates the piston based on the state of the control variable.
        if (pistonsState == true) {
            Pistons.set(true);
        } else if (pistonsState == false) {
            Pistons.set(false);
        }

        //Activates the piston based on the state of the control variable.
        if (pistons2State == true) {
            Pistons2.set(true);
        } else if (pistons2State == false) {
            Pistons2.set(false);
        }

        wait(20, msec); //Do not remove this. If removed, the robot will disconnect easily during matches. I am telling you this based on personal experience.
    }
}

//TROUBLESHOOTING----------------------------------------------------------------------------------------------------------------------------------------------
/*
Q: My robot keeps disconnecting during use?
A: Didn't I tell you not to remove wait(20, msec)?

Q: I wrote something to activate when I press a button, but it doesn't work?
A: Did you download the program to the Brain? If so, make sure the button condition, device names, and motor directions are correct.

Q: My problem is not in the troubleshooting section?
A: Go to the GitHub repository, click on the "Issues" tab, and submit a new issue describing your problem. I will respond.
*/
