#pragma once
#include <string>

std::string intervalCounter();
void dumpInstance( std::string sOutFile, bool verbose = true );
std::string simplifyLProgram();
std::string prepareFields();
std::string replicateIDX();
std::string convertReferences();