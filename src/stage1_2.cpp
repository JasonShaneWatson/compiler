#include "stage1.h"


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
