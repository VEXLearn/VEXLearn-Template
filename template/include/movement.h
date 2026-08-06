//INTRODUCTION-------------------------------------------------------------------------------------------------------------------------------------------------
/*
This is the header file for movement.cpp. For more information about header files, check the PDF.
*/

//STRUCTURE EXPLANATION----------------------------------------------------------------------------------------------------------------------------------------
/*
The format for writing a function in a header file is:

return_type function_name(arguments);

It is VERY IMPORTANT that your return_type function_name(arguments) declaration is identical to the one in movement.cpp. A quick way to write this is to copy the
function declaration line from movement.cpp, remove the opening brace ({), and add a semicolon (;) to the end.
*/

//CODE---------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once //This prevents the header file from being duplicated when included multiple times. For more information, refer to the PDF file. This should be supported
             //by any modern compiler.

void turn(double heading, double volt_max = 12, bool stop = true);
void move(double dis, double heading, double volt_max, double exit_volt = 0, bool stop = true);

//TROUBLESHOOTING----------------------------------------------------------------------------------------------------------------------------------------------
/*
Q: I can't access the movement functions in another file?
A: Make sure you have added your movement function to the header file. Make sure the header file is included in the file you are working on. If it isn't, add
   #include "movement.h" to the file. Make sure the function name is spelled correctly and is identical in movement.cpp, movement.h, and the other file.

Q: My problem is not in the troubleshooting section?
A: Go to the GitHub repository, click on the "Issues" tab, and submit a new issue describing your problem. I will respond.
*/
