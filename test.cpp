#include <iostream>
#include <string>
#include <sstream>
int main()
{
    std::ostringstream ss;
        std::string testString("a\0b\0c\0d", 7);

	    ss.write(&testString, testString.size());

	        std::cout << ss.str(); // abcd
		}
