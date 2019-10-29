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

string nextToken()
{
  return "a token";
}
