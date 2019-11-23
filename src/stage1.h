#ifndef STAGE1_H
#define STAGE1_H

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <ctime>
#include <string>
#include <stack>

using namespace std;

/*
* Constants
*/ 
enum storeType {INTEGER, BOOLEAN, PROG_NAME, UNKNOWN, RESERVED};
string storeTypeString[] = {"INTEGER", "BOOLEAN", "PROG_NAME", "UNKNOWN", "RESERVED"};

enum allocation {YES, NO};
string allocationString[] = {"YES", "NO"};

enum modes {VARIABLE, CONSTANT};
string modesString[] = {"VARIABLE", "CONSTANT"};

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
   int position;
};


stack<string> operatorStk;
stack<string> operandStk;




/*
* variables
*/
string token;
char charac;
int lineNumber = 0;
string oprnd;
string oprtr; 


/*
* prototypes
*/
bool Key_Id(string s);
bool non_Key_Id();
string ids();
string nextToken();
unordered_map<string,entry> symbolTable; //https://www.geeksforgeeks.org/unordered_map-in-cpp-stl/
void createListingHeader();
void createListingTrailer();
void error(string);
void insert(string externalName, storeType inType, modes inMode, string inValue, allocation inAlloc, int inUnits); 
void parser();
void printSymbolTable();
string whichValue(string name);
storeType whichType(string name);

#endif