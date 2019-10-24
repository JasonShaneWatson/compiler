#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <stage0.h>

using namespace std; 

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

const string names = "Aspen Wills, Jason Watson";
const int MAX_SYMBOL_TABLE_SIZE = 256;
const char END_OF_FILE = ’$’; // arbitrary choice
const time_t currentT = time (NULL);

enum storeType {INTEGER, BOOLEAN, PROG_NAME, UNKNOWN};
enum allocation {YES, NO};
enum modes {VARIABLE, CONSTANT};


vector<entry> symbolTable;
ifstream sourceFile;
ofstream listingFile,objectFile;
string ids();
string token;
char charac;

//this program is the stage0 compiler for Pascallite. It will accept
//input from argv[1], generating a listing to argv[2], and object code to
//argv[3]
int main(int argc, char **argv)
{

  //make sure we have the correct # of arguments
  if(argc != 4)
  {
    cerr<< "Usage: " << *argv << " <input file> <listing file> < object file>" << endl;
    exit(EXIT_FAILURE); 
  }

  sourceFile.open(argv[1]);
  //make sure we were able to open the file 
  if(!sourceFile)
  {
    cerr<<"Unable to open file:" << argv[1] << endl;
    exit(EXIT_FAILURE);
  }

  createListingHeader();
  parser();
  createListingTrailer();
  printSymbolTable();

  sourceFile.close();
  return 0;
}

void createListingHeader()
{
  cout<< "STAGE0:" << names << ctime(&currentT) << "\n";
	cout << "LINE NO:" << setw(14) << "SOURCE STATEMENT";
}
