//INTRODUCTION-------------------------------------------------------------------------------------------------------------------------------------------------
/*
This is the header file for drive.cpp. For more information about header files, check the PDF.
*/

//STRUCTURE EXPLANATION----------------------------------------------------------------------------------------------------------------------------------------
/*
The format for writing a function in a header file is:

return_type function_name(arguments);

It is VERY IMPORTANT that your return_type function_name(arguments) declaration is identical to the one in drive.cpp. A quick way to write this is to copy the
function declaration line from drive.cpp, remove the braces ({}) and everything inside them, and add a semicolon (;) to the end.
*/

//CODE---------------------------------------------------------------------------------------------------------------------------------------------------------
#pragma once //This prevents the header file from being duplicated when included multiple times. For more information, refer to the PDF file. This should be supported
             //by any modern compiler.

void drive(); //The drive function contains what the robot does during the user-control phase.

//TROUBLESHOOTING----------------------------------------------------------------------------------------------------------------------------------------------
/*
Q: I can't find my drive function in other files?
A: Make sure you have added your drive function to the header file. Make sure the header file is included in the other files. If it isn't, add
   #include "drive.h" to the other files. Make sure the function name is spelled correctly and is identical in drive.cpp, drive.h, and the other files.

Q: My problem is not in the troubleshooting section?
A: Go to the GitHub repository, click on the "Issues" tab, and submit a new issue describing your problem. I will respond.
*/
