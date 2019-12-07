#include "stage2.h"


/*
* variables
*/


/*
* prototypes
*/
void PushOperator(string);
void PushOperand(string);
string PopOperand();
string PopOperator();
void free_Temp();
string get_Temp();
string get_Label();
string getExternalName(string);
void checkDataType(string, string, string);
void commutativeCode(string, string, string);
void EmitAdditionCode(string, string); 
void EmitSubtractionCode(string, string);
void EmitNegationCode(string);
void EmitNotCode(string);
void EmitMultiplicationCode(string, string);
void EmitDivisionCode(string, string);
void EmitModuloCode(string, string);
void EmitAndCode(string, string);
void EmitOrCode(string, string);
void EmitEqualsCode(string, string);
void EmitLTCode(string, string);
void EmitGTCode(string, string);
void EmitGTOECode(string, string);
void EmitLTOECode(string, string);
void EmitDNECode(string, string);
void EmitAssignCode(string, string);
void nonCommutativeCode(string, string, string);
void EmitThenCode(string);
void EmitElseCode(string);
void EmitPostIfCode(string);
void EmitWhileCode();
void EmitDoCode(string, string);
void EmitPostWhileCode(string);
void EmitRepeatCode(string, string);
void EmitUntilCode(string);
void BoolTAF(string);


void BoolTAF(string Type)
{
	
  	if (Type == "TRUE")
	{
	auto tableValue3 = symbolTable.find("TRUE");
		if(tableValue3 == symbolTable.end()) //we did not find an entry in the symbolTable
		{
		  insert("TRUE", BOOLEAN, CONSTANT, "1", YES, 1); 
		}
	}
	if (Type == "FALSE")
	{
		auto tableValue2 = symbolTable.find("FALSE");
		if(tableValue2 == symbolTable.end()) //we did not find an entry in the symbolTable
		{
		  insert("FALSE", BOOLEAN, CONSTANT, "0", YES, 1); 
		}
	}
}
void PushOperator(string oprtr)
{
 //cout << "\nPushing \"" << oprtr << "\"\n";
	operatorStk.push(oprtr);
}

void PushOperand(string oprnd)
{	  
	oprnd = oprnd.substr(0,15);
   //cout << "\nPushing \"" << oprnd << "\"\n";
    bool oprndIsINT = true; 

    for (uint x = 0; x < oprnd.length(); x++)
    {
		if (oprnd.at(0) == '+' || oprnd.at(0) == '-')
		{
			continue;
		}
      if (!isdigit(token[x]))
      {
        oprndIsINT = false;
      }
    }


	auto searchValue = symbolTable.find(oprnd);
	if (searchValue == symbolTable.end() || oprnd == "true" || oprnd == "false")
	{
	  int currentElement = 0;
	  for (auto x = symbolTable.cbegin(); x != symbolTable.cend(); ++x) 
	  {
		for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
		{
		  if(y->second.position == currentElement)
		  {
			//cout << "     " << y->second.internalName <<y->second.value <<endl;
			if(y->second.value == whichValue(oprnd) && y->second.value != "" && y->second.dataType == whichType(oprnd))
			{
				//cout << "YVAL  " << y->second.value << "   " << y->second.internalName  << " OPRNDVAL" << whichValue(oprnd) << endl;
				operandStk.push(y->second.internalName);
				return;
			}
		  }
		}
		  currentElement++;
	  }
	 //////cout << "EERRRRRR" << endl;


    }
	
	//if oprnd is ((boolean || int) && not in sybol talbe)
	if (((oprnd == "true" || oprnd == "false") || oprndIsINT ) && (searchValue == symbolTable.end()))
	{
		////cout << "DEBUGGGGGGGGGG" << endl;
		insert(oprnd,whichType(oprnd),CONSTANT,whichValue(oprnd),YES,1);

	}
		searchValue = symbolTable.find(oprnd);
		operandStk.push(searchValue->second.internalName);




}

string PopOperand()
{
 
	if (operandStk.empty()) {
		error("operand stack underflow");
	}
	else
	{
		string top = operandStk.top();
		operandStk.pop();
    //cout << "\nPopping \"" << top << "\"\n";
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
    //cout << "\nPopping \"" << top << "\"\n";
		return top;
	}
	return 0;
}

void free_Temp()
{
	currentTempNo--;
	if (currentTempNo < -1)
	{
		error(" compiler error, currentTempNo should be >= –1");
	}
}

