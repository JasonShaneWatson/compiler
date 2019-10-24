#ifndef STAGE0_H
#define STAGE0_H


#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;


void createListingHeader();
void parser();
void createListingTrailer();
void printSymbolTable();
void prog();
void progStmt();
void consts();
void constStmts();
void vars();
void varStmts();
void initKeywords();
string ids();
void beginEndStmt();
void insert(string externalName, storeType inType, modes inMode, string inValue, allocation inAlloc, int inUnits);







#endif
