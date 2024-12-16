# Design-of-MM
README for Memory Management Simulator

Britney Lopez bnlopez2@csu.fullerton.edu\
  
worked on debugging & readme file
Alexie Gonzalez alexieg3@csu.fullerton.edu
  
worked on code 

Project Overview
Used C++

How to Execute the Program

Prerequisites
Platform: A Linux or Windows environment.
Compiler: g++ (GNU Compiler Collection) for Linux or MinGW for Windows.
Required Files: Ensure the source file memory_management_sim.cpp and input file in1 are present in the working directory.

  Compilation Steps
Open a terminal or command prompt.
Navigate to the directory containing memory_management_sim.cpp.
Compile the program using the following command:
bash
g++ -o memory_management_sim memory_management_sim.cpp

Execution Steps
Run the compiled program:
bash
./memory_management_sim

For Windows:
cmd
memory_management_sim.exe


Enter  memory size and page size
Specify the process details by giving an input file named in1

Input File Format (in1)
The first line has to contain the number of processes, N.
Each process block contains:
Process ID (integer).
Arrival time and lifetime (two integers).
Number of address space blocks and their sizes (integer followed by space-separated integers).
Blank line separates each process block.
