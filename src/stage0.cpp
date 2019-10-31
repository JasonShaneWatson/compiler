#include <iostream>
#include <iomanip>
#include "stage0.h"
#include "stage0main.C"
using namespace std; 

void createListingHeader()
{
  cout<< "STAGE0:" << names << ctime(&currentT) << "\n";
	cout << "LINE NO:" << setw(14) << "SOURCE STATEMENT";
}

void parser()
{
  nextChar();
  //charac must be initialized to the first character of the source file
  if(nextToken() != "program")
       error("keyword 'program' expected");
    //a call to NextToken() has two effects
    // (1) the variable, token, is assigned the value of the next token
    // (2) the next token is read from the source file in order to make
    //     the assignment. The value returned by NextToken() is also
    //     the next token.
  prog();
    //parser implements the grammar rules, calling first rule
}

void createListingTrailer()
{
  cout << "COMPILATION TERMINATED      0 ERRORS ENCOUNTERED";
}

void printSymbolTable()
{

}

bool Key_Id(string token)
{
	string key[] = {"Program", "begin", "end", "var", "conts", "integer", "boolean", "true", "false", "not"};
	
	for (uint i = 0; i <= sizeof(key); i++)
	{
		if (key[i] == token)
		{
			//error
			return false;
		}
	}
	
	return true;
}

void prog()
{
  if (token != "program")
  {
      error("keyword 'program' expected");
  }

  progStmt();
  
  if(token == "const") { consts(); }
  if (token == "var")  { vars();   }
  if (token != "begin")
  {
    error("keyword 'begin' expected");
  }
 
  beginEndStmt();
  
  if (token[0] != END_OF_FILE)
  {
    error("no text may follow end");
  } 
}

void progStmt()
{
	string token;
	if (token != "program")
	{
		//error
	}
	string x = nextToken();
  string NON_KEY_ID; // this should be a function or something to test. maybe the if below could be an attempt to insert into map. 
	if (token != NON_KEY_ID)
	{
		//error
	}
	if (x != ";")
	{
		//error
	}
	nextToken();
	//insert(x, PROG_NAME, CONSTANT, x, NO, 0);
}

//example from class:
//Insert("a,b","integer",VARIABLE,"",YES,1)
//insert should split apart the list like "a,b" on the commas. there is a function that can do this "stringtok"
//this function will return the position of the next comma. then we can take a substring from current position to the position
//returned by the stringtok function. so the first would be substr from 0 to 1. that would return 'a' we then want to check if it is a
//multipfly defined symbol. I think we can do by attempting to insert the value into a map. 
//we check for uppercase because eventually we will be assigning the sum / product of two variables to a temp space like T0,T1,T2....
//in another function y needs to be checked to make sure it is the correct type before inserting 
void insert() // we need to add params and stuff
{
//vvvvvvvvv SUDO CODE FROM HANDOUT vvvvvvvvvv
 //void Insert(string externalName,storeType inType, modes inMode, string inValue,allocation inAlloc, int inUnits)
 //
 //create symbol table entry for each identifier in list of external names
 //Multiply inserted names are illegal
 // 
 //  string name;
 //   while (name broken from list of external names and put into name != "")
 //   {
 //     if symbolTable[name] is defined
 //       process error: multiple name definition
 //     else if name is a keyword
 //       process error: illegal use of keyword
 //     else //create table entry
 //     {
 //       if name begins with uppercase then
 //         symbolTable[name]=(name,inType,inMode,inValue,inAlloc,inUnits)
 //       else
 //         symbolTable[name]=(GenInternalName(inType),inType,inMode,inValue,inAlloc,inUnits)
 //     }
}

//in case 'letter' we check if the last character is an underscore but we will aslso need to check if the token has two underscores in a row
string nextToken()
{
  token = "";
    
  return "a token";
}

bool isInt()
{
  for (uint x = 0; x < token.length(); x++)
  {
    if (!isdigit(token[x]))
    {
      return false;
    }
  }
  return true;   
}

char nextChar()
{
  char nextCh;
  static char prevCh;
  static int lineNumber = 0;
  sourceFile.get(nextCh);
  
  //if we reached the end of the source file. Set char to reflect
  if(sourceFile.eof())
  {
    charac = END_OF_FILE; 
  }
  else
  {
    prevCh = charac;
    charac = nextCh;
    //we are on the first line
    if(lineNumber == 0)
    {
      lineNumber++;
      listingFile << setw(5) << lineNumber << '|';
    }
    //need to make sure this is right 
    else if (prevCh == '\n')
    {
      lineNumber++;
      listingFile << setw(5) << lineNumber << '|';
    }
    listingFile << charac;
  }
  return charac; 
}

//TODO
void error( string err)
{
  listingFile << "Error: " << err << "\n";
  sourceFile.close();
  listingFile.close();
  objectFile.close();
  exit(EXIT_FAILURE);// might need to exit another way  
}

void consts()
{

}

void vars()
{

}

void beginEndStmt()
{

}
