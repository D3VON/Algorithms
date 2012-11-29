// inserting into a vector
#include <iostream>
#include <vector>
using namespace std;

int main ()
{

/*Other functions:

vector::push_back   Add element at the end (public member function) 
vector::erase       Erase elements (public member function) 

*/



  vector<int> myvector (3,100);
  vector<int>::iterator it;
//iterator means'how many'

  it = myvector.begin();
  it = myvector.insert ( it , 200 );

  myvector.insert (it,2,300);

  // "it" no longer valid, get a new one:
  it = myvector.begin();
  // begins iterator at ZERO

  vector<int> anothervector (2,400);
  myvector.insert (it+2,anothervector.begin(),anothervector.end());

  int myarray [] = { 501,502,503 };
  myvector.insert (myvector.begin(), myarray, myarray+3);

  cout << "myvector contains:";
  for (it=myvector.begin(); it<myvector.end(); it++)
    cout << " " << *it;
  cout << endl;

  return 0;
}

/*Output:
myvector contains: 501 502 503 300 300 400 400 200 100 100 100 
*/


