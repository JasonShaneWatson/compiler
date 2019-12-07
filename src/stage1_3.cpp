#include "stage2.h"
#include <cstring> 
bool add_Lev_OP();
bool mult_Lev_OP();
bool rel_OP();
void assignStmt();
void ifStmt();
void repeatStmt();
void whileStmt();
void nullStmt();
void execStmt();
void execStmts();
void express();
void expresses();
void factor();
void factors();
void outputCode(string code, string value, string codeDescription);
void part();
void elsePt();
void term();
void terms();

void execStmts() 
{
  //cout  << setw(14) << left << "\nexecStmts," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;

  nextToken();
  if (token == "else")
  {
	  error("non-keyword identifier, \"read\", \"write\", \"if\", \"while\", \"repeat\", \";\", or \"begin\" expected");
  }
  if( token == "until")
    return;
  execStmt();
 
  //recurse until token == end
  if(token == "end")
  {
	beginNo -= 1;
	return;
  }
  else
  {
	execStmts(); 
  }

}

void execStmt()
{
	//cout << "HELLO" << token << endl;
  //cout  << setw(14) << left << "\nexecStmt," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
  if(token == "read")
  {
    readStmt();
  }
  else if(token == "begin")
  {
    beginEndStmt();
  }
  else if(token == "end") 
  {

    return;
  } 
  else if(token == "write")
  {
    writeStmt();
  }
  else if(token == "if") 
  {
    ifStmt();
  }
  else if(token == "while")
  {
    whileStmt();
  }
  else if(token == "repeat")
  {
    repeatStmt();
  }
  else if(token == ";")
  {
    nullStmt();
  }
  else if (non_Key_Id())
  {
    assignStmt();
  }
}

void readStmt()
{
  //cout << setw(13) << left << "\nreadStmt," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
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

  outputCode("RDI",listOfIDs, "read");

  if(token != ")")
  {
    error("expected ')'");
  }
  nextToken();
	//cout << token;
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

  outputCode("PRI",listOfIDs, "write");

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
  //cout  << setw(14) << left << "\nassignStmt," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
  //make sure token is valid non reserved key  
  if(non_Key_Id())
  {  
    PushOperand(token);
  }
  nextToken();

  //next token should be ":="
  if(token != ":=")
  {
    error("expected \":=\"");
  }

  PushOperator(token);  
  nextToken();
  express();

  if(token != ";")
  {
    error("expected ';' after Assignment statement");
  }
  //cout << "calling code 2" << endl;
  auto temp1 = PopOperand();
  auto temp2 = PopOperand();
  code(PopOperator(), temp1, temp2);
}
void ifStmt()
{
	//cout  << setw(14) << left << "\nifStmt," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
	if (token != "if") 
	{
		error( "'if' was expected");
	}
	nextToken();
	express();
	if (token != "then") 
	{
		error("'then' was expected");
	}
	string operand1 = PopOperand();
	string operand2 = getExternalName(operand1);
	if (whichType(operand2) != 1)
	{
		error( "predicate of if statement must be boolean valued");
	}
	
	code("then", operand1, "");
	nextToken();
	if (token == "else")
	{
		error("then clause may not be empty");
	}
	execStmt();
	if (token == ";") 
	{
		nextToken();
	}
	if (token == "else")
	{
		elsePt();
	}
	else{
	code("post_if", PopOperand(), "");
	execStmt();
	}
}

void elsePt()
{
	if (token != "else") 
	{
		error("else was expected");
	}
	code("else", PopOperand(), "");
	nextToken();
	execStmt();
	code("post_if", PopOperand(), "");
	
}
void repeatStmt()
{
		//cout  << setw(14) << left << "\nrepeatStmt," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
	if (token != "repeat") 
	{
		error( "'repeat' was expected");
	}
	code("repeat", "", "");
	execStmts();
	if (token != "until") 
	{
		error("'until' was expected");
	}
	
	nextToken();
	
	//cout << "HELLO" << token;
	
	express();
	
	string operand2 = PopOperand();
	string operand1 = PopOperand();
	code("until", operand1, operand2);

	//nextToken();
	
	
}
void whileStmt()
{
			//cout  << setw(14) << left << "\nwhileStmt," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
	if (token != "while") 
	{
		error( "'while' was expected");
	}
	code("while", "", "");
	nextToken();
	express();
	if (token != "do") 
	{
		error("'do' was expected");
	}
		string operand1 = PopOperand();
	string operand2 = getExternalName(operand1);
	if (whichType(operand2) != 1)
	{
		error( "predicate of while statement must be boolean valued");
	}
	code("do", operand1, "");
	nextToken();
	execStmt();
	code("post_while", PopOperand(), PopOperand());
	//nextToken();
	
}
void nullStmt()
{
	if (token != ";") {
		error("semicolon was expected");
	}
}

