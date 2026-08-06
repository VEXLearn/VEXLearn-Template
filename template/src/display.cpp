//INTRODUCTION-------------------------------------------------------------------------------------------------------------------------------------------------
/*
The display.cpp file contains the functions for the Brain and controller displays. The BrainRender function is called in main.cpp to render the Brain
display, and the ControllerRender function is called in main.cpp to render the controller display. You can customize what is displayed on the Brain
and controller by changing the contents of these functions. You can also add more functions if you want to display different things on the brain and controller,
but make sure to call those functions in main.cpp as well.
*/

//STRUCTURE EXPLANATION----------------------------------------------------------------------------------------------------------------------------------------
/*
The display functions use an infinite loop to update the display continuously, and they use the screen's setCursor, clearScreen, print, and newLine functions
to display information. You can use the screen's setFont and setPenColor functions to change the font and color of the text. The ControllerRender function
displays the heading, temperature, and battery percentage on the controller screen. The MainMenu function displays various information on the brain screen
and has buttons for different screens. You can customize what is displayed and how it looks by changing the code in these functions.
*/

//CODE---------------------------------------------------------------------------------------------------------------------------------------------------------
#include "robotConfig.h" //Includes the robotConfig header file so you can use motor functions.

enum ScreenState {MENU, MOTORS, BATTERY, TEAM, SOFTWARE}; //See the PDF file for an explanation of enums. Declares screen states for future use.
ScreenState currentScreen = MENU; //The display OS starts with the menu screen.

void ControllerRender() { //The controller-screen rendering function.
    while (true) { //Infinite loop.
        double temperature = ((LeftMotor1.temperature() + LeftMotor2.temperature() + LeftMotor3.temperature()
                              + RightMotor1.temperature() + RightMotor2.temperature() + RightMotor3.temperature()
                             ) / 6); //temperature = average motor temperature

        Controller1.Screen.setCursor(1, 1); //Text starts at row 1, column 1 of the text grid.
        Controller1.Screen.clearScreen(); //Clears anything on screen.
        Controller1.Screen.print("Heading: %.1f deg", Inertial.heading()); //Prints the heading on the screen.
        Controller1.Screen.newLine(); //Prints a new line.
        Controller1.Screen.print("Temperature: %.1f deg C", temperature); //Prints the average motor temperature on the screen.
        Controller1.Screen.newLine(); //Prints a new line.
        Controller1.Screen.print("Battery: %d%%", Brain.Battery.capacity()); //Prints the remaining battery capacity on the screen.

        wait(100, msec); //Waits 0.1 seconds before refreshing the screen, making the display update at 10 Hz.
    }
}

