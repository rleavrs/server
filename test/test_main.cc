#include <iostream>
// #include <format>
#include <string_view>
#include <vector>
using namespace std;

//template <typename... Args>
//void print(const std::string_view fmt_str, Args&&... args) {
//	auto fmt_args{ std::make_format_args(args...) };
//	std::string outstr{ std::vformat{fmt_str, fmt_arges} };
//	fputs{ outstr.c_str(),stdout };
//}

class Base {
protected:
	int a = 1;
};

int main() {

	vector<int> v = { 1,2,3 };

	auto begin = v.begin();
	auto end = v.end();
	while (begin != end) {
		cout << &*begin << endl;
        cout << *begin << endl;

        begin++;
	}
	

}