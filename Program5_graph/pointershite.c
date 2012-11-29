#include <stdio.h>
 
void f(int *j); // function declaration
 
int main() 
{ 
  int i; 
  int *p; 
  printf("i's actual address: ");
  printf("%x\n",&i); // i's actual address
 
  p = &i; // p points to the 'address of' i
  printf("p's actual address: ");
  printf("%x\n",&p); // p's actual address
 
  f(p);   // f gets the address of i via pointer p
 
  printf("i's value: "); // is output after all the function's output.
  printf("%d\n",i);
 
  return 0; 
} 
 
void f(int *j) // f gets the address of i , stores it in j pointer
{ 
  // j is acting as if it was p now. 
  *j = 100; // store the value 100 where j points (j points to p points to i)

  // wow, I didn't expect this:
  printf("the address j's holding (and p's holding), which is i's address: ");
  printf("%x\n",j); // the address j's holding (p's address)

  printf("the value stored (j points to p points to a value): ");
  printf("%d\n",*j); // the value stored (j points to p points to a value)

  printf("j's own address: ");
  printf("%x\n",&j); // j's own address

}
