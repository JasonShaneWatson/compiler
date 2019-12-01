#include "stage1.h"
#include "stage1_2.cpp"
#include "stage0main.C"
#include "exec_stmts.cpp"
#include <unordered_map>
#include <cstring> 

bool isInt();
char nextChar();
string genInternalName(storeType);
void beginEndStmt();
void consts();
void constStmts();
void prog();
void progStmt();
void vars();
void varStmts();

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
       error("keyword \"program\" expected");
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
  objectFile << setw(4) << "" << setw(2) << "" << "HLT" << endl;
  int currentElement = 0;
  for (auto x = symbolTable.cbegin(); x != symbolTable.cend(); ++x) 
  {
    for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
    {
      if(y->second.position == currentElement)
      {
        if(y->second.units == 1 && allocationString[y->second.alloc] == "YES")
        {
          //Internal Name
          objectFile << left << setw(6) << y->second.internalName << 
          //DEC or BSS
          setw(4) << ((modesString[y->second.mode] == "VARIABLE") ? "BSS" : "DEC") <<
          //value
          setw(4) << setfill('0') << right << y->second.value << setfill(' ') <<
          //padding
          setw(6) << left << "" <<
          //ExternalName          
          setw(15) << y->second.externalName << 
          //end the line;
          endl;
        }
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
          token == "boolean" ||  token == "not");
}

//returns true if token is a valid NON_KEY_ID
//this means that token is properly formatted and not a key in the symbol table 
bool non_Key_Id()
{
	// If key not found in map iterator to end is returned 
  //if (symbolTable.find(token)!= symbolTable.end())//we found a key with value of token, return false
    //error("multiple name definition");
  
  //first character of token must be a lowercased leter
  if(isupper(token[0]) || !isalpha(token[0]))
    error("invalid input, token must begin with a lowercased character");
  
  //next token should prevent this from happening, but lets make sure its a good token.
  //capital letters will have already been rejected in nextToken. 
  /*for(uint x = 0; x < token.length(); x++)
    if(!isalnum(token[x]) && token[x] != '_')
      error("invalid character in token \"" + token + "\""); 
		*/
	//if(Key_Id(token))
			//error("reserved keyword '" + token + "' cannot be redefined");
  for(int x = 1; x < (int)token.length(); x++)
  {
    if( !isalpha(token[x]) && !isdigit(token[x]) && token[x] != '_' )
      return false;
  }
      
	return true;
}

void prog()
{
  if (token != "program")
  {
      error("keyword \"program\" expected");
  }

  progStmt();
  
  if (token == "const") { consts(); }
  if (token == "var"  ) { vars();   }
  if (token != "begin")
  {
    error("keyword \"begin\" expected");
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
		error("keyword \"program\" expected in first");
	}

  string x = nextToken();
	// If key not found in map iterator to end is returned 
	//check token to make sure it is a valid non_Key_Id()
	
	for(uint x = 0; x < token.length(); x++)
    if(!isalnum(token[x]) && token[x] != '_')
      error("expected program name");
  
  if(x.length() > 15)
  {
    x = x.substr(0,15);
  }
  
  nextToken();
	if (token != ";")
	{
		error("expected \";\" after program name");
	}
	nextToken();
	insert(x, PROG_NAME, CONSTANT, x, NO, 0);
	objectFile << "STRT  NOP" << setw(10) << "" << x << names << endl;
}

//insert a unique entry into the symbolTable
void insert(string externalName, storeType inType, modes inMode, string inValue, allocation inAlloc, int inUnits)
{
  static int position = 0;
	static bool hasPrgName = false;
	if(symbolTable.size() == 256)
	{
		error("symbol table overflow. exceded 256 entries ");
	}
	
  char name[externalName.size()+1];
  
  // create copy of externalName as a char *
  strcpy(name, externalName.c_str());  
  
  // find first token in string that could consist of multiple values like "a,b,c"
  char *token_in_string = strtok(name,","); 
  
  //loop through the char * and find each individuall value seperated by a ','
  while(token_in_string != NULL)
  {
		if(symbolTable.size() == 256)
		{
			error("symbol table overflow. exceded 256 entries ");
		}
    string nameToken = string(token_in_string);
    //external names are only allowed to be 15 characters long 
    if(nameToken.length() > 15)
    {
      nameToken = nameToken.substr(0,15);
    }
    
    if(symbolTable.find(nameToken) != symbolTable.end())
    {
      error("'" + nameToken + "'" + " is defined multiple times");
    }
    else if (Key_Id(nameToken))
    {
      error("illegal use of keyword \"" + nameToken + "\"");
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
				if(inType == 2 && !hasPrgName)//prg_name
				{
					hasPrgName = true;
					New.internalName = "P0";
				}
				else
				{
					New.internalName = genInternalName(inType);
				}
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
      error("\"}\" cannot begin token");
    }
    
    else if (charac == '_')
    {
      error("\"_\" cannot start an identifier");
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
              charac == '-' ||
              charac == '*' ||
              charac == '(' ||
              charac == ')' ||
              charac == '<' ||
              charac == '>' 
            )
    {
      
      token = charac;
      nextChar();
      
      if((token == ":" || token == ">" ||token == "<") && charac == '=')
      {
        token += charac;
        nextChar();
      }
      else if(token == "<" && charac == '>')
      {
        token += charac;
        nextChar();
      }  
    }
    
    else if(isalpha(charac))
    {
      token += charac;

			
      nextChar();
      while(islower(charac) || isdigit(charac) || charac == '_')
      {
        token += charac;
        if(token.back() == '_')
				{
					nextChar();
					if(charac == '_')
					{
						error(("illegal use of consecutive underscore: " + token + '_'));
					}
					if(!islower(charac) && !isdigit(charac))
					{
							error("token cannot end in an '_'");
					}
					else
					{
						token += charac;
					}
				}
				nextChar();
      }
      if(token.back()  == '_')
      {
        error(("\"_\" cannot end token: " + token));
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
  //static int lineNumber = 0;
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
  listingFile << "\nError: Line " << lineNumber << ": " << err << "\n";
  listingFile << "\nCOMPILATION TERMINATED      1 ERRORS ENCOUNTERED\n";
  sourceFile.close();
  listingFile.close();
  objectFile.close();
  exit(0);// might need to exit another way  
}

void consts()
{
if (token != "const")
{
	error("keyword \"const\" expected");
}
nextToken();
if (Key_Id(token))
{
	error("non-keyword identifier must follow \"const\"");
}
constStmts();
}

void constStmts()
{	
	string x, y;
	
	//check token to make sure it is a valid non_Key_Id()
	non_Key_Id();
	
	x = token;
	if (nextToken() != "=")
	{
		error("'=' expected after " + x);
	}
	y = nextToken();
	if ( 
        token != "="     && 
        token != "-"     &&
        token != "+"     &&
        token != "not"   &&
        token != "true"  && 
        token != "false" &&
        !(symbolTable.find(token) != symbolTable.end())   && // look for a value in symbol table and not the expression. basically !(isInSymbolTable(token))
        !isInt()
      )
	{
		if (Key_Id(token) == true)
		error(("illegal type with value '" + token + "' follows '='"));
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
	else if ( y == "not")
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
	if (token == ";")
	{
		error("expected value for identifier \"" + x + "\" after the '=' sign");
	}
	string z = token;
	if (nextToken() != ";")
	{
		error("semicolon expected after " + z );
	}
	insert(x, whichType(y), CONSTANT, whichValue(y), YES, 1);
	nextToken();
	if (token != "begin" && token != "var")
	{
		for(uint x = 0; x < token.length(); x++)
			if(!isalnum(token[x]) && token[x] != '_')
				error("non-keyword identifier, \"begin\" or \"var\" expected");
	}

	if(token == "var" || token == "begin")
			return;
	if (non_Key_Id())
		constStmts();
}

string whichValue(string name)
{
  if(name == "true")
    return "1";
  else if (name == "false")
    return "0";
  
  bool isLiteral = true;
  if(isdigit(name[0]) || name[0] == '-' || name[0] == '+')
  {
    if(name.length() < 2 && !isdigit(name[0]))
      isLiteral = false;
    
    for(uint x = 1; x < name.length(); x++)
    {
      if(!isdigit(name[x]))
        isLiteral = false;
    }
    if(isLiteral)
      return name;
  }
  else
  {
    auto tableValue = symbolTable.find(name);
    if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
    {
      return tableValue->second.value;
    }
  }
  error("reference to undefined constant " + name);
  return ""; // wont run, just to stop the compiler from complaining  
}

storeType whichType(string name)
{
  if(name == "true" || name == "false")
    return BOOLEAN;
   
  //if name is an INTEGER, check and make sure it is correctly formatted
  if(isdigit(name[0]) || name[0] == '-' || name[0] == '+')
  {
    if(name.length() < 2 && !isdigit(name[0]))
      error("expected a number to follow " + name);
    
    for(uint x = 1; x < name.length(); x++)
    {
      if(!isdigit(name[x]))
        error("invalid character in\"" + name + "\"");
    }
    return INTEGER;
  }
  else
  {
    auto tableValue = symbolTable.find(name);
    if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
    {
      return tableValue->second.dataType;
    }
  }
  error("reference to undefined constant");
  return UNKNOWN; // wont run, just to stop the compiler from complaining 
}

void vars()
{
	if (token != "var")
	{
		error("keyword \"var\" expected");
	}

  nextToken();
	
	//check token to make sure it is a valid non_Key_Id()
	non_Key_Id();
	
	varStmts();
}

void varStmts()
{
	string x, y, z;
	z = token;
	//check token to make sure it is a valid non_Key_Id()
	non_Key_Id();
  
	x = ids();
	if(token != ":")
	{
		error("\":\" expected");
	}
  //input from program is lowercased
  nextToken();
	if (token != "integer" && token != "boolean")
	{
		error("illegal type follows \":\"");
	}
	y = token;
	if (nextToken() != ";")
	{
		error("semicolon expected after " + y);
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
			error("\"begin\" or non Key identifier expected");
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

	//check token to make sure it is a valid non_Key_Id()
	non_Key_Id();
	tempString = token;
	temp = token;
  nextToken();
  if(token == ",")
  {
    nextToken();
    //check token to make sure it is a valid non_Key_Id()
    non_Key_Id();
    tempString = temp + "," + ids();
	}
  return tempString;
}

void beginEndStmt() 
{
	if (token != "begin") 
	{
		error("\"begin\" expected");
	}
  
  execStmts();
  
	if (token != "end") 
	{
	error("\"end\" expected but recieved " + token);
	}

	if (nextToken() != ".") 
	{
		error("\".\" expected after end");
	}
	nextToken();	
}

string genInternalName(storeType inType){

	static int internalNI = 0;
	static int internalNB = 0;

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
  else
  {
		error("data type of token must be INTEGER or BOOLEAN");
	}
	return I;
}
