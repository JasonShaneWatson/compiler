#include "stage1.h"


/*
* variables
*/
static int currentTempNo = -1;
int maxTempNo = 256;
static int currentLabelNo = -1;
int maxLabelNo = 256;

/*
* prototypes
*/
void PushOperator(string);//d
void PushOperand(string);//d
string PopOperand();//d
string PopOperator();//d
void free_Temp();//d
string get_Temp();//d
string get_Label();//d
void ReadStmt();
void WriteStmt();
void EmitAdditionCode(string, string); //d
void EmitSubtractionCode(string, string);//d
void EmitNegationCode(string);//d
void EmitNotCode(string);//d
void EmitMultiplicationCode(string, string);//d
void EmitDivisionCode(string, string);
void EmitModuloCode(string, string);
void EmitAndCode(string, string);//d
void EmitOrCode(string, string);
void EmitEqualsCode(string, string);
void EmitLTCode(string, string);
void EmitGTCode(string, string);
void EmitGTOECode(string, string);
void EmitLTOECode(string, string);
void EmitDNECode(string, string);
void EmitAssignCode(string, string);


void PushOperator(string oprtr)
{
	operatorStk.push(oprtr);
}
void PushOperand(string oprnd)
{	  
    //check if oprnd is a already defined constant 
    auto searchValue = symbolTable.find(oprnd);
    if (searchValue != symbolTable.end())
    {
          operandStk.push(searchValue->second.internalName);
          return;
    } 
    
		for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
		{
			if(y->second.value == oprnd)
			{
				operandStk.push(y->second.internalName);
			}
			else
			{
				insert(genInternalName(whichType(oprnd)),whichType(oprnd),CONSTANT,whichValue(oprnd),YES,1);
			}
		}
}
string PopOperand()
{
	if (operandStk.empty()) {
		error("operator stack underflow");
	}
	else
	{
		string top = operandStk.top();
		operandStk.pop();
		return top;

	}
	return 0;
}
string PopOperator()
{
	if (operatorStk.empty()) {
		error("operator stack underflow");
	}
	else
	{
		string top = operatorStk.top();
		operatorStk.pop();
		return top;
	}
	return 0;
}

void free_Temp()
{
	currentTempNo--;
	if (currentTempNo < -1)
	{
		error(" compiler error, currentTempNo should be > –1");
	}
}

string get_Temp()
{
	 string temp;
	 currentTempNo++;
	 temp = "T" + to_string(currentTempNo);
	 if (currentTempNo > maxTempNo)
	 {
		 insert(temp, UNKNOWN, VARIABLE, "", NO, 1);
		 maxTempNo++;
	 }
	 return temp;
}
string get_Label()
{
	string temp;
	 currentLabelNo++;
	 temp = "L" + to_string(currentLabelNo);
	 if (currentLabelNo > maxLabelNo)
	 {
		 insert(temp, UNKNOWN, VARIABLE, "", NO, 1);
		 maxLabelNo++;
	 }
	 return temp;
}


void EmitAdditionCode(string operand1,string operand2) //add operand1 to operand2
{
	 //check if  data Types are integers
	 for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
			{
				if(y->second.internalName == operand1)
				{
					if ( storeTypeString[y->second.dataType] != "INTEGER")
					{
						error("illegal type");
					}
				}
				else if(y->second.internalName == operand2)
				{
					if (storeTypeString[y->second.dataType] != "INTEGER")
					{
						error("illegal type");
					}
				}
			}
	//Allocate Temp and store it 		
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) == 'T')
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	else if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2
	else if (Areg != operand1 && Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "IDA " << setw(4) <<left << operand1;
		
	}
	// if register has operand1
	else if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IDA " << setw(4) <<left << operand2;
	}
	// if register has operand2
	else if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IDA " << setw(4) <<left << operand1;
	}
	else 
	{
		error("you broke it");
	}
	// if operand 1 was a temp free it
	if (operand1.at(0) == 'T' )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if (operand2.at(0) == 'T' )
	{
		free_Temp();
	}
	//A register == Tn
	Areg = get_Temp();
	// make Tn dataType = INTEGER
	auto tableValue1 = symbolTable.find(Areg);
	if(tableValue1 != symbolTable.end()) //we found an entry in the symbolTable
	{
	  tableValue1->second.dataType = INTEGER;
	 
	}
	PushOperand(Areg);
}

