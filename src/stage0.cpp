#include "stage0.h"
#include "stage0main.C"
#include <unordered_map>

void createListingHeader()
{
  cout<< "STAGE0:" << names << ctime(&currentT) << "\n";
	cout << "LINE NO:" << setw(14) << "SOURCE STATEMENT";
}

void parser()
{
  //uncomment below code if we want to reserve keywords in our symbol table 
  /*entry reserved;
  symbolTable["program"] = reserved;
  symbolTable["begin"] = reserved;
  symbolTable["end"] = reserved;
  symbolTable["var"] = reserved;
  symbolTable["conts"] = reserved;
  symbolTable["boolean"] = reserved;
  symbolTable["true"] = reserved;
  symbolTable["false"] = reserved;
  symbolTable["not"] = reserved;
  */
  
  //charac must be initialized to the first character of the source file
  nextChar();
  
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

//check if string is a reserved keyword 
bool Key_Id(string token)
{
  return (token == "program" || token == "begin" || token == "end" || 
          token == "var" || token == "const" || token == "integer" || 
          token == "boolean" || token == "true" || token == "false" || token == "not");
}

//check if token is already a non key id
bool non_Key_Id()
{
	// If key not found in map iterator to end is returned 
  if (symbolTable.find(token)!= symbolTable.end())//we found a key, so return true
	{
<<<<<<< HEAD
    return true; 
  }
=======
		if (key[i] == token)
		{
			//error
			return true;
		}
	}
>>>>>>> 934c36691ed8e6f2cd3144bf0b213a5dab06f212
	
	return false;
}

void prog()
{
  if (token != "program")
  {
      error("keyword 'program' expected");
  }

  progStmt();
  
  if (token == "const") { consts(); }
  if (token == "var"  ) { vars();   }
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
	if (token != "program")
	{
		error("keyword 'program' expected in first");
	}

  string x = nextToken();
	// If key not found in map iterator to end is returned 
  if (symbolTable.find(token)!= symbolTable.end())//we found a key, we want token to be a a new key so error out 
	{
		error("program name expected");
	}
  
  nextToken();
	if (token != ";")
	{
		error("expected ';' after program name");
	}
	nextToken();
	insert(x, PROG_NAME, CONSTANT, x, NO, 0);
}

//example from class:
//Insert("a,b","integer",VARIABLE,"",YES,1)
//insert should split apart the list like "a,b" on the commas. there is a function that can do this "stringtok"
//this function will return the position of the next comma. then we can take a substring from current position to the position
//returned by the stringtok function. so the first would be substr from 0 to 1. that would return 'a' we then want to check if it is a
//multipfly defined symbol. I think we can do by attempting to insert the value into a map. 
//we check for uppercase because eventually we will be assigning the sum / product of two variables to a temp space like T0,T1,T2....
//in another function y needs to be checked to make sure it is the correct type before inserting 
void insert(string externalName, storeType inType, modes inMode, string inValue, allocation inAlloc, int inUnits)
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

//get next token and store in global variable 'token'
string nextToken()
{
  token = "";
  while(token == "")
  {
    if(charac == '{')// comment section starting
    {  
      nextChar();
      while(charac != '}')//ignore everything until we find }
      {
        if(charac == END_OF_FILE)
        {
          error("unexpected end of file");
        }
        else
        {
          nextChar();
        }
      }
      nextChar();
      cout << "breaking out of {" << endl;
    }
    
    else if (charac == '}')
    {
      error("'}' cannot begin token");
    }
    
    else if (charac == '_')
    {
      error("'_' cannot start an identifier");
    }
    
    else if (isspace(charac))
    {
      nextChar();
    }
    
    else if( 
              charac == '=' ||
              charac == ':' ||
              charac == ',' ||
              charac == ';' ||
              charac == '.' ||
              charac == '+' ||
              charac == '-'
            )
    {
      token = charac;
      nextChar();
    }
    
    else if(isalpha(charac))
    {
      token = charac;
      nextChar();
      while(islower(charac) || isdigit(charac) || charac == '_')
      {
        if(token.back()  == '_')
        {
          error(("illegal use of consecutive underscore: " + token + '_'));
        }
        token += charac;
        nextChar();
      }
      if(token.back()  == '_')
      {
        error(("'_' cannot end token: " + token));
      }
    }
    
    else if (isdigit(charac))
    {
      token = charac;
      nextChar();
      while(isdigit(charac))
      {
        token += charac;
        nextChar();
      }
    }
    
    else if(charac == END_OF_FILE)
    {
      token = charac;
    }
    
    else
    {
      error(("illegal symbol: " + charac));
    }
  }
  return token;
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
  cout << prevCh << endl;
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
  listingFile << "\n\nError: " << err << "\n";
  sourceFile.close();
  listingFile.close();
  objectFile.close();
  exit(EXIT_FAILURE);// might need to exit another way  
}