string get_Temp()
{

  //////cout << "currentTempNo = " << currentTempNo << endl
	 currentTempNo++;
	 string temp = "T" + to_string(currentTempNo);
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
  commutativeCode("addition", operand1, operand2);
}

void EmitSubtractionCode(string operand1,string operand2)
{
	 nonCommutativeCode("subtraction",operand1,operand2);
}

void EmitMultiplicationCode(string operand1,string operand2) //multiply operand2 by operand1
{ 
	 commutativeCode("multiplication",operand1,operand2);
}

void EmitDivisionCode(string operand1, string operand2)
{
  nonCommutativeCode("division",operand1,operand2);
}

void EmitAndCode(string operand1,string operand2) //"and" operand1 to operand2
{
 	//make sure  data types are the same
	checkDataType("bool",operand1,operand2);
	//Allocate Temp and store it 
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg << endl;
		Areg = "";
		
	}
	
	// if non-temp is in register then deassign it 
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2 ld op2 and multiply by op1
	if (Areg != operand1 && Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(6) <<left << operand1;
		objectFile << setw(5) << "" << operand2 << " AND " << operand1 << endl;
		
	}
	// if register has operand1 multiply by op2
	if (Areg == operand1 )
	{
		//objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(4) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(6) <<left << operand2;
		objectFile << setw(5) << "" << operand2 << " AND " << operand1 << endl;

	}
	// if register has operand2 multiply by op1
	if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IMU " << setw(6) <<left << operand1;
		objectFile << setw(5) << "" << operand2 << " AND " << operand1 << endl;
	}
	
	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if ((operand2.at(0) == 'T' && operand2 != "TRUE") )
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
	//cout << Areg << endl;
	if ( !Areg.empty() && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg<< "\n";
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	if ( !Areg.empty() && Areg != operand1  && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has operand1 multiply by op2
	if (Areg != operand1 )
	{
		auto tableValue2 = symbolTable.find("ZERO");
		if(tableValue2 == symbolTable.end()) //we did not find an entry in the symbolTabl
		{
		  insert("ZERO", INTEGER, CONSTANT, "0", YES, 1);
		 
		}
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << "ZERO"<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) <<left << operand1<< "\n";
	}
	// if register has operand2 multiply by op1
	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
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
						error("illegal type NOT");
					}
				}
			}
			

	//Allocate Temp and store it 		
	//cout << Areg << endl;
	if ( !Areg.empty() && Areg != operand1  && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg<< "\n";
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	if ( !Areg.empty() && Areg != operand1  && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	if (Areg != operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand1<< "\n";
		Areg = operand1;
	}
	// if register has operand1 set jumps
	if (Areg == operand1 )
	{
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(6) << left << label;
		objectFile << setw(4) << "" << " not " << operand1 << "\n";
		BoolTAF("FALSE");
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << "FALS" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		BoolTAF("TRUE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) <<left << "TRUE"<< endl;
	}

	else 
	{
		error("you broke it in NOT");
	}
	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
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
 emit code to store the contents of that register into the memory location pointed to by operand2
 deassign operand1;
 if operand1 is a temp then free its name for reuse;
 //operand2 can never be a temporary since it is to the left of ':=' */
 	 //check if  data Types are integers
    string i = "";
    string j = "";
    string k = "";
    string l = "";

	for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
  {
    if(y->second.internalName == operand1)
    {
      i = storeTypeString[y->second.dataType] ;
      k = y->second.value ;
    }
    if(y->second.internalName == operand2)
    {
      j = storeTypeString[y->second.dataType] ;
      l = y->second.value ;
      if (modesString[y->second.mode] != "VARIABLE")
      {
       //////cout << modesString[y->second.mode];
        error("symbol on the left hand side is not variable1");
      }
    }
  }
  if (i != j)
  {
    error( " incompatable types Assign" + i );
  }
  
	if (operand1 == operand2)
	{
		return; 
	}

	if (Areg != operand1)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand1<< "\n";
	}
  
	Areg = operand2;
	objectFile << left << setw(6) << " " << setw(3) << "STA " << setw(6) << operand2 << setw(6) << " " << getExternalName(operand2) << " := "<< getExternalName(operand1) << endl;

	// if operand 1 was a temp free it
	if (!operand1.empty() && (operand1.at(0) == 'T' && operand1 != "TRUE"))
	{
		free_Temp();
	}
	//PushOperand(Areg);
	//Areg == operand1;
}


void EmitModuloCode(string operand1, string operand2)
{
	//make sure  data types are integers
	checkDataType("int",operand1,operand2);
	//Allocate Temp and store it 		
	if ( !Areg.empty() &&  Areg != operand2 && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg<< "\n";
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	 if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2
	 if (Areg != operand1 && Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IDV " << setw(6) <<left << operand1<< "\n";
		
	}
	// if register is not operand 2 then empty reg and load op2
	else if (Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IDV " << setw(6) <<left << operand1<< "\n";
		
	}
	if (Areg == operand2 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IDV " << setw(6) <<left << operand1<< "\n";
	}

	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	//A register == Tn
	Areg = get_Temp();
	// make Tn dataType = INTEGER
	objectFile << setw(4) << "" << setw(2) << "" << "STQ " << setw(6) <<left << Areg<< "\n";
	objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << Areg<< "\n";
	auto tableValue1 = symbolTable.find(Areg);
	if(tableValue1 != symbolTable.end()) //we found an entry in the symbolTable
	{
	  tableValue1->second.dataType = INTEGER;
	}
	PushOperand(Areg);
}

void EmitOrCode(string operand1, string operand2)
{
	//make sure  data types are the bool
	checkDataType("bool",operand1,operand2);
	//Allocate Temp and store it 	

	//cout << Areg << " " << operand1 << " " << operand2 <<endl;
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg << endl;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2 ld op2 and multiply by op1
	if (Areg != operand1 && Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "IAD " << setw(6) <<left << operand1;
		string label = get_Label();
		objectFile << setw(5) << "" << operand2 << " or " << operand1 << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << left << label << "+1   " << endl;
		BoolTAF("TRUE");
		objectFile << setw(4) << label << setw(2) << "" << "LDA " << setw(6) << left << "TRUE     " << endl;
		
	}
	// if register has operand1 multiply by op2
	if (Areg == operand1 )
	{
		
		objectFile << setw(4) << "" << setw(2) << "" << "IAD " << setw(6) <<left << operand2 ;
		string label = get_Label();
		objectFile << setw(5) << "" << operand2 << " or " << operand1 << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << left << label << "+1   " << endl;
		BoolTAF("TRUE");
		objectFile << setw(4) << label << setw(2) << "" << "LDA " << setw(6) << left << "TRUE     " << endl;
	}
	// if register has operand2 multiply by op1
	if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "IAD " << setw(6) <<left << operand1 ;
		string label = get_Label();
		objectFile << setw(5) << "" << operand2 << " or " << operand1 << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << left << label << "+1   " << endl;
		BoolTAF("TRUE");
		objectFile << setw(4) << label << setw(2) << "" << "LDA " << setw(6) << left << "TRUE     " << endl;
	}
	
	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if ((operand2.at(0) == 'T' && operand2 != "TRUE") )
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
	//make sure  data types are the same
	checkDataType("same",operand1,operand2);
	//Allocate Temp and store it 
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg << endl;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2 ld op2 and multiply by op1
	if (Areg != operand1 && Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) <<left << operand1;
		objectFile << setw(5) << "" << operand2 << " = " << operand1 << endl;
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(6) << label << setw(5) << "" << endl;
		BoolTAF("FALSE");
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) << "FALS" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		BoolTAF("TRUE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) <<left << "TRUE"<< endl;
		
	}
	// if register has operand1 multiply by op2
	if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) <<left << operand2;
		objectFile << setw(5) << "" << operand2 << " = " << operand1 << endl;
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(6) << label << setw(5) << "" << endl;
		BoolTAF("FALSE");
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) << "FALS" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		BoolTAF("TRUE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) <<left << "TRUE"<< endl;
	}
	// if register has operand2 multiply by op1
	if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) <<left << operand1;
		objectFile << setw(5) << "" << operand2 << " = " << operand1 << endl;
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(6) << label << setw(5) << "" << endl;
		BoolTAF("FALSE");
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) << "FALS" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		BoolTAF("TRUE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) <<left << "TRUE"<< endl;
		
	}

	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if ((operand2.at(0) == 'T' && operand2 != "TRUE") )
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

