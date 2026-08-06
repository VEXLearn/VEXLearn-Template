//INTRODUCTION-------------------------------------------------------------------------------------------------------------------------------------------------
/*
I would say this is the most complex part of VEX software. If your movement code is not good, you will struggle to write autonomous functions. In
seasons such as VEX Push Back and VEX High Stakes, where autonomous performance is very important, you will likely lose if your autonomous performance isn't
good.

If anything seems too complex, don't be afraid to use resources like AI (ChatGPT, Gemini, Grok, Claude, etc.), Reddit (https://www.reddit.com/r/vex/),
the VEX Forum (https://www.vexforum.com/), your mentor, the VEXLearn GitHub Issues page, the PDF document, or Discord communities (Robolytics is a very large one:
https://discord.gg/robolytics-1272763279739191296).

The movement functions use voltage control because it directly determines motor output. This keeps the PID loop simple and gives you full control over its response.
*/

//STRUCTURE EXPLANATION----------------------------------------------------------------------------------------------------------------------------------------
/*
Your movement.cpp file will start by including any header files that contain movement functions. If you haven't changed my file structure, you will include
the vex.h header file as well as the robotConfig.h header file. An include follows this format:

#include whatever_file

In this file, it would be:

#include "vex.h"
#include "robotConfig.h"

Function declarations follow the normal declaration format.

return_type function_name(arguments) {
    contents
}

Generally, a movement function declaration looks like this:

void move(double dis, double heading, double volt_max, double exit_volt = 0, bool stop = true) {
    contents
}

The return type is void because movement functions should not return anything. You pass the functions' arguments, such as the distance to travel, the heading to
follow, and the maximum voltage allowed. You should also name your autonomous function after what it does. For example, the move function moves the robot.
*/

//CODE---------------------------------------------------------------------------------------------------------------------------------------------------------
#include "vex.h" //A variable called wheel_2r is declared in this header file. wheel_2r is the diameter of the wheel. You can declare the variable inside
                 //the functions if you want. I declared it in vex.h so I won't have to change several wheel_2r values if the wheel size changes.
#include "robotConfig.h" //This header file contains all the motors, pistons, inertial sensors, and other sensors on your robot. Because the functions require
                         //data from the sensors and motors to work, we have to include this file.

void motor_spin(double L_volt, double R_volt) { //Declares the motor_spin function. This function spins the motors at the voltage passed in the arguments.
    LeftMotor_group.spin(fwd, L_volt, volt); //Spins LeftMotor_group at L_volt. fwd is the direction, L_volt is the power, and volt is the unit.
    RightMotor_group.spin(fwd, R_volt, volt); //Spins RightMotor_group at R_volt. fwd is the direction, R_volt is the power, and volt is the unit.
}

void motor_reset() { //Declares the motor_reset function. This function resets each motor's position value to 0.
    LeftMotor_group.resetPosition(); //Resets the position of each motor in LeftMotor_group (stored in degrees rotated) to 0.
    RightMotor_group.resetPosition(); //Resets the position of each motor in RightMotor_group (stored in degrees rotated) to 0.
}

void motor_stop(brakeType type = brake) { //Declares the motor-stop function. This function applies brakes of some type to each motor.
    LeftMotor_group.stop(type); //Applies brakes of type to each motor in LeftMotor_group.
    RightMotor_group.stop(type); //Applies brakes of type to each motor in RightMotor_group.
}

double clamp(double value, double minVal, double maxVal) { //Declares the clamp function. This function keeps value within the range minVal to maxVal.
    if (value > maxVal) return maxVal; //If value is greater than maxVal, then the function returns maxVal (the maximum value). Return ends the function.
    if (value < minVal) return minVal; //If value is smaller than minVal, then the function returns minVal (the minimum value). Return ends the function.
    return value; //If the code reaches this line, neither limit was reached, so it can return value without further comparison.
}

void stabilization() { //Declares the stabilization function. This function makes sure no motor is moving before it ends.
    while (fabs(LeftMotor_group.velocity(pct)) > 1 || fabs(RightMotor_group.velocity(pct)) > 1) { //Checks each side separately so opposite directions cannot cancel out.
        wait(10, msec); //Waits 10 milliseconds before checking the motor groups again.
    }
    wait(100, msec); //Waits another 100 milliseconds to ensure nothing is moving. You can decrease this value if you need to save time for autonomous.
}