void MainMenu() { //The main menu screen.
    Brain.Screen.setOrigin(90, 68); //Sets coordinates 90, 68 as the origin.
    Brain.Screen.setCursor(1,1); //Text starts at row 1, column 1 of the text grid.
    Brain.Screen.setFont(mono60); //Sets a big font size.
    Brain.Screen.clearScreen(); //Clears the screen.
    Brain.Screen.print("Loading..."); //Since there is a 0.5 second wait, showing a loading screen makes the program feel more responsive.

    wait (0.5, sec); //Waits to avoid refreshing the screen before the finger lifts, which would return to another screen.

    Brain.Screen.clearScreen(); //Clears the screen.
    Brain.Screen.setCursor(1,1); //Text starts at row 1, column 1 of the text grid.
    Brain.Screen.setFont(monoM); //Sets the font size.
    Brain.Screen.setPenColor(white); //Sets the text color.

    while (true) { //Infinite loop.
        double touchX; //Variable containing the touch's x-coordinate.
        double touchY; //Variable containing the touch's y-coordinate.
        Brain.Screen.setOrigin(0, 0); //Sets coordinates 0, 0 as the origin.

        //The following is the content of the main menu screen. The code itself is pretty self-explanatory. Copy and paste anything you don't understand into AI,
        //and it will do a pretty good job of explaining it.
        Brain.Screen.print("--------------------Main Menu-------------------");
        Brain.Screen.newLine();
        Brain.Screen.print("Welcome to team_name's robot (team_number)!"); //Replace team_name and team_number.
        Brain.Screen.newLine();
        Brain.Screen.print("------------------------------------------------");
        Brain.Screen.newLine();
        Brain.Screen.print("Battery Remaining: ");
        Brain.Screen.print(Brain.Battery.capacity());
        Brain.Screen.print("%");
        Brain.Screen.newLine();
        Brain.Screen.print("Battery Temperature: %.1f deg C", Brain.Battery.temperature());
        Brain.Screen.newLine();
        Brain.Screen.print("Battery Draw: %.2f A", Brain.Battery.current());
        Brain.Screen.newLine();
        Brain.Screen.print("------------------------------------------------");
        Brain.Screen.newLine();
        if (Brain.SDcard.isInserted()) {
            Brain.Screen.print("SD Card Status: Inserted");
        } else {
            Brain.Screen.print("SD Card Status: Not Inserted");
        }
        Brain.Screen.newLine();
        Brain.Screen.print("Inertial Heading: %.0f", Inertial.heading());
        Brain.Screen.newLine();
        Brain.Screen.print("Software Version: 1.0.0");

        //Draws the buttons
        Brain.Screen.drawRectangle(0,200, 120, 40, blue);
        Brain.Screen.drawRectangle(120,200, 120, 40, red);
        Brain.Screen.drawRectangle(240,200, 120, 40, green);
        Brain.Screen.drawRectangle(360,200, 120, 40, purple);

        //Gives the buttons text
        Brain.Screen.printAt(30, 225, "Motors");
        Brain.Screen.printAt(145, 225, "Battery");
        Brain.Screen.printAt(280, 225, "Team");
        Brain.Screen.printAt(380, 225, "Software");

        if (Brain.Screen.pressing() == true) { //Detects whether the Brain screen is touched.
            touchX=Brain.Screen.xPosition(); //Records the touch's x-coordinate.
            touchY=Brain.Screen.yPosition(); //Records the touch's y-coordinate.

            if (touchX > 0 && touchX < 120 && touchY > 200 && touchY < 240) { //Determines if touch is on a button
                currentScreen = MOTORS; //Change the screen to the button's screen
                touchX = 0; //Resets the variable.
                touchY = 0; //Resets the variable.
                break; //Breaks from the loop.
            } else if (touchX > 120 && touchX < 240 && touchY > 200 && touchY < 240) { //Determines if touch is on a button
                currentScreen = BATTERY; //Change the screen to the button's screen
                touchX = 0; //Resets the variable.
                touchY = 0; //Resets the variable.
                break; //Breaks from the loop.
            } else if (touchX > 240 && touchX < 360 && touchY > 200 && touchY < 240) { //Determines if touch is on a button
                currentScreen = TEAM; //Change the screen to the button's screen
                touchX = 0; //Resets the variable.
                touchY = 0; //Resets the variable.
                break; //Breaks from the loop.
            } else if (touchX > 360 && touchX < 480 && touchY > 200 && touchY < 240) { //Determines if touch is on a button
                currentScreen = SOFTWARE; //Change the screen to the button's screen
                touchX = 0; //Resets the variable.
                touchY = 0; //Resets the variable.
                break; //Breaks from the loop.
            } else { //None of the buttons were touched.
                currentScreen = MENU; //The screen shows the same menu.
                touchX = 0; //Resets the variable.
                touchY = 0; //Resets the variable.
            }
        }
        wait(100, msec); //Waits 0.1 seconds before refreshing the screen, making the display update at 10 Hz.
    }
}