void EmitLTCode(string operand1, string operand2)
{
	//make sure  data types are integers
	checkDataType("int",operand1,operand2);
	//Allocate Temp and store it 		
	if ( !Areg.empty() && (Areg != operand1 ) && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg<< "\n" ;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2

    if (Areg != operand2 )
	{
		Areg = operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2 << "\n";
		
	}

	// if register has operand2
	if (Areg == operand2)
	{
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) << left << operand1<< "      sub "<< operand1 <<"\n";
		objectFile << setw(4) << "" << setw(2) << "" << "AMJ " << setw(6) << label << setw(5) << "" << endl;
		BoolTAF("FALSE");
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) << "FALS" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		BoolTAF("TRUE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) <<left << "TRUE"<< endl;
		
	}
	else 
	{
		error("you broke it in lessthan");
	}
	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if ((operand2.at(0) == 'T' && operand2 != "TRUE") )
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
	else 
	{
		error ("Compiler error: 'A' register is not in symbol table.");
	}
	PushOperand(Areg);
}

void EmitGTCode(string operand1, string operand2)
{
	//make sure  data types are integers
	checkDataType("int",operand1,operand2);
	//Allocate Temp and store it 		
	if ( !Areg.empty() && (Areg != operand1 ) && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg<< "\n" ;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2
	
    if (Areg != operand2 )
	{
		Areg = operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2 << "\n";
		
	}

	// if register has operand2
	if (Areg == operand2)
	{
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) <<left << operand1<< "      sub "<< operand1 <<"\n";
		objectFile << setw(4) << "" << setw(2) << "" << "AMJ " << setw(6) << label << setw(5) << "" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(6) << label << setw(5) << "" << endl;
		BoolTAF("TRUE");
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) << "TRUE" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		BoolTAF("FALSE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) <<left << "FALS" << endl;
		
	}

	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if ((operand2.at(0) == 'T' && operand2 != "TRUE") )
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
	else 
	{
		error ("Compiler error: 'A' register is not in symbol table.");
	}
	PushOperand(Areg);
}

