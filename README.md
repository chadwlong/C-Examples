# C-Examples
Examples of my code in C or C++


## Follower-Watchdog-Controller.c

This program shows the software in C for a system that I completed for my Senior Design project at Oklahoma State. This program ran on a Peripheral Interface Controller (PIC), the PIC2580. **I specifcally wrote the code for lines 9 - 298 with all method's labeled as "Chad's method and code" in the comments.**
This code indicates to the main platform that the user wants to operate in "Watchdog" or "Follower" mode based on the switch input.
Each mode results in different LED outputs for the user to know the current mode while IR signal outputs and XBEE outputs communicate with the main platform.

## LivenessAnalysis.cpp

**As noted in the placeholder ".txt" file, please contact me at cwlong@okstate.edu or linkedin.com/in/chad-long-3b1543100 to see this code** 

This program performs a liveness analysis in C++ on an instruction set produced by LLVM. Information on LLVM can be found at the following links:

http://llvm.org
http://releases.llvm.org/3.6.2/docs/GettingStarted.html
http://releases.llvm.org/3.6.2/docs/ProgrammersManual.html

A very simple program, approximately 5 lines, produced the instruction set in this file and is referenced in DataflowAnalysis seen at the beginning of the code. **I specifically wrote the code for lines 15 - 132 with comments indicating the start and end of "CHAD'S CODE."** 
Liveness analysis involves following an instruction chain to find out which variables are live at a certain instruction in the execution of a program. This can be helpful in determining the minimum number of registers needed for a program to store values. Please read about liveness analysis here: https://en.wikipedia.org/wiki/Live_variable_analysis.
