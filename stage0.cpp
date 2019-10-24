#include <iostream>
#include <iomanip>
#include <ctime>





using namespace std; 


const int MAX_SYMBOL_TABLE_SIZE = 256;
enum storeType {INTEGER, BOOLEAN, PROG_NAME, UNKNOWN};
enum allocation {YES, NO};
enum modes {VARIABLE, CONSTANT};

const string names = "Aspen Wills, Jason Watson";

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

vector<entry> symbolTable;
ifstream sourceFile;
ofstream listingFile,objectFile;
string token;
char charac;
const char END_OF_FILE = ’$’; // arbitrary choice


const time_t currentT = time (NULL);
string token;

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


int main(int argc, char **argv)
{
 //this program is the stage0 compiler for Pascallite. It will accept
 //input from argv[1], generating a listing to argv[2], and object code to
 //argv[3]
 //
 //if(argc != 4)
 //{
 // cerr<< "Usage: " << *argv << " <input file> <listing file> < object file>" << endl;
 // exit(EXIT_FAILURE); 
 //}
 createListingHeader();
 parser();
 createListingTrailer();

 printSymbolTable();

 return 0;
}

void createListingHeader()
{
   cout<< "STAGE0:" << names << ctime(&currentT) << "\n";
	cout << "LINE NO:" << setw(14) << "SOURCE STATEMENT";
}