void EmitGTOECode(string operand1, string operand2)
{
  //make sure  data types are integers
	checkDataType("int",operand1,operand2);
	//Allocate Temp and store it 		
	if ( !Areg.empty() && (Areg != operand1 ) && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg<< "\n" ;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2
	
    if (Areg != operand1 )
	{
		Areg = operand1;
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2 << "\n";
		
	}

	// if register has operand2
	if (Areg == operand1)
	{
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) <<left << operand1<< "      sub "<< operand1 <<"\n";
		objectFile << setw(4) << "" << setw(2) << "" << "AMJ " << setw(6) << label << setw(5) << "" << endl;
		BoolTAF("TRUE");
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) << "TRUE" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		BoolTAF("FALSE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) << left << "FALS"<< endl;
		
	}
	else 
	{
		error("you broke it in lessthan");
	}
	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if ((operand2.at(0) == 'T' && operand2 != "TRUE") )
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
	else 
	{
		error ("Compiler error: 'A' register is not in symbol table.");
	}
	PushOperand(Areg);
}

void EmitLTOECode(string operand1, string operand2)
{
  //make sure  data types are integers
	checkDataType("int",operand1,operand2);
	//Allocate Temp and store it 		
	if ( !Areg.empty() && (Areg != operand1 ) && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg<< "\n" ;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
	// if register has neither operand1 or 2
	
    if (Areg != operand2 )
	{
		Areg = operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2 << "\n";
		
	}

	// if register has operand2
	if (Areg == operand2)
	{
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) <<left << operand1<< "      sub "<< operand1 <<"\n";
		objectFile << setw(4) << "" << setw(2) << "" << "AMJ " << setw(6) << label << setw(5) << "" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(6) << label << setw(5) << "" << endl;
		BoolTAF("FALSE");
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) << "FALS" << endl;
		objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		BoolTAF("TRUE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) <<left << "TRUE"<< endl;
		
	}
	else 
	{
		error("you broke it in lessthan");
	}
	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if ((operand2.at(0) == 'T' && operand2 != "TRUE") )
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
	else 
	{
		error ("Compiler error: 'A' register is not in symbol table.");
	}
	PushOperand(Areg);
}