void Motors() { //The motor screen.
    Brain.Screen.setOrigin(90, 68); //Sets coordinates 90, 68 as the origin.
    Brain.Screen.setCursor(1,1); //Text starts at row 1, column 1 of the text grid.
    Brain.Screen.setFont(mono60); //Sets a big font size.
    Brain.Screen.clearScreen(); //Clears the screen.
    Brain.Screen.print("Loading..."); //Since there is a 0.5 second wait, showing a loading screen makes the program feel more responsive.

    wait (0.5, sec); //Waits to avoid refreshing the screen before the finger lifts, which would return to another screen.

    Brain.Screen.clearScreen(); //Clears the screen.
    Brain.Screen.setOrigin(0,0); //Sets 0, 0 as the origin.
    Brain.Screen.setFont(monoS); //Sets the font size.
    Brain.Screen.setPenColor(white); //Sets the text color.

    while (true) {
        double touchX; //Variable containing the touch's x-coordinate.
        double touchY; //Variable containing the touch's y-coordinate.

        // Brain.Screen.clearScreen(); //Clears the screen

        //Prints the velocity, temperature, and voltage information for each motor.
        Brain.Screen.setCursor(1,1);
        Brain.Screen.print("Left Motor 1: ");
        Brain.Screen.newLine();
        Brain.Screen.print("    Velocity: ");
        Brain.Screen.print(LeftMotor1.velocity(rpm));
        Brain.Screen.print(" RPM");
        Brain.Screen.newLine();
        Brain.Screen.print("    Temperature: ");
        Brain.Screen.print(LeftMotor1.temperature());
        Brain.Screen.print(" deg C");
        Brain.Screen.newLine();
        Brain.Screen.print("    Voltage: ");
        Brain.Screen.print(LeftMotor1.voltage());
        Brain.Screen.print(" V");
        Brain.Screen.newLine();

        Brain.Screen.print("Left Motor 2: ");
        Brain.Screen.newLine();
        Brain.Screen.print("    Velocity: ");
        Brain.Screen.print(LeftMotor2.velocity(rpm));
        Brain.Screen.print(" RPM");
        Brain.Screen.newLine();
        Brain.Screen.print("    Temperature: ");
        Brain.Screen.print(LeftMotor2.temperature());
        Brain.Screen.print(" deg C");
        Brain.Screen.newLine();
        Brain.Screen.print("    Voltage: ");
        Brain.Screen.print(LeftMotor2.voltage());
        Brain.Screen.print(" V");
        Brain.Screen.newLine();

        Brain.Screen.print("Left Motor 3: ");
        Brain.Screen.newLine();
        Brain.Screen.print("    Velocity: ");
        Brain.Screen.print(LeftMotor3.velocity(rpm));
        Brain.Screen.print(" RPM");
        Brain.Screen.newLine();
        Brain.Screen.print("    Temperature: ");
        Brain.Screen.print(LeftMotor3.temperature());
        Brain.Screen.print(" deg C");
        Brain.Screen.newLine();
        Brain.Screen.print("    Voltage: ");
        Brain.Screen.print(LeftMotor3.voltage());
        Brain.Screen.print(" V");

        Brain.Screen.setOrigin(240, 0);
        Brain.Screen.print("Right Motor 1: ");
        Brain.Screen.newLine();
        Brain.Screen.print("    Velocity: ");
        Brain.Screen.print(RightMotor1.velocity(rpm));
        Brain.Screen.print(" RPM");
        Brain.Screen.newLine();
        Brain.Screen.print("    Temperature: ");
        Brain.Screen.print(RightMotor1.temperature());
        Brain.Screen.print(" deg C");
        Brain.Screen.newLine();
        Brain.Screen.print("    Voltage: ");
        Brain.Screen.print(RightMotor1.voltage());
        Brain.Screen.print(" V");
        Brain.Screen.newLine();

        Brain.Screen.print("Right Motor 2: ");
        Brain.Screen.newLine();
        Brain.Screen.print("    Velocity: ");
        Brain.Screen.print(RightMotor2.velocity(rpm));
        Brain.Screen.print(" RPM");
        Brain.Screen.newLine();
        Brain.Screen.print("    Temperature: ");
        Brain.Screen.print(RightMotor2.temperature());
        Brain.Screen.print(" deg C");
        Brain.Screen.newLine();
        Brain.Screen.print("    Voltage: ");
        Brain.Screen.print(RightMotor2.voltage());
        Brain.Screen.print(" V");
        Brain.Screen.newLine();

        Brain.Screen.print("Right Motor 3: ");
        Brain.Screen.newLine();
        Brain.Screen.print("    Velocity: ");
        Brain.Screen.print(RightMotor3.velocity(rpm));
        Brain.Screen.print(" RPM");
        Brain.Screen.newLine();
        Brain.Screen.print("    Temperature: ");
        Brain.Screen.print(RightMotor3.temperature());
        Brain.Screen.print(" deg C");
        Brain.Screen.newLine();
        Brain.Screen.print("    Voltage: ");
        Brain.Screen.print(RightMotor3.voltage());
        Brain.Screen.print(" V");
        Brain.Screen.newLine();

        //Prints the velocity, temperature, and voltage information for each side of the chassis.
        Brain.Screen.setOrigin(190, 210);
        Brain.Screen.print("Average Left Motor RPM: ");
        Brain.Screen.print((LeftMotor1.velocity(rpm) + LeftMotor2.velocity(rpm) + LeftMotor3.velocity(rpm)) / 3);
        Brain.Screen.print(" RPM");
        Brain.Screen.newLine();
        Brain.Screen.print("Average Right Motor RPM: ");
        Brain.Screen.print((RightMotor1.velocity(rpm) + RightMotor2.velocity(rpm) + RightMotor3.velocity(rpm)) / 3);
        Brain.Screen.print(" RPM");

        Brain.Screen.setOrigin(0, 0); //Resets the origin to 0, 0.
        Brain.Screen.drawRectangle(0, 200, 175, 40, red); //Draws the return-to-menu button.
        Brain.Screen.printAt(30, 225, "Return to Menu"); //Button text.

        if (Brain.Screen.pressing() == true) { //Detects a touch.
            touchX=Brain.Screen.xPosition(); //Stores the touch's x-coordinate.
            touchY=Brain.Screen.yPosition(); //Stores the touch's y-coordinate.

            if (touchX > 0 && touchX < 175 && touchY > 200 && touchY < 240) { //Checks whether the button is touched.
                currentScreen = MENU; //Returns to the menu if true.
                touchX = 0; //Resets the variable.
                touchY = 0; //Resets the variable.
                break; //Breaks from the loop.
            } else { //Remain on the same screen
                touchX = 0; //Resets the variable.
                touchY = 0; //Resets the variable.
            }
        }
        wait(0.1, sec); //Waits 0.1 seconds before refreshing the screen, making the display update at 10 Hz.
    }
}