void EmitSubtractionCode(string operand1,string operand2)
{

	 for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
			{
				if(y->second.internalName == operand1)
				{
					if ( storeTypeString[y->second.dataType] != "INTEGER")
					{
						error("illegal type");
					}
				}
				else if(y->second.internalName == operand2)
				{
					if (storeTypeString[y->second.dataType] != "INTEGER")
					{
						error("illegal type");
					}
				}
			}
	//Allocate Temp and store it 		
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) == 'T')
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	else if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2
	else if (Areg != operand1 && Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(4) <<left << operand1;
		
	}
	// if register is not operand 2 then empty reg and load op2
	else if (Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(4) <<left << operand1;
		
	}
	// if register has operand2 then subtract op1
	else if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(4) <<left << operand1;
	}
	// if operand 1 was a temp free it 
	if (operand1.at(0) == 'T' )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if (operand2.at(0) == 'T' )
	{
		free_Temp();
	}
	else 
	{
		error("you broke it");
	}
	//A register == Tn
	Areg = get_Temp();
	// make Tn dataType = INTEGER
	auto tableValue1 = symbolTable.find(Areg);
	if(tableValue1 != symbolTable.end()) //we found an entry in the symbolTable
	{
	  tableValue1->second.dataType = INTEGER;
	 
	}

	PushOperand(Areg);
}

void EmitAndCode(string operand1,string operand2) //"and" operand1 to operand2
{
 	 //check if  data Types are integers
	 for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
			{
				if(y->second.internalName == operand1)
				{
					if ( storeTypeString[y->second.dataType] != "BOOLEAN")
					{
						error("illegal type");
					}
				}
				else if(y->second.internalName == operand2)
				{
					if (storeTypeString[y->second.dataType] != "BOOLEAN")
					{
						error("illegal type");
					}
				}
			}
	//Allocate Temp and store it 		
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) == 'T')
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	else if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2 ld op2 and multiply by op1
	else if (Areg != operand1 && Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand1;
		
	}
	// if register has operand1 multiply by op2
	else if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand2;
	}
	// if register has operand2 multiply by op1
	else if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand1;
	}
	else 
	{
		error("you broke it");
	}
	// if operand 1 was a temp free it
	if (operand1.at(0) == 'T' )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if (operand2.at(0) == 'T' )
	{
		free_Temp();
	}
	//A register == Tn
	Areg = get_Temp();
	// make Tn dataType = INTEGER
	auto tableValue1 = symbolTable.find(Areg);
	if(tableValue1 != symbolTable.end()) //we found an entry in the symbolTable
	{
	  tableValue1->second.dataType = BOOLEAN;
	 
	}
	PushOperand(Areg);
}


void EmitMultiplicationCode(string operand1,string operand2) //multiply operand2 by operand1
{ 
	 //check if  data Types are integers
	 for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
			{
				if(y->second.internalName == operand1)
				{
					if ( storeTypeString[y->second.dataType] != "INTEGER")
					{
						error("illegal type");
					}
				}
				else if(y->second.internalName == operand2)
				{
					if (storeTypeString[y->second.dataType] != "INTEGER")
					{
						error("illegal type");
					}
				}
			}
	//Allocate Temp and store it 		
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) == 'T')
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	else if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2 ld op2 and multiply by op1
	else if (Areg != operand1 && Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand1;
		
	}
	// if register has operand1 multiply by op2
	else if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand2;
	}
	// if register has operand2 multiply by op1
	else if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand1;
	}
	else 
	{
		error("you broke it");
	}
	// if operand 1 was a temp free it
	if (operand1.at(0) == 'T' )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if (operand2.at(0) == 'T' )
	{
		free_Temp();
	}
	//A register == Tn
	Areg = get_Temp();
	// make Tn dataType = INTEGER
	auto tableValue1 = symbolTable.find(Areg);
	if(tableValue1 != symbolTable.end()) //we found an entry in the symbolTable
	{
	  tableValue1->second.dataType = INTEGER;
	 
	}
	PushOperand(Areg);
}