void EmitDNECode(string operand1, string operand2)
{
  //make sure  data types are the same
	checkDataType("same",operand1,operand2);
	//Allocate Temp and store it 		
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && (Areg.at(0) == 'T' && Areg == "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		  
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg << endl;
		Areg = "";
		
	}
	// if non-temp is in register then deassign it 
	else if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T' && Areg == "TRUE")
	{
		Areg = "";
	}
	// if register has neither operand1 or 2 ld op2 and multiply by op1
	if (Areg != operand1 && Areg != operand2 )
	{
		Areg = "";
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2<< "\n";
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) <<left << operand1;
		objectFile << setw(5) << "" << operand2 << " <> " << operand1 << endl;
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << label << "+1   \n" ;
		BoolTAF("TRUE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) <<left << "TRUE"<< endl;
		
	}
	// if register has operand1 multiply by op2
	if (Areg == operand1 )
	{
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) <<left << operand2;
		objectFile << setw(5) << "" << operand2 << " <> " << operand1 << endl;
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << label  << "+1   \n" ;
		BoolTAF("TRUE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) << "TRUE     " << endl;
		//objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		//BoolTAF("FALSE");
		//objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) <<left << "FALS"<< endl;
	}
	// if register has operand2 multiply by op1
	if (Areg == operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "ISB " << setw(6) <<left << operand1;
		objectFile << setw(5) << "" << operand2 << " <> " << operand1 << endl;
		string label = get_Label();
		objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(4) << label  << "+1   \n" ;
		BoolTAF("TRUE");
		objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) << "TRUE     " << endl;
		//objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(4) << label << "+1   \n" ;
		//BoolTAF("FALSE");
		//objectFile << setw(4) << left << label << setw(2) << "" << "LDA " << setw(6) <<left << "FALS"<< endl;
		
	}

	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	// if operand 2 was a temp free it
	if ((operand2.at(0) == 'T' && operand2 != "TRUE") )
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

void EmitThenCode(string operand1)
{
	/*
	 string tempLabel;
 assign next label to tempLabel;
 emit instruction to set the condition code depending on the value of operand;
 emit instruction to branch to tempLabel if the condition code indicates operand is zero
 (false);
 push tempLabel onto operandStk so that it can be referenced when EmitElseCode() or
 EmitPostIfCode() is called;
 if operand is a temp then
 free operand's name for reuse;
 deassign operands from all registers

	
	*/
	cout << operand1 << endl;
	if (whichType(operand1) != 1)
	{
		error( "predicate of if statement must be boolean valued");
	}
	
	string tempLabel = get_Label();
	if ( Areg != operand1)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand1 << endl;
	}
	
	objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(6) <<left << tempLabel << setw(3)<< "    if false jump to " << tempLabel << endl;
	PushOperand(tempLabel);
	
	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	Areg = "";
	
	
	
	
	
}
void EmitElseCode(string operand1)
{
/*
 string tempLabel;
 assign next label to tempLabel;
 emit instruction to branch unconditionally to tempLabel;
 emit instruction to label this point of object code with the argument operand;
 push tempLabel onto operandStk;
 deassign operands from all registers
	*/
	string tempLabel = get_Label();
	objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(6) <<left << tempLabel << endl;
	objectFile << setw(4) << left << operand1 << setw(2) << "" << "NOP " << setw(8)<< "          else" << endl;
	PushOperand(tempLabel);
	Areg = "";

}
void EmitPostIfCode(string operand1)
{
	/*
 emit instruction to label this point of object code with the argument operand;
 deassign operands from all registers
	*/
	objectFile << setw(4) << left << operand1 << setw(2) << "" << "NOP " << setw(8)<< "          end if" << endl;
	Areg = "";
}
void EmitWhileCode()
{
/*string tempLabel;
 assign next label to tempLabel;
 emit instruction to label this point of object code as tempLabel;
 push tempLabel onto operandStk;
 deassign operands from all registers
 */
 	string tempLabel = get_Label();
	objectFile << setw(4) << left << tempLabel << setw(2) << "" << "NOP " << setw(6) << setw(3)<< "          while" << endl;
	PushOperand(tempLabel);
	Areg = "";
}
void EmitDoCode(string operand1)
{
/*
 string tempLabel;
 assign next label to tempLabel;
 emit instruction to set the condition code depending on the value of operand;
 emit instruction to branch to tempLabel if the condition code indicates operand is zero
 (false);
 push tempLabel onto operandStk;
 if operand is a temp then
 free operand's name for reuse;
 deassign operands from all registers
*/	
 	string tempLabel = get_Label();
	if ( Areg != operand1)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand1 << endl;
	}
	
	objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(6) <<left << tempLabel << setw(3)<< "          do " << tempLabel << endl;

	PushOperand(tempLabel);
	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	Areg = "";
}
void EmitPostWhileCode(string operand1, string operand2)
{
	/*
 emit instruction which branches unconditionally to the beginning of the loop, i.e., to the
 value of operand2;
 emit instruction which labels this point of the object code with the argument operand1;
 deassign operands from all registers
*/
	objectFile << setw(4) << "" << setw(2) << "" << "UNJ " << setw(6) <<left << operand1 << setw(6) << setw(3)<< "    end while" <<endl;
	objectFile << setw(4) << left << operand2 << setw(2) << "" << "NOP       " <<  endl;
	Areg = "";	
}
void EmitRepeatCode()
{
	/*
 var string tempLabel;
 assign next label to tempLabel;
 emit instruction to label this point in the object code with the value of tempLabel;
 push tempLabel onto operandStk;
 deassign operands from all registers
*/	
 	string tempLabel = get_Label();
	objectFile << setw(4) << left << tempLabel << setw(2) << "" << "NOP " << setw(6) << setw(3)<< "          repeat" << endl;
	PushOperand(tempLabel);
	Areg = "";
}
void EmitUntilCode(string operand1, string operand2)
{
	/*
 emit instruction to set the condition code depending on the value of operand1;
 emit instruction to branch to the value of operand2 if the condition code indicates operand is
 zero (false);
 if operand1 is a temp then
 free operand1's name for reuse;
 deassign operands from all registers

*/	
	if ( Areg != operand2)
	{
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand1 << endl;
	}
	
	objectFile << setw(4) << "" << setw(2) << "" << "AZJ " << setw(6) <<left << operand1 << setw(3)<< "until " << endl;
	
	// if operand 1 was a temp free it
	if ((operand1.at(0) == 'T' && operand1 != "TRUE") )
	{
		free_Temp();
	}
	Areg = "";
}