void consts()
{
if (token != "const")
{
	error("keyword 'const' expected");
}
nextToken();
if (Key_Id(token))
{
  cout << token << endl;
	error("non-keyword identifier must follow 'const'");
}
constStmts();
}

void constStmts()
{
	string x, y;
	if (Key_Id(token) == true)
	{
		error("non-keyword identifier must follow 'const'");
	}
	x = token;
	if (nextToken() != "=")
	{
		error("'=' expected");
	}
	y = nextToken();
	if ( 
        token != "="     && 
        token != "-"     &&
        token != "+"     &&
        token != "not"   &&
        token != "true"  && 
        token != "false" &&
        !non_Key_Id()    &&
        !isInt()
      )
	{
		if (Key_Id(token) == true)
		error(("illegal type with value '" + token + "' follows ':'"));
	}
	
	if ( y == "+" || y == "-")
	{
		if (nextToken() != "INTEGER")
		{
			error("illegal type follows '+' or '-'");
		}
		y = y + token;
	}
	if ( y == "not")
	{
		if (nextToken() != "BOOLEAN")
		{
			error("illegal type follows '='");
		}
		if (token == "true")
		{
			y = "false";
		}
		if (token == "false")
		{
			y = "true";
		}
	}
	if (nextToken() != ";")
	{
		error("semicolon expected");
	}
	//insert(x, WhichType(y), CONSTANT, WhichValue(y), YES, 1);
	if (nextToken() != "begin" || "var")
	{
		error("'begin' or 'var' expected");
	}
	else if (Key_Id(token) == true)
	{
		error("Non-Key-Identifier expected");	
	}
	if (Key_Id(token)== false)
		constStmts();
}

void vars()
{
	if (token != "var")
	{
		error("keyword 'var' expected");
	}
	if (Key_Id(token)== true)
	{
		error("non-keyword identifier expected");
	}
	varStmts();
}
void varStmts()
{
	string x, y;
	if (Key_Id(token) == true)
	{
		error("non-keyword identifier expected");
	}
	x = ids();
	if(token != ":")
	{
		error("':' expected");
	}
  //input from program is lowercased 
	if (nextToken() != "integer" || "boolean")
	{
		error("illegal type follows ':'");
	}
	y = token;
	if (nextToken() != ";")
	{
		error("semicolon expected");
	}
	
  //insert token into symbolTable wtih associated storeType
  if (y == "integer")
  {
    insert(x, INTEGER, VARIABLE, "", YES, 1);
	}
  else if (y == "boolean")
	{	
    insert(x, BOOLEAN, VARIABLE, "", YES, 1);
  }
  ////////////////////////////////
	
  if (nextToken() != "begin")
	{
		if (Key_Id(token) == true)
		{
			error("'begin' or non Key identifier expected");
		}
	}
	if (Key_Id(token) == false)
	{
		varStmts();
	}
}

string ids()
{
	string temp,tempString;

	if (Key_Id(token) == true)
	{
		Error("process error: non-keyword identifier expected");
	}
	tempString = token;
	temp = token;

	NextToken();
	if (Key_Id(token) == true)
	{
		Error("process error: non-keyword identifier expected");
	}
	tempString = temp + "," + Ids();
	return tempString;
}

void beginEndStmt()
{
	if (token != "begin") 
	{
		error("'begin' expected");
	}

	if (nextToken() != "end") 
	{
	error("'end' expected");
	}

	if (nextToken() != ".") 
	{
		error("'.' expected");
	}
	nextToken();	
}

/*

string genInternalName(storeType inType){
	static int internalNI = 0;
	static int internalNB = 0;
	static bool newProg = true;
    string I = ""; 
	
	if(inType == INTEGER){
		I += "I";
		string a = tostring(internalNI);
		I += a ;
		internalI++;
	}else if(inType == BOOLEAN){
		I += "I";
		string b = tostring(internalBI);
		string I += b ;
		internalB++;
	}else if (inType == PROG_NAME){
		if (progFound == true){
			I += "P0";
			progFound = false;
		}else{
			Error("Can not have more than one program name");
		}
	}
	return I;
}

*/

