#include <iostream>
#include <string_view>
#include <vector>
#include <time.h>
#include <unistd.h>
using namespace std;


class Base {
public:
	Base() {cout << "c" << endl;}
	~Base() {cout << "d" << endl;}

};

int main() {
	while (1)
	{
		Base a;
		sleep(0.01);
	}
	
	

}