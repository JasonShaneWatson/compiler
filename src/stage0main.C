#include <iostream>
#include <iomanip>
#include <vector>
#include "stage0.h"
#include <fstream>

using namespace std;

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