void Battery() { //Page showing battery information.
    Brain.Screen.setOrigin(90, 68); //Sets coordinates 90, 68 as the origin.
    Brain.Screen.setCursor(1,1); //Text starts at row 1, column 1 of the text grid.
    Brain.Screen.setFont(mono60); //Sets a big font size.
    Brain.Screen.clearScreen(); //Clears the screen.
    Brain.Screen.print("Loading..."); //Since there is a 0.5 second wait, showing a loading screen makes the program feel more responsive.

    wait (0.5, sec); //Waits to avoid refreshing the screen before the finger lifts, which would return to another screen.

    Brain.Screen.clearScreen(); //Clears the screen.
    Brain.Screen.setOrigin(0,0); //Sets 0, 0 as the origin.
    Brain.Screen.setPenColor(white); //Sets the text color.

    while (true) {
        Brain.Screen.setFont(monoL);
        double touchX; //Variable containing the touch's x-coordinate.
        double touchY; //Variable containing the touch's y-coordinate.

        // Brain.Screen.clearScreen(); //Clears the screen

        //Prints the remaining battery charge, voltage, current draw, and temperature.
        Brain.Screen.setCursor(1,1);
        Brain.Screen.print("Battery Info: ");
        Brain.Screen.newLine();
        Brain.Screen.print("    Charge Remaining: ");
        Brain.Screen.print(Brain.Battery.capacity());
        Brain.Screen.print("%");
        Brain.Screen.newLine();
        Brain.Screen.print("    Voltage: ");
        Brain.Screen.print(Brain.Battery.voltage());
        Brain.Screen.print(" V");
        Brain.Screen.newLine();
        Brain.Screen.print("    Total Draw: ");
        Brain.Screen.print(Brain.Battery.current());
        Brain.Screen.print(" A");
        Brain.Screen.newLine();
        Brain.Screen.print("    Temperature: ");
        Brain.Screen.print(Brain.Battery.temperature());
        Brain.Screen.print(" deg C");

        //The same return-to-menu button mechanism.
        Brain.Screen.setFont(monoS);
        Brain.Screen.drawRectangle(0, 200, 175, 40, red);
        Brain.Screen.printAt(30, 225, "Return to Menu");

        if (Brain.Screen.pressing() == true) {
            touchX=Brain.Screen.xPosition();
            touchY=Brain.Screen.yPosition();

            if (touchX > 0 && touchX < 175 && touchY > 200 && touchY < 240) {
                currentScreen = MENU;
                touchX = 0;
                touchY = 0;
                break;
            } else {
                touchX = 0;
                touchY = 0;
            }
        }
        wait(0.1, sec); //Waits 0.1 seconds before refreshing the screen, making the display update at 10 Hz.
    }
}