void code(string oprtr, string operand1, string operand2) 
{
 //////cout  << endl << "coding: " << setw(12) << left << "oprtr = " + oprtr + ", " << setw(14) << "operand1 = " + operand1 + ", " << setw(14) << "operand2 = " + operand2 << endl;
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
	else if (oprtr == "then") 
	{
		EmitThenCode(operand1);
	}
	else if (oprtr == "else") 
	{
		EmitElseCode(operand1);
	}
	else if (oprtr == "post_if") 
	{
		EmitPostIfCode(operand1);
	}
	else if (oprtr == "while") 
	{
		EmitWhileCode();
	}
	else if (oprtr == "do") 
	{
		EmitDoCode(operand1);
	}
	else if (oprtr == "post_while") 
	{
		EmitPostWhileCode(operand1, operand2);
	}
	else if (oprtr == "repeat") 
	{
		EmitRepeatCode();
	}
	else if (oprtr == "until") 
	{
		EmitUntilCode(operand1, operand2);
	}
	else
	{
		error("undefined operation");
	}	

}

string getExternalName(string internalName)
{
  for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
  {
    if(y->second.internalName == internalName)
    {
      return y->second.externalName;
    }
  }
  error("Could not find external name for " + internalName);
  return "";
}

void checkDataType(string type, string operand1, string operand2)
{
  storeType operand1Type;
  storeType operand2Type;
  for (auto y = symbolTable.cbegin(); y != symbolTable.cend(); ++y)
	{
    if(y->second.internalName == operand1)
		{
			operand1Type = y->second.dataType ;
			if (storeTypeString[y->second.dataType] != (type=="int"?"INTEGER":(type=="bool"?"BOOLEAN":(storeTypeString[y->second.dataType]))))
			{
				error("illegal type");
			}
		}
	else if(y->second.internalName == operand2)
		{
			operand2Type = y->second.dataType ; 
			//////cout << y->second.dataType << "    " << y->second.externalName << "    " << y->second.internalName << endl;
			if (storeTypeString[y->second.dataType] != (type=="int"?"INTEGER":(type=="bool"?"BOOLEAN":(storeTypeString[y->second.dataType]))))
			{
				error("illegal type");
			}
		}
	}
	//////cout << "HELLLLOOO" << operand1Type <<  " asd  " << operand2Type << endl;
  if (operand1 == operand2)
  {
	  return;
  }
  if  (operand1Type != operand2Type)
  {
		
    error("data types must match");
  }
}

