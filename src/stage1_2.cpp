#include "stage1.h"


/*
* variables
*/
static int currentTempNo = 0;
int maxTempNo = 256;


/*
* prototypes
*/
void PushOperator(string);
void PushOperand(string);
string PopOperand();
string PopOperator();
void free_Temp();
string get_Temp();
void EmitAdditionCode(string operand1,string operand2);


void PushOperator(string oprtr)
{
	operatorStk.push(oprtr);
}
void PushOperand(string oprnd)
{	
		for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
		{
			if(y->second.value == oprnd)
			{
				operandStk.push(y->second.internalName);
			}
			else
			{
				insert(oprnd,whichType(oprnd),CONSTANT,whichValue(oprnd),YES,1);
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
	if ( !Areg.empty() && (Areg != operand1 || Areg != operand2) && Areg.at(0) == 'T')
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue == symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	if ( !Areg.empty() && (Areg != operand1 || Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2
	if (Areg != operand1 || Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "IDA " << setw(4) <<left << operand1;
		
	}
	// if register has operand1
	if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IDA " << setw(4) <<left << operand2;
	}
	// if register has operand2
	if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IDA " << setw(4) <<left << operand1;
	}
	// if register has operand1
	if (operand1.at(0) == 'T' )
	{
		free_Temp();
	}
	// if register has operand2
	if (operand2.at(0) == 'T' )
	{
		free_Temp();
	}
	//A register == Tn
	Areg = get_Temp();
	// make Tn dataType = INTEGER
	auto tableValue1 = symbolTable.find(Areg);
	if(tableValue1 == symbolTable.end()) //we found an entry in the symbolTable
	{
	  tableValue1->second.dataType = INTEGER;
	 
	}
}