#include "TarReader.hpp"
#include <iostream>

using namespace libtarpp;
int main()
{
	TarReader tr;
	tr.load("test.ccell");
	cout<<tr.readText("description")<<endl;
	return 0;
}
