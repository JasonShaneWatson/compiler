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
  return "a token";
}
