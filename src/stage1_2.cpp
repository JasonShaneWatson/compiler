#include "stage1.h"


/*
* variables
*/


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
	
	else{
    
		for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
		{
			if(y->second.value == oprnd)
			{
				operandStk.push(y->second.internalName);
				return;
			}
			else
			{	insert(oprnd,whichType(oprnd),CONSTANT,whichValue(oprnd),YES,1);
				auto searchValue1 = symbolTable.find(oprnd);
				if (searchValue1 != symbolTable.end())
				{
					  operandStk.push(searchValue1->second.internalName);
					  return;
				}
			}
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
		 //printSymbolTable();
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
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2 << "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IAD " << setw(4) <<left << operand1<< "\n";
		
	}
	// if register has operand1
	else if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IAD " << setw(4) <<left << operand2<< "\n";
	}
	// if register has operand2
	else if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IAD " << setw(4) <<left << operand1<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(4) <<left << operand1<< "\n";
		
	}
	// if register is not operand 2 then empty reg and load op2
	else if (Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(4) <<left << operand1<< "\n";
		
	}
	// if register has operand2 then subtract op1
	else if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(4) <<left << operand1<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand1<< "\n";
		
	}
	// if register has operand1 multiply by op2
	else if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand2<< "\n";
	}
	// if register has operand2 multiply by op1
	else if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand1<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand1<< "\n";
		
	}
	// if register has operand1 multiply by op2
	else if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand2<< "\n";
	}
	// if register has operand2 multiply by op1
	else if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(4) <<left << operand1<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "LDA" << setw(4) <<left << "zero"<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "ISB" << setw(4) <<left << operand1<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg<< "\n";
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	else if ( !Areg.empty() && Areg != operand1  && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	else if (Areg != operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "LDA" << setw(4) <<left << operand1<< "\n";
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

void EmitAssignCode(string operand1, string operand2)
{
	/*
	 if types of operands are not the same
 process error: incompatible types
 if storage mode of operand2 is not VARIABLE
 process error: symbol on left-hand side of assignment must have a storage mode of VARIABLE
 if operand1 = operand2 return;
 if operand1 is not in A register then
 emit code to load operand1 into the A register;
 emit code to store the contents of that register into the memory location pointed to by
 operand2
 deassign operand1;
 if operand1 is a temp then free its name for reuse;
 //operand2 can never be a temporary since it is to the left of ':=' */
 
 	 //check if  data Types are integers
	 for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
			{
				string i = "";
				string j = "";
				string k = "";
				string l = "";
				if(y->second.internalName == operand1)
				{
					i = storeTypeString[y->second.dataType] ;
					k = y->second.value ;

				}
				if(y->second.internalName == operand2)
				{
					j = storeTypeString[y->second.dataType] ;
					l = y->second.value ;
					if (storeTypeString[y->second.mode] != "VARIABLE")
					{
						error("symbol on the left hand side is not variable");
					}
				}
				if (i != j)
				{
					error( " incompatable types");
				}
				if (l == k)
				{
					break;
				}	
			}
	if (Areg != operand1 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand1<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IDV " << setw(4) <<left << operand1<< "\n";
		
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



void EmitDivisionCode(string operand1, string operand2)
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
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IDV " << setw(4) <<left << operand1<< "\n";
		
	}
	// if register is not operand 2 then empty reg and load op2
	else if (Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IDV " << setw(4) <<left << operand1<< "\n";
		
	}
	else if (Areg == operand2 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IDV " << setw(4) <<left << operand1<< "\n";
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


void EmitModuloCode(string operand1, string operand2)
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
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(4) <<left << Areg<< "\n";
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
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IDV " << setw(4) <<left << operand1<< "\n";
		
	}
	// if register is not operand 2 then empty reg and load op2
	else if (Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IDV " << setw(4) <<left << operand1<< "\n";
		
	}
	else if (Areg == operand2 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IDV " << setw(4) <<left << operand1<< "\n";
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
void EmitOrCode(string operand1, string operand2)
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
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IAD " << setw(4) <<left << operand1<< "\n";
		string label = get_Label();
		objectFile << setw(5) << "" << operand2 << " or " << operand1 << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << left << label << "+1   " << endl;
		objectFile << setw(4) << label << setw(2) << "" << "LDA " << setw(4) << left << "TRUE     " << endl;
		
	}
	// if register has operand1 multiply by op2
	else if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IAD " << setw(4) <<left << operand2<< "\n";
		string label = get_Label();
		objectFile << setw(5) << "" << operand2 << " or " << operand1 << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << left << label << "+1   " << endl;
		objectFile << setw(4) << label << setw(2) << "" << "LDA " << setw(4) << left << "TRUE     " << endl;
	}
	// if register has operand2 multiply by op1
	else if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IAD " << setw(4) <<left << operand1<< "\n";
		string label = get_Label();
		objectFile << setw(5) << "" << operand2 << " or " << operand1 << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << left << label << "+1   " << endl;
		objectFile << setw(4) << label << setw(2) << "" << "LDA " << setw(4) << left << "TRUE     " << endl;
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
void EmitEqualsCode(string operand1, string operand2)
{
	string x = "";
	string z = "";
		 //check if  data Types are integers
	 for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
			{
				if(y->second.internalName == operand1)
				{
					x = storeTypeString[y->second.dataType]; 
					
				}
				else if(y->second.internalName == operand2)
				{
					z = storeTypeString[y->second.dataType];
				}
			}
	if (x != z)
	{
		error("incompatable data types");
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
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(4) <<left << operand1;
		objectFile << setw(5) << "" << operand2 << " = " << operand1 << endl;
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << label << setw(5) << "" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA" << setw(4) <<left << "TRUE";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) << "FALS     " << endl;
		
	}
	// if register has operand1 multiply by op2
	else if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(4) <<left << operand2;
		objectFile << setw(5) << "" << operand2 << " = " << operand1 << endl;
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << label << setw(5) << "" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA" << setw(4) <<left << "TRUE";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) << "FALS     " << endl;
	}
	// if register has operand2 multiply by op1
	else if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(4) <<left << operand1;
		objectFile << setw(5) << "" << operand2 << " = " << operand1 << endl;
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << label << setw(5) << "" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA" << setw(4) <<left << "TRUE";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) << "FALS     " << endl;
		
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
void EmitLTCode(string, string)
{
	
}
void EmitGTCode(string, string)
{
	
}
void EmitGTOECode(string, string)
{
	
}
void EmitLTOECode(string, string)
{
	
}
void EmitDNECode(string, string)
{
	
}

 

void code(string oprtr, string operand1, string operand2) 
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
        readStmt();
    }
    else if (oprtr == "write")
    {
        writeStmt();
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