void turn(double heading, double volt_max = 12, bool stop = true) { //Declares the turn function. This function will turn the robot. The heading variable is the
                                                                    //heading you want your robot to turn to. volt_max is the maximum voltage allowed.
                                                                    //The turn function uses absolute heading, so the heading is relative to the starting heading.
                                                                    //For example, you start facing forward; that is 0 degrees. When you want to turn to the opposite
                                                                    //direction, you input 180 degrees. When you want to turn back again, you input 0 degrees.
    timer limit_time; //Initializes (creates) a timer to measure how long the movement takes. If the timer exceeds the timeout,
                      //the movement will end. This keeps the robot from moving indefinitely if it gets stuck.
    limit_time.reset(); //Resets the timer.

    //TUNE THESE VALUES (REFERENCE THE PDF ON HOW TO TUNE)
    double Kp = 1; //Kp is the P value in the PID formula. Reference the PDF for more information.
    double Ki = 0; //Ki is the I value in the PID formula. Reference the PDF for more information.
    double Kd = 1; //Kd is the D value in the PID formula. Reference the PDF for more information.

    double error_INT = 0; //This error integral value is used in the PID formula. It is the sum of all previous error_now values.
    double error_now = 0; //This current error value is used in the PID formula. It is the difference between the current and goal headings.
    double error_last = 0; //This last error value is used in the PID formula. It is the previous error_now value.

    motor_reset(); //Resets all the motors' distance values.

    double volt_now = 0; //This is the current volt value.

    int count = 0; //Count prevents a brief arrival followed by an overshoot from tricking the code into thinking the robot has reached its destination.
                   //It makes sure the robot stays at the destination for a period of time before the function ends.

    while(count < 10 && limit_time.time(msec) < 2000) { //The main while loop stops only after count is greater than 10 or the timeout has been reached.
        double position = Inertial.heading(); //Records the current heading.
        error_now = (heading - position); //Determines the difference between the current and goal headings: goal - current = difference.
        if(error_now > 180) {error_now -= 360;} //Converts the error_now value to a -180~180 range.
        else if(error_now < -180) {error_now += 360;} //Converts the error_now value to a -180~180 range.

        if (fabs(volt_now) < volt_max) {error_INT += error_now;} //Accumulates the integral only while the motors are below the voltage limit.
        if (error_now * error_last < 0) {error_INT = 0;} //Resets accumulated error after the robot crosses the target heading.
        volt_now = clamp(Kp * (error_now) + Ki * error_INT + Kd * (error_now - error_last), -volt_max, volt_max); //PID calculation to determine the volt value.
                                                                                                                  //Uses clamp to make sure volt value does not
                                                                                                                  //exceed volt_max (the volt value limit).
        motor_spin(volt_now, - volt_now); //Executes the turning movement based on the calculated volt. Two sides spin in opposite direction to turn.

        if(fabs(error_now) < 2) {count++;} //Checks if the difference between current and goal heading is smaller than two degrees. If true, increases count.
        else{count = 0;} //If the value exceeds two degrees, resets count. This ensures the robot remains within two degrees for some time.
        error_last = error_now; //Sets the current error to the last error before the loop runs again.
        wait(10, msec); //Waits 10 milliseconds to avoid wasting computing resources. (It could also break the connection if set too low, speaking from experience.)
    }
    if (stop == false) { //Checks if it should stop
        motor_stop(coast); //If stop = false, then coast.
    } else { //Else follow normal ending procedure.
        motor_stop(brake); //Brakes the motors using the brake type specified.
        stabilization(); //Ensures the robot is stationary.
    }
}

