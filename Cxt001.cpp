/**
 * @mainpage Main Doxygen documentation page of the tiny tool
 * 
 * This Doxygen documentation page describes the features of the 
 * tiny tool for quality assurance developed in the course 
 * "Calidade na Xestion TIC (CXT)" during 2015-2016 by the following
 * authors:
 * 1. Name:Carlos Bouzón García, Email:carlos.bouzon.garcia@udc.es 
 * 2. Name:Mario Alonso Saavedra, Email:mario.alonso.saavedra@udc.es 
 * 
 * Given a program written in the C/C++ programming language, the tool
 * computes the Key Performance Indicators (KPIs) shortly described 
 * below (3 KPIs are mandatory; and 1 KPI is optional):
 * 1. Short description of the first KPI
 * 2. Short description of the second KPI
 * 3. Short description of the third KPI
 * 4. Short description of the fourth KPI (optional)
 * 
 * Candidate KPIs to be selected for implementation are briefly 
 * described below. Many of these KPIs have already been discussed in
 * the brainstorming session that took place at lecture time.
 * 
 * 1. Quality of the program in terms of size
 *    - Number of Source Lines Of Code (SLOC) of the whole program
 *    - Number of asembly Lines Of Code (LOC) of the whole program
 *      (stimated as lines of code in the LLVM intermediate representation)
 *    - Number of functions with <= 80 SLOC (or <= 200 LOC)
 *    - Number of loops with <= 80 SLOC (or <= 200 LOC)
 * 
 * 2. Quality of the program in terms of memory management
 *    - Number of memory accesses
 *    - Number of bytes (KB,MB,...) used by the program (memory consumption)
 *    - Number of malloc() calls with a mathcing free() call
 * 
 * 3. Quality of the program in terms of CPU instruction set architecture
 *    - Number of floating-point instructions
 *    - Number of integer instructions
 * 
 * 4. Quality of the program in terms of modularity
 *    - Number of global variables declared in the program
 *    - Number of object classes declared in the program
 *    - Number of calls to functions in the source code of the program
 *    - Number of calls to external functions in the source code of the program
 *    - Number of object classes with <= 80 SLOC (or <= 200 LOC)
 *    - Ratio of number of debug source code lines to source code lines
 *    - Ratio of number of I/O source code lines to source code lines
 * 
 * 5. Quality of the program in terms of documentation
 *    - Number of program variables that do not follow naming conventions
 *    - Number of function names that do not follow naming conventions
 *    - Number of object classes whose name does not follow naming conventions
 *    - Ratio of number of documentation lines to source code lines
 *    - Listing of external libraries used in the program
 *      (e.g., sockets, OpenMP, MPI)
 * 
 * 6. Quality of the program in terms of performance
 *    - Stimation of execution time consumed by the program (e.g., CPU time in seconds)
 *    - Stimation ofxecution time consumed by each function of the program
 *    - Stimation ofxecution time consumed by calls to external libraries
 *      (e.g., communications time, parallelization overhead time)
 * 
 */

/**
 * @file cxt0000.cpp
 * @brief Tiny tool for quality assurance based on LLVM infrastructure.
 *
 * This tool allows to analyze the source code of a program.
 * The tool uses the LLVM 3.7+ compiler infrastructure.
 * It assumes that CLANG 3.7+ has been used to create the 
 * file with the intermediate representation of the program.
 */
 
 
#include <iostream>

#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/LegacyPassNameParser.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/InitializePasses.h"
#include "llvm/LinkAllIR.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SourceMgr.h"

#include <Cxt001Pass.h>

using namespace std;
using namespace llvm;

std::unique_ptr<Module> getModuleFromFile(const char *path) {
  LLVMContext &Context = llvm::getGlobalContext();
  SMDiagnostic Err;

  auto result = parseIRFile(path, Err, Context);

  if (Err.getSourceMgr()) {
    cerr << Err.getMessage().str();
    exit(-1);
  }
  return result;
}

void initializeLLVM(int argc, char *argv[]) {
  sys::PrintStackTraceOnErrorSignal();
  llvm::PrettyStackTraceProgram X(argc, argv);

  // Initialize passes
  PassRegistry &Registry = *PassRegistry::getPassRegistry();
  initializeCore(Registry);
  initializeAnalysis(Registry);
}

/**
 * @brief Default main() entry point of the tool. 
 *
 * This is the default function main() that implements the 
 * default of the executable file.
 *
 * 1. It parses the file .ll with the LLVM intermediate reprsentation.
 *    Files .ll are created by executing clang with command-line
 *    options "-S -emit-llvm -g".
 *
 * 2. It creates a LLVM compiler pass called "cxtPass".
 *
 * 3. Runs the pass "cxtPass" and dumps the information collected
 *    about the program using method "print()".
 */	
int main(int argc, char *argv[]) {
  llvm_shutdown_obj Y; // Call llvm_shutdown() on exit.

  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <llvm assemble/bitcode file>\n";
    return -1;
  }

  initializeLLVM(argc, argv);
  legacy::PassManager pm;

  auto Mod = getModuleFromFile(argv[1]);

  // Needs to be a pointer because PassManager will delete it
  Cxt001Pass *pass = new Cxt001Pass();
  pm.add(pass);
  pm.run(*Mod);

  pass->print(llvm::dbgs(), Mod.get());

  return 0;
}