void Team() { //Team tab, mostly a gimmick to show judges during interviews.
    Brain.Screen.setOrigin(90, 68); //Sets coordinates 90, 68 as the origin.
    Brain.Screen.setCursor(1,1); //Text starts at row 1, column 1 of the text grid.
    Brain.Screen.setFont(mono60); //Sets a big font size.
    Brain.Screen.clearScreen(); //Clears the screen.
    Brain.Screen.print("Loading..."); //Since there is a 0.5 second wait, showing a loading screen makes the program feel more responsive.

    wait (0.5, sec); //Waits to avoid refreshing the screen before the finger lifts, which would return to another screen.

    Brain.Screen.clearScreen(); //Clears the screen.
    Brain.Screen.setOrigin(0,0); //Sets 0, 0 as the origin.
    Brain.Screen.setPenColor(white); //Sets the text color.

    while (true) {
        double touchX; //Variable containing the touch's x-coordinate.
        double touchY; //Variable containing the touch's y-coordinate.

        // Brain.Screen.clearScreen(); //Clears the screen.

        Brain.Screen.setFont(monoXS); //Sets the font size.
        Brain.Screen.setCursor(1,1); //Sets text to start printing at row 1, column 1 of the text grid.

        Brain.Screen.print("Team: Team_name Team_number");
        Brain.Screen.newLine();
        Brain.Screen.print("Members: Person 1, Person 2, and Person 3");
        Brain.Screen.newLine();
        Brain.Screen.newLine();
        Brain.Screen.print("Name: Role");
        Brain.Screen.newLine();
        Brain.Screen.print("description description description description description------");
        Brain.Screen.newLine();
        Brain.Screen.print("description description description description description------");
        Brain.Screen.newLine();
        Brain.Screen.print("description description description description description------");
        Brain.Screen.newLine();
        Brain.Screen.newLine();
        Brain.Screen.print("Name: Role");
        Brain.Screen.newLine();
        Brain.Screen.print("description description description description description------");
        Brain.Screen.newLine();
        Brain.Screen.print("description description description description description------");
        Brain.Screen.newLine();
        Brain.Screen.print("description description description description description------");
        Brain.Screen.newLine();
        Brain.Screen.newLine();
        Brain.Screen.print("Name: Role");
        Brain.Screen.newLine();
        Brain.Screen.print("description description description description description------");
        Brain.Screen.newLine();
        Brain.Screen.print("description description description description description------");
        Brain.Screen.newLine();
        Brain.Screen.print("description description description description description------");

        //The same return-to-menu button mechanism.
        Brain.Screen.setFont(monoS);
        Brain.Screen.drawRectangle(0, 210, 175, 40, red);
        Brain.Screen.printAt(30, 230, "Return to Menu");

        if (Brain.Screen.pressing() == true) {
            touchX=Brain.Screen.xPosition();
            touchY=Brain.Screen.yPosition();

            if (touchX > 0 && touchX < 175 && touchY > 210 && touchY < 240) {
                currentScreen = MENU;
                touchX = 0;
                touchY = 0;
                break;
            } else {
                touchX = 0;
                touchY = 0;
            }
        }
        wait(0.1, sec); //Waits 0.1 seconds before refreshing the screen, making the display update at 10 Hz.
    }
}