//express = term -> expresses
void express()
{
  ////cout  << setw(14) << left << "\nexpress," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
  term();
  expresses();
}

//term = factor -> terms 
void term()
{
  ////cout  << setw(14) << left << "\nterm," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
  factor();//$
  terms();
}

//expresses = rel_OP -> term || epsilon 
void expresses()
{
  ////cout  << setw(14) << left << "\nexpresses," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
  if(rel_OP())
  {
    PushOperator(token);
    nextToken();
    term();
    ////cout << "calling code 3" << endl;
    auto temp1 = PopOperand();
    auto temp2 = PopOperand();
    code(PopOperator(), temp1, temp2);
  }
}

//factor = part -> factors 
void factor()
{
   ////cout  << setw(14) << left << "\nfactor," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
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
  ////cout  << setw(14) << left << "\npart," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
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
      ////cout << "calling code 4" << endl;
      code("not", PopOperand(), "not");
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
		PushOperand(token);
      code("not", PopOperand(), "not");
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
	else if( token == "+")
	{
		error ("integer, literal, or non keyword identifier expected");
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
      code("neg", PopOperand(), "neg");
    }
    else if(isInt())
    
    {
      PushOperand("-" + token);
      nextToken();
    }
    
    else if(non_Key_Id())
    {
	PushOperand(token);
      code("neg", PopOperand(), "neg");
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
    {
      nextToken();
    }
  }
  else if(isInt()|| non_Key_Id() || token == "true" || token == "false")
  {
    PushOperand(token);
    nextToken();
	/*if(operandStk.size() == 2 && token == ";")
	{
		code(PopOperator(), PopOperand(), PopOperand());
	}
	*/

  }
  else
  {
    error("Illegal keyword");
  }
}  

//factors = mult_Lev_OP -> part -> factors || epsilon 
void factors()
{
  ////cout  << setw(14) << left << "\nfactors," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
  if(mult_Lev_OP())
  {
    PushOperator(token);
    nextToken();
    part();
    ////cout << "calling code 5" << endl;
    auto temp1 = PopOperand();
    auto temp2 = PopOperand();
    code(PopOperator(), temp1, temp2);
    factors();
  }  
}

//terms = add_Lev_OP -> factor -> terms || epsilon 
void terms()
{
  ////cout  << setw(14) << left << "\nterms," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
  if(add_Lev_OP())
  {
    PushOperator(token);
    nextToken();
    factor();
    ////cout << "calling code 1" << endl;
    auto temp1 = PopOperand();
    auto temp2 = PopOperand();
    code(PopOperator(), temp1, temp2);
    terms();
  }
}

// will write code of passed params. 
void outputCode(string code, string value, string codeDescription)
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
	  valueToken = valueToken.substr(0,15);
    // If value not found in map iterator to end is returned
    // we cannot emit a code for a key that hasn't been inserted into table
    auto searchValue = symbolTable.find(valueToken);
    if (searchValue == symbolTable.end())
    {
      error("unknown symbol \"" + valueToken + "\"");
    } 
    else // we found the symbol in the symbol table. 
    {
      objectFile << "      " << code << " " << searchValue->second.internalName << "        " << codeDescription << "(" << valueToken << ")" << endl;
    }
    token_in_string = strtok(NULL, ",");
  }
}
//is token a relational operator?
bool rel_OP()
{
  ////cout  << setw(14) << left << "\nrel_OP," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
  return (token == "=" || token == "<" || token == ">" || token == "<=" || token == ">=" || token == "<>");
}

bool add_Lev_OP()
{
  ////cout  << setw(14) << left << "\nadd_Lev_OP," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
  return (token == "+" || token == "-" || token == "or");
}

bool mult_Lev_OP()
{
  ////cout  << setw(14) << left << "\nmult_Lev_OP," << setw(17) << "token == " + token << setw(13) << "Areg == " + Areg  << endl;
  return (token == "*" || token == "div" || token == "mod" || token == "and");
}
