#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;
int main(){
	cout << getcwd(NULL, 0) << endl;
	return 0;
}
