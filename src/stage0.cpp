#include <iostream>
#include <iomanip>
#include <stage0.h>


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
bool Key_Id(string s)
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
	string x;
	if (token != "program")
	{
		//error
	}
	x = nextToken();
	if (token != NON_KEY_ID)
	{
		//error
	}
	if (nextToken() != ';')
	{
		//error
	}
	nextToken();
	insert(x, PROG_NAME, CONSTANT, x, NO, 0);
	
}
