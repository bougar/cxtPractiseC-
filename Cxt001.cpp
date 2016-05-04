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
 * 1. Number of instructions per function 
 * 2. Number of floating-poing instructions per function
 * 3. Size of memory allocated dynamically (e.g. malloced/new'ed) per function
 * ==>> Report as a table: function, kpi1, kpi2, ...
 *
 * 4. Report list of variables with matching malloc-free, per function
 *    - Report as a table: function name, LOCs, FLOPs, memory consumption
 *    - Interprocedural analysis of the code of the function, as well as the code of called functions 
 *    - Design tests in C/C++ (check functionality)
 *    - Internal run&test with your tests and with FOR, FOR2 & LULESH (check robustness)
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
  Cxt001Pass *pass = new Cxt001Pass(Mod.get());
  pm.add(pass);
  pm.run(*Mod);

  pass->print(llvm::dbgs(), Mod.get());
	pass->printTotals();

  return 0;
}