void commutativeCode(string type, string operand1,string operand2)
{
  //make sure  data types are integers
	checkDataType("int",operand1,operand2);
  
	//if A Register holds a temp not operand1 nor operand2 then 
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) <<left << Areg<< "\n" ;
		Areg = "";
	}
  
	// if A register holds a non-temp not operand1 nor operand2 then deassign it
	if ( !Areg.empty() && (Areg != operand1 && Areg != operand2) && Areg.at(0) != 'T')
	{
		Areg = "";
	}
  
	// if neither operand is in A register then
  if (Areg != operand1 && Areg != operand2 )
	{
    //emit code to load operand2 into A register
		Areg = operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2 << "\n";
	}
  
	// if register has operand1
  //emit code to perform register-memory addition
	objectFile << setw(4) << "" << setw(2) << "" << setw(4) << (type=="addition" ? "IAD " : "IMU ") <<
    setw(4) << left << (Areg==operand1?operand2:operand1) <<
    setw(6) << "" <<
    left << (Areg==operand1?getExternalName(operand1):getExternalName(operand2)) << 
    (type=="addition"?" + ":" * ") << 
    (Areg==operand1?getExternalName(operand2):getExternalName(operand1))  << "\n";
  
  //deassign all temporaries involved in the addition and free those names for reuse
	// if operand 1 was a temp free it
	if (operand1.at(0) == 'T' && operand1 != "TRUE")
	{
		free_Temp();
	}
	if (operand2.at(0) == 'T' && operand1 != "TRUE" )
	{
		free_Temp();
	}
  
	//A Register = next available temporary name and change type of its symbol table entry to integer
	Areg = get_Temp();
  //make sure Areg
	auto tableValue1 = symbolTable.find(Areg);
  //we found Areg in the symbolTable
	if(tableValue1 != symbolTable.end())
	{
	  tableValue1->second.dataType = INTEGER;
	}
  else 
  {
    error ("Compiler error: 'A' register is not in symbol table.");
  }
	PushOperand(Areg);
}

void nonCommutativeCode(string type, string operand1,string operand2)
{
  //make sure  data types are integers
	checkDataType("int",operand1,operand2);
  
	//if A Register holds a temp not operand2 then 
	if (!Areg.empty() && Areg != operand2 && (Areg.at(0) == 'T' && Areg != "TRUE"))
	{
		auto tableValue = symbolTable.find(Areg);
		if(tableValue != symbolTable.end()) //we found an entry in the symbolTable
		{
		  tableValue->second.alloc = YES;
		  tableValue->second.units = 1;
		}
		objectFile << setw(4) << "" << setw(2) << "" << "STA " << setw(6) << left << Areg << "\n" ;
		Areg = "";
	}
  
	//if A register holds a non-temp not operand2 then deassign it
	if ( !Areg.empty() && Areg != operand2 && Areg.at(0) != 'T')
	{
		Areg = "";
	}
  
	// if operand2 is not in A register
  if (Areg != operand2 )
	{
    //emit code to load operand2 into A register
		Areg = operand2;
		objectFile << setw(4) << "" << setw(2) << "" << "LDA " << setw(6) <<left << operand2 << "\n";
	}
  
	// if register has operand1
  //emit code to perform register-memory addition
	objectFile << setw(4) << "" << setw(2) << "" << setw(4) << (type=="subtraction" ? "ISB " : "IDV ") <<
    setw(6) << left << operand1 <<
    setw(6) << "" <<
    left << getExternalName(operand2) << 
    (type=="subtraction"?" - ":" div ") << 
    getExternalName(operand1) << "\n";
  
  //deassign all temporaries involved in the addition and free those names for reuse
	// if operand 1 was a temp free it
	if (operand1.at(0) == 'T' && operand1 != "TRUE")
	{
		free_Temp();
	}
	if (operand2.at(0) == 'T' && operand1 != "TRUE" )
	{
		free_Temp();
	}
  
	//A Register = next available temporary name and change type of its symbol table entry to integer
	Areg = get_Temp();
  //make sure Areg
	auto tableValue1 = symbolTable.find(Areg);
  //we found Areg in the symbolTable
	if(tableValue1 != symbolTable.end())
	{
	  tableValue1->second.dataType = INTEGER;
	}
  else 
  {
    error ("Compiler error: 'A' register is not in symbol table.");
  }
	PushOperand(Areg);
}