void Software() { //A placeholder tab; feel free to change it to display anything you want.
    Brain.Screen.setOrigin(90, 68); //Sets coordinates 90, 68 as the origin.
    Brain.Screen.setCursor(1,1); //Text starts at row 1, column 1 of the text grid.
    Brain.Screen.setFont(mono60); //Sets a big font size.
    Brain.Screen.clearScreen(); //Clears the screen.
    Brain.Screen.print("Loading..."); //Since there is a 0.5 second wait, showing a loading screen makes the program feel more responsive.

    wait (0.5, sec); //Waits to avoid refreshing the screen before the finger lifts, which would return to another screen.

    Brain.Screen.clearScreen(); //Clears the screen.
    Brain.Screen.setOrigin(0,0); //Sets 0, 0 as the origin.
    Brain.Screen.setPenColor(white); //Sets the text color.

    while (true) {
        double touchX; //Variable containing the touch's x-coordinate.
        double touchY; //Variable containing the touch's y-coordinate.

        // Brain.Screen.clearScreen(); //Clears the screen

        //Whatever you want to write.
        Brain.Screen.setCursor(1,1);
        Brain.Screen.setFont(monoM);
        Brain.Screen.print("The VEXLearn Template");
        Brain.Screen.newLine();
        Brain.Screen.print("This is a placeholder page. Feel free to");
        Brain.Screen.newLine();
        Brain.Screen.print("change this.");

        //The same return-to-menu button mechanism.
        Brain.Screen.setFont(monoS);
        Brain.Screen.drawRectangle(0, 200, 175, 40, red);
        Brain.Screen.printAt(30, 225, "Return to Menu");

        if (Brain.Screen.pressing() == true) {
            touchX=Brain.Screen.xPosition();
            touchY=Brain.Screen.yPosition();

            if (touchX > 0 && touchX < 175 && touchY > 200 && touchY < 240) {
                currentScreen = MENU;
                touchX = 0;
                touchY = 0;
                break;
            } else {
                touchX = 0;
                touchY = 0;
            }
        }
        wait(0.1, sec); //Waits 0.1 seconds before refreshing the screen, making the display update at 10 Hz.
    }
}

void BrainRender() { //The Brain-screen rendering function.
    while (true) { //Infinite loop.
        Brain.Screen.clearScreen(); //Clears everything on the screen.
        switch (currentScreen) { //Switch statement with the current screen variable as its argument.
            case MENU: //Display screen based on currentScreen's value.
                MainMenu(); //Display
                break; //Breaks from the switch statement.
            case MOTORS: //Display screen based on currentScreen's value.
                Motors(); //Display
                break; //Breaks from the switch statement.
            case BATTERY: //Display screen based on currentScreen's value.
                Battery(); //Display
                break; //Breaks from the switch statement.
            case TEAM: //Display screen based on currentScreen's value.
                Team(); //Display
                break; //Breaks from the switch statement.
            case SOFTWARE: //Display screen based on currentScreen's value.
                Software(); //Display
                break; //Breaks from the switch statement.
        }
    }
}