void EmitNegationCode(string operand1)
{
	 	 //check if  data Types are integers
	 for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
			{
				if(y->second.internalName == operand1)
				{
					if ( storeTypeString[y->second.dataType] != "INTEGER")
					{
						error("illegal type");
					}
				}
			}
	//Allocate Temp and store it 		
	if ( !Areg.empty() && Areg != operand1  && Areg.at(0) == 'T')
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	else if ( !Areg.empty() && Areg != operand1  && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has operand1 multiply by op2
	else if (Areg == operand1 )
	{
		auto tableValue2 = symbolTable.find("zero");
		if(tableValue2 == symbolTable.end()) //we did not find an entry in the symbolTable
		{
		  insert("zero", INTEGER, CONSTANT, "0", YES, 1);
		 
		}
		objectFile << setw(4) << "" << setw(2) << "" << "LDA" << setw(4) <<left << "zero";
		objectFile << setw(4) << "" << setw(2) << "" << "ISB" << setw(4) <<left << operand1;
	}
	// if register has operand2 multiply by op1

	else 
	{
		error("you broke it");
	}
	// if operand 1 was a temp free it
	if (operand1.at(0) == 'T' )
	{
		free_Temp();
	}
	//A register == Tn
	Areg = get_Temp();
	// make Tn dataType = INTEGER
	auto tableValue1 = symbolTable.find(Areg);
	if(tableValue1 != symbolTable.end()) //we found an entry in the symbolTable
	{
	  tableValue1->second.dataType = INTEGER;
	}
	PushOperand(Areg);
}

void EmitNotCode(string operand1)
{
	 	 //check if  data Types are integers
	 for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
			{
				if(y->second.internalName == operand1)
				{
					if ( storeTypeString[y->second.dataType] != "BOOLEAN")
					{
						error("illegal type");
					}
				}
			}
	//Allocate Temp and store it 		
	if ( !Areg.empty() && Areg != operand1  && Areg.at(0) == 'T')
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	else if ( !Areg.empty() && Areg != operand1  && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	else if (Areg != operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "LDA" << setw(4) <<left << operand1;
	}
	// if register has operand1 set jumps
	else if (Areg == operand1 )
	{
		auto tableValue2 = symbolTable.find("false");
		if(tableValue2 == symbolTable.end()) //we did not find an entry in the symbolTable
		{
		  insert("false", BOOLEAN, CONSTANT, "0", YES, 1);
		 
		}
		auto tableValue3 = symbolTable.find("true");
		if(tableValue3 == symbolTable.end()) //we did not find an entry in the symbolTable
		{
		  insert("true", BOOLEAN, CONSTANT, "1", YES, 1);
		 
		}
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << left << label;
		objectFile << setw(4) << "" << " not " << operand1 << "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA" << setw(4) <<left << "FALS";
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA" << setw(4) <<left << "TRUE";
	}

	else 
	{
		error("you broke it");
	}
	// if operand 1 was a temp free it
	if (operand1.at(0) == 'T' )
	{
		free_Temp();
	}
	//A register == Tn
	Areg = get_Temp();
	// make Tn dataType = INTEGER
	auto tableValue1 = symbolTable.find(Areg);
	if(tableValue1 != symbolTable.end()) //we found an entry in the symbolTable
	{
	  tableValue1->second.dataType = INTEGER;
	}
	PushOperand(Areg);
}



 
/*
void Code(string oprtr, string operand1, string operand2) 
{
	operand2 = operand2.substr(0,15);
	operand1 = operand1.substr(0,15);
	
	if (oprtr == "program")
	{
		for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
			{
				if(y->second.internalName == "P0")
				{
					string x = y->second.externalName;
					objectFile << "STRT  NOP" << setw(10) << "" << x << names << endl;
				}
			}
			
	}
	else if (oprtr == "end")
	{
		printSymbolTable(); // we need to change this funciton 
	}
	 else if (oprtr == "read")
    {
        ReadStmt();
    }
    else if (oprtr == "write")
    {
        WriteStmt();
    }
    else if (oprtr == "+")
    {
        EmitAdditionCode(operand1, operand2);
    }
    else if (oprtr == "-")
    {
        EmitSubtractionCode(operand1, operand2);
    }
    else if (oprtr == "neg")
    {
        EmitNegationCode(operand1);
    }
    else if (oprtr == "not")
    {
        EmitNotCode(operand1);
    }
    else if (oprtr == "*")
    {
        EmitMultiplicationCode(operand1, operand2);
    }
    else if (oprtr == "div")
    {
        EmitDivisionCode(operand1, operand2);
    }
    else if (oprtr == "mod")
    {
        EmitModuloCode(operand1, operand2);
    }
    else if (oprtr == "and")
    {
        EmitAndCode(operand1, operand2);
    }
    else if (oprtr == "or")
    {
        EmitOrCode(operand1, operand2);
    }
    else if (oprtr == "=")
    {
        EmitEqualsCode(operand1, operand2);
    }
    else if (oprtr == ":=")
    {
        EmitAssignCode(operand1, operand2);
    }
    else if (oprtr == "<")
    {
        EmitLTCode(operand1, operand2);
    }
    else if (oprtr == "<=")
    {
        EmitLTOECode(operand1, operand2);
    }
    else if (oprtr == ">")
    {
        EmitGTCode(operand1, operand2);
    }
    else if (oprtr == ">=")
    {
        EmitGTOECode(operand1, operand2);
    }
    else if (oprtr == "<>")
    {
        EmitDNECode(operand1, operand2);
    }
	else
	{
		error("undefined operation");
	}	

}
*/