void move(double dis, double heading, double volt_max, double exit_volt = 0, bool stop = true) { //Declares the move function. This function moves the robot.
                                                                                                  //Heading is the orientation of the robot at the end of the
                                                                                                  //movement. It is useful for curved routes, but since curved
                                                                                                  //routes are very hard to tune, I recommend that you stick to
                                                                                                  //straight line routes. Put heading = 0 for straight line.
    double target = ((dis * 360 * 4)/(wheel_2r * 2.54 * M_PI * 3)); //Converts the distance from centimeters to degrees.
                                                                    //dis / (wheel_2r * 2.54 * M_PI) converts the distance to wheel rotations.
                                                                    //Multiplication by 360 converts the rotations to degrees.
                                                                    //4/3 is the gear ratio. This is a PLACEHOLDER and MUST BE CHANGED to match your robot!!!
                                                                    //If you want to reverse the direction of movement, add -1* to the start, so the line becomes
                                                                    //double target = -1*((dis * 360 * 4)/(wheel_2r * 2.54 * M_PI * 3));
    double position_now = 0; //The current movement progress (starts at 0).

    double volt_now = 0; //The current power.
    double turn_volt = 0; //The turning power.

    double Kp; //Kp is the P value in the PID formula. Reference the PDF for more information.
    double Ki; //Ki is the I value in the PID formula. Reference the PDF for more information.
    double Kd; //Kd is the D value in the PID formula. Reference the PDF for more information.
    double k_turn = 0.7; //k_turn controls how aggressive the heading correction is. Change this value too since 0.7 is just a placeholder.

    if (heading != 0) {k_turn = 3;} //k_turn needs to be a lot more aggressive to achieve curved movement. Change this value too since 3 is just a placeholder.
    else {heading = Inertial.heading();} //If heading is 0, sets it to the robot's current heading, telling the robot to maintain its current course.

    //TUNE THESE VALUES (REFERENCE THE PDF ON HOW TO TUNE)
    if (volt_max == 12) { //PID set for 12 volts.
        Kp = 1;
        Ki = 0;
        Kd = 1;
    } else if (volt_max == 8) { //PID set for 8 volts.
        Kp = 1;
        Ki = 0;
        Kd = 1;
    } else if (volt_max == 6) { //PID set for 6 volts.
        Kp = 1;
        Ki = 0;
        Kd = 1;
    } else { //Default PID values if volt_max is not equivalent to any of the above conditions.
        Kp = 1;
        Ki = 0;
        Kd = 1;
    }
    //Different volt values can require different PID values. If you do not want to tune many different volt values, you can set them all to one set of PID values,
    //but it might not be accurate at different volt values. Lower volt values provide more accuracy, while higher volt values can move the robot faster. I do not
    //recommend using volt_max = 12 because the motor can't spin faster, which causes the code to lose the ability to do heading correction or curved routes.

    double distErrorInt = 0; //This distance error integral value is used in the PID formula. It is the sum of all previous distError values.
    double distErrorPrev = 0; //This previous distance error value is used in the PID formula. It is the previous distError value.
    double distError; //This current distance error value is used in the PID formula. It is the difference between the destination and current position.
    double angError; //This angular error value is used to maintain a straight line or curved route.

    double left_power; //The power provided to the left motor group.
    double right_power; //The power provided to the right motor group.
    motor_reset(); //Resets all the motors' distance values.

    timer timeOut; //Initializes (creates) a timer to measure how long the movement takes. If the timer exceeds the timeout,
                   //the movement will end. This keeps the robot from moving indefinitely if it gets stuck.
    timeOut.clear(); //Resets the timer.

    while (fabs(position_now) < fabs(target)) { //The main while loop. Checks if target is reached.
        distError = target - position_now; //Calculates the distance between the goal and current position.
        if (fabs(volt_now) < volt_max) { //Only accumulates the integral term when the robot isn't running at the volt limit. This prevents integral windup. For more
                                         //information about PID, refer to the PDF guide or the Wikipedia PID page, which has an explanation of this phenomenon.
            distErrorInt += distError; //Adds the distance error to the integral.
        }
        if (distError * distErrorPrev < 0) {distErrorInt = 0;} //Checks whether an overshoot has occurred. If so, resets the integral value, which prevents accumulated error
                                                               //from one half of the motion from dragging into the other.
        volt_now = (Kp * distError + Ki * distErrorInt + Kd * (distError - distErrorPrev)); //Calculates the volt value using the PID formula.

        double volt_min = (exit_volt > 0 && !stop) ? exit_volt : 0; //If exit volt is greater than 0 and stop is false, the minimum volt is set to the exit_volt,
                                                                    //else there is no minimum volt (0).
        if (abs(volt_now) > volt_max) volt_now = copysign(volt_max, volt_now); //If the volt exceeds the volt_max value, then the volt is set to volt_max while
                                                                               //maintaining the sign (+ or -).
        if (fabs(position_now) >= 0.5 * fabs(target)) {if (abs(volt_now) < volt_min) volt_now = copysign(volt_min, volt_now);} //Checks if the robot is more than
                                                                                                                   //halfway to the destination. This is because
                                                                                                                   //the exit_volt is used to regulate the volt
                                                                                                                   //at the end. Since the volt is definitely
                                                                                                                   //at or close to the max at halfway point,
                                                                                                                   //we can start regulating the minimum volt.
                                                                                                                   //If volt is lower than minimum volt, then
                                                                                                                   //volt is set to volt_min while maintaining
                                                                                                                   //the sign (+ or -).

        angError = heading - Inertial.heading(); //Calculates the angular error, which is the difference in angle between the current and goal heading.
        while (angError > 180) angError -= 360; //Converts the angError value to a -180~180 range.
        while (angError < -180) angError += 360; //Converts the angError value to a -180~180 range.
        turn_volt = angError * k_turn; //Converts the angle error to volts. k_turn is an arbitrary value used to scale the value of turn_volt.
        if (fabs(turn_volt) > fabs(volt_now) * 0.8) turn_volt = copysign(fabs(volt_now) * 0.8, turn_volt); //Limits heading correction to 80% of the current voltage.

        left_power  = volt_now + turn_volt; //The volt for the left motor group to turn at, turn_volt's sign is different from right_power to allow turning.
        right_power = volt_now - turn_volt; //The volt for the right motor group to turn at, turn_volt's sign is different from left_power to allow turning.

        left_power  = clamp(left_power, -volt_max, volt_max); //Makes sure volt is within power limit. -volt_max to handle backward movement.
        right_power = clamp(right_power, -volt_max, volt_max); //Makes sure volt is within power limit. -volt_max to handle backward movement.
        motor_spin(left_power, right_power); //Executes the physical movement.

        position_now = copysign((fabs(LeftMotor_group.position(deg)) + fabs(RightMotor_group.position(deg))) / 2, target); //Applies the target's direction to the
                                                                                                                           //average encoder distance so backward error approaches zero.
        distErrorPrev = distError; //Assigns the current distance error to the previous value before the loop runs again.

        if (timeOut.time(msec) > 7000) break; //Checks if the timeout of 7000 milliseconds is reached, breaks out of the while loop if timeout is reached.
        wait(10, msec); //Waits 10 milliseconds to avoid wasting computing resources. (It could also break the connection if set too low, speaking from experience.)
    }
    if (stop == false) { //Checks if stopping is required.
        motor_stop(coast); //If exit volt isn't 0, then make the motors coast instead of brake.
    } else { //Else follow normal ending procedure.
        motor_stop(brake); //Brakes the motors using the brake type specified.
        stabilization(); //Ensures the robot is stationary.
    }
}

//TROUBLESHOOTING----------------------------------------------------------------------------------------------------------------------------------------------
/*
Q: I have been tuning the PID for a very long time but it still isn't accurate?
A: It takes some time to properly tune the PID. If you haven't reached a usable PID after tuning for hours, you should consider asking online forums or
   the GitHub Issues page.

Q: I have to turn a small amount between two move functions. Is it possible to avoid stopping between them?
A: You have two options:
    1: Combine the two movements into one and use the heading argument. This will result in a curved route instead of two straight lines, but the ending
       position will be similar.
    2: Use the exit_volt argument in the move function. First, set exit_volt in the first function to your desired speed (a slower speed will result in a more
       accurate turn). Then, execute a turn function with the stop = false argument before using another move function. The distance traveled this way could be
       inaccurate, so you might need to change the travel-distance argument even if your PID is perfect.

Q: My problem is not in the troubleshooting section?
A: Go to the GitHub repository, click on the "Issues" tab, and submit a new issue describing your problem. I will respond.
*/
