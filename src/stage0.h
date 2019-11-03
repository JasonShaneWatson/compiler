#ifndef STAGE0_H
#define STAGE0_H


#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;



enum storeType {INTEGER, BOOLEAN, PROG_NAME, UNKNOWN};
enum allocation {YES, NO};
enum modes {VARIABLE, CONSTANT};

const string names = "Aspen Wills, Jason Watson";
const int MAX_SYMBOL_TABLE_SIZE = 256;
const char END_OF_FILE = '$'; // arbitrary choice
const time_t currentT = time (NULL);


struct entry
{
   string internalName;
   string externalName;
   storeType dataType;
   modes mode;
   string value;
   allocation alloc;
   int units;
};


bool Key_Id(string s);
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
void error(string);
string ids();
void beginEndStmt();
void insert(string externalName, string type, modes inMode, string inValue, allocation inAlloc, int inUnits); 
char nextChar();
string nextToken();
bool isInt();
string genInternalName(storeType);






#endif
