#include "stage1.h"

void execStmts() 
{
  nextToken();
  if(token == "end")
    return;
  
  //recurse until 
  execStmts();
}
