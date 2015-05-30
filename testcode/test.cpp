#include "TarReader.hpp"
#include "TarWriter.hpp"
#include <iostream>

using namespace libtarpp;
int main()
{
	TarReader tr;
	tr.load("test.ccell");
	//cout<<tr.readText("description")<<endl;
		
	TarWriter tw;
	tw.addBinary(tr.readBinary("png"),"png");
	tw.save("written.tar");

	return 0;
}
