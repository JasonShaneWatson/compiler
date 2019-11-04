#include "stage0.h"
#include "stage0main.C"
#include <unordered_map>
#include <cstring>

void createListingHeader()
{
  listingFile << "STAGE0:  " << names << "       "<< ctime(&currentT) << "\n";
	listingFile <<  left << setw(22) << "LINE NO." << "SOURCE STATEMENT\n\n" << right;
}

void parser()
{
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
  listingFile << "\nCOMPILATION TERMINATED      0 ERRORS ENCOUNTERED\n";
}

void printSymbolTable()
{
	objectFile << "STAGE0:  " << names << "       " << ctime(&currentT) << "\n";
  int currentElement = 0;
  for (auto x = symbolTable.cbegin(); x != symbolTable.cend(); ++x) 
  {
    for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
    {
      if(y->second.position == currentElement)
      {
        objectFile << left << setw(17) << y->second.externalName;
        objectFile << left << setw(6) << y->second.internalName;
        objectFile << setw(11) << storeTypeString[y->second.dataType];
        objectFile << setw(10) << modesString[y->second.mode];
        objectFile << setw(17) << y->second.value;
        objectFile << setw(5) << allocationString[y->second.alloc];
        objectFile << setw(1) << y->second.units;
        objectFile << "\n";
        currentElement += 1;
      }
    }
  }
}

//check if string is a reserved keyword 
bool Key_Id(string token)
{
  return (token == "program" || token == "begin" || token == "end" || 
          token == "var" || token == "const" || token == "integer" || 
          token == "boolean" || token == "true" || token == "false" || token == "not");
}

//returns true if token is a valid NON_KEY_ID
//this means that token is properly formatted and not a key in the symbol table 
bool non_Key_Id()
{
	// If key not found in map iterator to end is returned 
  if (symbolTable.find(token)!= symbolTable.end())//we found a key with value of token, return false
    return false; 
  
  //first character of token must be a lowercased leter
  if(!islower(token[0]))
    return false;
  
  //next token should prevent this from happening, but lets make sure its a good token.
  //capital letters will have already been rejected in nextToken. 
  for(uint x = 0; x < token.length(); x++)
    if(!isalnum(token[x]) && token[x] != '_')
      return false; 
      
	return true;
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

//insert a unique entry into the symbolTable
void insert(string externalName, storeType inType, modes inMode, string inValue, allocation inAlloc, int inUnits)
{
  static int position = 0; 
  char name[externalName.size()+1];
  
  // create copy of externalName as a char *
  strcpy(name, externalName.c_str());  
  
  // find first token in string that could consist of multiple values like "a,b,c"
  char *token_in_string = strtok(name,","); 
  
  //loop through the char * and find each individuall value seperated by a ','
  while(token_in_string != NULL)
  {
    string nameToken = string(token_in_string);
    //external names are only allowed to be 15 characters long 
    if(nameToken.length() > 15)
    {
      nameToken = nameToken.substr(0,15);
    }
    
    if(symbolTable.find(nameToken) != symbolTable.end())
    {
      error(nameToken + "is defined multiple times");
    }
    else if (Key_Id(nameToken))
    {
      error("illegal use of keyword '" + nameToken + "'");
    }
    else //insert the value
    {
      entry New;
      New.externalName = nameToken;
      if(isupper(nameToken[0]))
      {
        New.internalName = nameToken;
      }
      else
      {
        New.internalName = genInternalName(inType);
      }
      New.dataType = inType;
      New.mode = inMode;
      New.value = inValue;
      New.alloc = inAlloc;
      New.units = inUnits;
      New.position = position;
      position += 1;
      symbolTable[nameToken] = New;
    }
    token_in_string = strtok(NULL, ",");
  }

 /* while (name broken from list of external names and put into name != "")
  {
    if symbolTable[name] is defined
      process error: multiple name definition
    else if name is a keyword
      process error: illegal use of keyword
    else //create table entry
    {
      if name begins with uppercase then
        symbolTable[name]=(name,inType,inMode,inValue,inAlloc,inUnits)
      else
        symbolTable[name]=(GenInternalName(inType),inType,inMode,inValue,inAlloc,inUnits)
    }
  }
	
// I could be way off on this if we do use it we would have to change it to check the map 
// for if the name is used before or not instead of using a symbolTable vector.
    string name;
    auto eNend = externalName.cend();
    for (auto x = externalName.cbegin(); x < externalName.cend(); x++){
        name = ""; //initialize a new name
		for( auto i = externalName.cbegin(); i > eNend; i++)
		{
			if (*i != ',')
			{
				name += *i;
			}
		}
		if(name.size() >15)
		{
			name = name.substr(0,15);
		}
		for ( uint i = 0; i < symbolTable.size(); i++)
		{
			if (symbolTable[i].externalName == name)
			{
				Error("process error: multiple name definition");
			}
			
		}
		if ( Key_Id(name) == true)
			{
				Error("process error: illegal use of keyword");
			}
		entry New;
		New.externalName = name;
		if (isupper(name[0]))
		{
			New.internalName = name;
		}
		else
		{
			New.internalName = genInternalName(inType);
		}	
		New.dataType = inType;
		New.mode = inMode;
		New.value = inValue;
		New.alloc = inAlloc;
		New.units = inUnits;
		
		symbolTable.push_back(New);

*/
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
		nextToken();
		if (!isdigit(token[0]))
		{
			error("illegal type follows '+' or '-'");
		}
		y = y + token;
	}
	if ( y == "not")
	{
		nextToken();
		if (token != "true" && token != "false")
		{
			error("illegal type follows 'not'");
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
	insert(x, WhichType(y), CONSTANT, WhichValue(y), YES, 1);
	nextToken();
  if (token != "begin" && token != "var")
	{
		if(Key_Id(token) == true)
		{
			error("'begin' or 'var' expected");
		}
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
  nextToken();
	if (!non_Key_Id())
	{
		error("non-keyword identifier expected");
	}
	varStmts();
}
void varStmts()
{
	string x, y;
	if (!non_Key_Id())
	{
		error("non-keyword identifier expected");
	}
	x = ids();
	if(token != ":")
	{
		error("':' expected");
	}
  //input from program is lowercased
  nextToken();
	if (token != "integer" && token != "boolean")
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

	if (!non_Key_Id())
	{
		error("non-keyword identifier expected");
	}
	tempString = token;
	temp = token;
  nextToken();
  if(token == ",")
  {
    nextToken();
    if (!non_Key_Id())
    {
      error("non-keyword identifier expected");
    }
    tempString = temp + "," + ids();
	}
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

string genInternalName(storeType inType){
	static int internalNI = 0;
	static int internalNB = 0;
	static bool newProg = true;
    string I = ""; 

	if(inType == INTEGER)
  {
		I += "I";
		string a = to_string(internalNI);
		I += a;
		internalNI++;
	}
  else if(inType == BOOLEAN)
  {
		I += "B";
		string b = to_string(internalNB);
		I += b;
		internalNB++;
	}
  else if(inType == PROG_NAME)
  {
		if(newProg)
    {
			I += "P0";
			newProg = false;
		}
    else
    {
			error("Can not have more than one program name");
		}
	}
	return I;
}