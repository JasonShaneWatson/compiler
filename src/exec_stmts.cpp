#include "stage1.h"
#include <cstring> 
bool add_Lev_OP();
bool mult_Lev_OP();
bool rel_OP();
void assignStmt();
void execStmt();
void execStmts();
void express();
void expresses();
void factor();
void factors();
void outputCode(string code, string value);
void part();
void readStmt();
void term();
void terms();
void writeStmt();

void execStmts() 
{
  nextToken();
  if(token == "end")
    return;
 
  execStmt();
 
  //recurse until token == end
  execStmts();
}

void execStmt()
{
  if(token == "read")
  {
    readStmt();
  }
  else if(token == "write")
  {
    writeStmt();
  }
  else if (non_Key_Id())
  {
    assignStmt();
  }
}

void readStmt()
{
  if(token != "read")
  {
    error("expected \"read\"");
  }
  //move past read and look for a '('
  nextToken();
  if(token != "(")
  {
    error("expected '(' after \"read\"");
  }
  nextToken();
  string listOfIDs = ids();

  outputCode("RDI",listOfIDs);

  if(token != ")")
  {
    error("expected ')'");
  }
  nextToken();

  if(token != ";")
  {
    error("expected ';' after read statement");
  }
}

void writeStmt()
{
  if(token != "write")
  {
    error("expected \"write\"");
  }
  //move past read and look for a '('
  nextToken();
  if(token != "(")
  {
    error("expected '(' after \"write\"");
  }
  nextToken();
  string listOfIDs = ids();

  outputCode("PRI",listOfIDs);

  if(token != ")")
  {
    error("expected ')'");
  }
  nextToken();

  if(token != ";")
  {
    error("expected ';' after write statement");
  }
}

void assignStmt()
{
  //make sure token is valid non reserved key  
  non_Key_Id();

  nextToken();

  //next token should be ":="
  if(token != ":=")
  {
   error("expected \":=\"");
  }
  nextToken();

  express();

}

//express = term -> expresses
void express()
{
  term();
  expresses();
}

//term = factor -> terms 
void term()
{
  factor();//$
  terms();
}

//expresses = rel_OP -> term || epsilon 
void expresses()
{
  if(rel_OP())
  {
    PushOperator(token);
    nextToken();
    term();
    //code(PopOperator(),PopOperand(),PopOperand());
  }
}

//factor = part -> factors 
void factor()
{
  part();
  factors();
}

/*part = 
    '+' || '-' || epsilon ->  
                            '(' -> express -> ')' ||
                            integer               ||
                            non_Key_Id

    or 
    'not' -> 
            '(' -> express -> ')' ||
            integer               ||
            non_Key_Id
    
    or
    boolean
*/   
void part()
{
  if(token == "not")
  {
    nextToken();
    if(token == "(")
    {
      nextToken();
      express();
      if(token !=")")
        error("\")\" expected");
      nextToken();
      outputCode("not", PopOperand());
    }
    else if(token == "true" || token == "false")
    {
      if(token == "true")
      {
        PushOperand("false");
        nextToken();
      }
      else
      {
        PushOperand("true");
        nextToken();
      }
    }
    else if(non_Key_Id())
    {
      outputCode("not", token);
      nextToken();
    }
  }
  else if(token == "+")
  {
    nextToken();
    if(token == "(")
    {
      nextToken();
      express();
      if(token != ")")
      {
        error("\")\" expected");
      }
      nextToken();
    }
    else if(isInt() || non_Key_Id())
    {
      PushOperand(token);
      nextToken();
    }
    else
    {
      error("\"(\", integer, or non-keyword id expected");
    }
  }
  else if(token == "-")
  {
    nextToken();
    if(token == "(")
    {
      nextToken();
      express();
      if(token != ")")
        error("\")\" expected");
      nextToken();
      outputCode("neg", PopOperand());
    }
    else if(isInt())
    {
      PushOperand("-" + token);
      nextToken();
    }
    else if(non_Key_Id())
    {
      outputCode("neg", token);
      nextToken();
    }
  }
  else if(token == "(")
  {
    nextToken();
    express();
    if(token !=")")
    {
      error("\")\" expected");
    }
    else
      nextToken();
  }
  else if(isInt()|| non_Key_Id() || token == "true" || token == "false")
  {
    PushOperand(token);
    nextToken();
  }
  else
  {
    error("Illegal keyword");
  }
}

//factors = mult_Lev_OP -> part -> factors || epsilon 
void factors()
{
  if(mult_Lev_OP())
  {
    PushOperator(token);
    nextToken();
    part();
    //code(PopOperator(),PopOperand(),PopOperand());
    factors();
  }  
}

//terms = add_Lev_OP -> factor -> terms || epsilon 
void terms()
{
  if(add_Lev_OP())
  {
    PushOperator(token);
    nextToken();
    factor();
    //code(PopOperator(),PopOperand(),PopOperand());
    terms();
  }
}

// will write code of passed params. 
void outputCode(string code, string value)
{
  // create copy of value as a char *
  char valueArray[value.size()+1];
  strcpy(valueArray, value.c_str());  
  // find first token in string that could consist of multiple values like "a,b,c"
  char *token_in_string = strtok(valueArray,","); 
  //loop through the char * and find each individuall value seperated by a ','
  while(token_in_string != NULL)
  {
    string valueToken = string(token_in_string);
    // If value not found in map iterator to end is returned
    // we cannot emit a code for a key that hasn't been inserted into table
    auto searchValue = symbolTable.find(valueToken);
    if (searchValue == symbolTable.end())
    {
      error("unknown symbol \"" + valueToken + "\"");
    } 
    else // we found the symbol in the symbol table. 
    {
      objectFile << "      " << code << " " << searchValue->second.internalName << "        " << "read(" << valueToken << ")" << endl;
    }
    token_in_string = strtok(NULL, ",");
  }
}

//is token a relational operator?
bool rel_OP()
{
  return (token == "=" || token == "<" || token == ">" || token == "<=" || token == ">=" || token == "<>");
}

bool add_Lev_OP()
{
  return (token == "+" || token == "-" || token == "or");
}

bool mult_Lev_OP()
{
  return (token == "*" || token == "div" || token == "mod" || token == "and");
}
