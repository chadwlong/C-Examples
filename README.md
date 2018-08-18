# C-Examples
Examples of my code in C or C++


## Follower-Watchdog-Controller.c

This program shows the software in C for a system that I completed for a unique project in school several years ago. This program ran on a Peripheral Interface Controller (PIC) microcontroller, the PIC2580. **I specifcally wrote the code for lines 9 - 298 with all method's labeled as "Chad's method and code" in the comments.**
This code indicates to the main platform that the user wants to operate in "Watchdog" or "Follower" mode based on the switch input.
Each mode results in different LED outputs for the user to know the current mode while IR signal outputs and XBEE outputs communicate with the main platform.

## Liveness Analysis

**As noted in the placeholder file, please contact me at cwlong@okstate.edu or linkedin.com/in/chad-long-3b1543100 to see this code** 

This program performs a liveness analysis in C++ on an instruction set produced by LLVM. Information on LLVM can be found at the following links:

http://llvm.org
http://releases.llvm.org/3.6.2/docs/GettingStarted.html
http://releases.llvm.org/3.6.2/docs/ProgrammersManual.html 

A very simple program, approximately 5 lines, produced the instruction set in this file and is at the beginning of the code. This code involves a simple use of set vectors and maps with pointers and derefences to accomplish the analysis. Liveness analysis involves analyzing an instruction chain backwards to find out which variables are live at a certain instruction in the execution of a program. This can be helpful in determining the minimum number of registers needed for a program to store values. Please read about liveness analysis here: https://en.wikipedia.org/wiki/Live_variable_analysis.

## Reaching Definition Analysis

**As noted in the placeholder file, please contact me at cwlong@okstate.edu or linkedin.com/in/chad-long-3b1543100 to see this code**

This program performs a reaching definition analysis in C++ on an instruction set produced by LLVM. Information on LLVM can be found in the links contained in the description for LivenessAnalysis.cpp seen above.

A very simple program, approximately 5 lines, produced the instruction set in this file and is referenced at the beginning of the code. This code involves a simple use of set vectors and maps with pointers and derefences to accomplish the analysis. Reaching definition analysis involves analyzing an instruction chain forwards to find out if the definition of a variable reaches a specific instruction location in the execution of a program. Please read about reaching definition analysis here: https://en.wikipedia.org/wiki/Reaching_definition.
