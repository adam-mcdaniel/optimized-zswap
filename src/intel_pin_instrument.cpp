/*
 * Copyright (C) 2004-2021 Intel Corporation.
 * SPDX-License-Identifier: MIT
 */
 
#include <iostream>
#include <fstream>
#include "pin.H"
using std::cerr;
using std::endl;
using std::ios;
using std::ofstream;
using std::string;
 
ofstream OutFile;
 
// The running count of instructions is kept here
// make it static to help the compiler optimize docount
static UINT64 icount = 0;
 
// This function is called before every instruction is executed
VOID docount() { icount++; }
 
// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID* v)
{
    // Insert a call to docount before every instruction, no arguments are passed
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);
}
 
KNOB< string > KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "inscount.out", "specify output file name");
 
// This function is called when the application exits
VOID Fini(INT32 code, VOID* v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << "Count " << icount << endl;
    OutFile.close();
}
 
/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */
 
INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}
 
/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
/*   argc, argv are the entire command line: pin -t <toolname> -- ...    */
/* ===================================================================== */
 
int main(int argc, char* argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();
 
    OutFile.open(KnobOutputFile.Value().c_str());
 
    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);
 
    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
 
    // Start the program, never returns
    PIN_StartProgram();
 
    return 0;
}
Instruction Address Trace (Instruction Instrumentation)

In the previous example, we did not pass any arguments to docount, the analysis procedure. In this example, we show how to pass arguments. When calling an analysis procedure, Pin allows you to pass the instruction pointer, current value of registers, effective address of memory operations, constants, etc. For a complete list, see IARG_TYPE.

With a small change, we can turn the instruction counting example into a Pintool that prints the address of every instruction that is executed. This tool is useful for understanding the control flow of a program for debugging, or in processor design when simulating an instruction cache.

We change the arguments to INS_InsertCall to pass the address of the instruction about to be executed. We replace docount with printip, which prints the instruction address. It writes its output to the file itrace.out.

This is how to run it and look at the output:

$ ../../../pin -t obj-intel64/itrace.so -- /bin/ls
Makefile          atrace.o     imageload.out  itrace      proccount
Makefile.example  imageload    inscount0      itrace.o    proccount.o
atrace            imageload.o  inscount0.o    itrace.out
$ head itrace.out
0x40001e90
0x40001e91
0x40001ee4
0x40001ee5
0x40001ee7
0x40001ee8
0x40001ee9
0x40001eea
0x40001ef0
0x40001ee0
$

The example can be found in source/tools/ManualExamples/itrace.cpp
/*
 * Copyright (C) 2004-2021 Intel Corporation.
 * SPDX-License-Identifier: MIT
 */
 
#include <stdio.h>
#include "pin.H"
 
FILE* trace;
 
// This function is called before every instruction is executed
// and prints the IP
VOID printip(VOID* ip) { fprintf(trace, "%p\n", ip); }
 
// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID* v)
{
    // Insert a call to printip before every instruction, and pass it the IP
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)printip, IARG_INST_PTR, IARG_END);
}
 
// This function is called when the application exits
VOID Fini(INT32 code, VOID* v)
{
    fprintf(trace, "#eof\n");
    fclose(trace);
}
 
/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */
 
INT32 Usage()
{
    PIN_ERROR("This Pintool prints the IPs of every instruction executed\n" + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}
 
/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
 
int main(int argc, char* argv[])
{
    trace = fopen("itrace.out", "w");
 
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();
 
    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);
 
    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
 
    // Start the program, never returns
    PIN_StartProgram();
 
    return 0;
}
