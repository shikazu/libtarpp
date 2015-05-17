#pragma once
#include <string>

#ifdef __linux__
#include <pwd.h>
#endif

#include <vector>
#include "TarContents.hpp"
using namespace std;

namespace libtarpp
{
	class TarArchive
	{
		vector<TarContents> contents;

		public:

		void load(const string filename);
		void save(string filename);
		void addFile(string filename);
		void addFile(string filename,string path);
		TarContents getContents(const string filename);
	};
}
