#ifndef STAGE0_H
#define STAGE0_H

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <ctime>

using namespace std;

enum storeType {INTEGER, BOOLEAN, PROG_NAME, UNKNOWN, RESERVED};
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
//https://www.geeksforgeeks.org/unordered_map-in-cpp-stl/  << unordered_map description 
unordered_map<string,entry> symbolTable;
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
void insert(string externalName, storeType inType, modes inMode, string inValue, allocation inAlloc, int inUnits); 
char nextChar();
string nextToken();
bool isInt();
<<<<<<< HEAD
string ids();
string token;
char charac;
=======
string genInternalName(storeType);
>>>>>>> 934c36691ed8e6f2cd3144bf0b213a5dab06f212

#endif
