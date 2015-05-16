#include "TarArchive.hpp"
#include "TarContents.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

#ifdef __linux__
#include <sys/stat.h>
#include <unistd.h>
#endif
using namespace std;
void libtarpp::TarArchive::addFile(string filename)
{
	addFile(filename,filename);
}

void libtarpp::TarArchive::addFile(string filename,string path)
{
	
	auto it = contents.begin();
	auto ct = TarContents();
	ct.setName(path);

	struct stat s;
	stat(filename.c_str(),&s);

	ostringstream oss_mode;
	oss_mode<<oct<<s.st_mode<<flush;
	ct.setMode(oss_mode.str());
	
	ostringstream oss_uid;
	oss_uid<<s.st_uid<<flush;
	ct.setUid(oss_uid.str());
	
	ostringstream oss_gid;
	oss_gid<<s.st_gid<<flush;
	ct.setGid(oss_gid.str());

	ostringstream oss_size;
	oss_size<<s.st_size<<flush;
	ct.setSize(oss_size.str());
	

	ostringstream oss_mtime;
	oss_mtime<<s.st_mtime<<flush;
	ct.setMTime(oss_mtime.str());

	string tmp = "0000000";
	tmp+="\0";
	ct.setChkSum(tmp);

	ct.setTypeFlag("0");
	bool link = false;
	if(S_ISLNK(s.st_mode))
	{
		ct.setTypeFlag("1");
		link = true;
	}
	if(S_ISCHR(s.st_mode))
	{
		ct.setTypeFlag("3");
	}
	if(S_ISBLK(s.st_mode))
	{
		ct.setTypeFlag("4");
	}
	if(S_ISDIR(s.st_mode))
	{
		ct.setTypeFlag("5");
	}
	if(S_ISFIFO(s.st_mode))
	{
		ct.setTypeFlag("6");
	}

	if(link)
	{
		char buf[100];
		readlink(filename.c_str(),buf,sizeof(buf));
		string tmp(buf);
		ct.setLinkName(tmp);
	}
	
	contents.insert(it,ct);
	
}

void libtarpp::TarArchive::save(string filename)
{
	ofstream ofs(filename);
	for(auto it:contents)
	{
		ofs<<it.getName()<<flush;
		ofs<<it.getMode()<<flush;
		ofs<<it.getUid()<<flush;
		ofs<<it.getGid()<<flush;
		ofs<<it.getSize()<<flush;
		ofs<<it.getMTime()<<flush;
		ofs<<it.getChkSum()<<flush;
		ofs<<it.getTypeFlag()<<flush;
		ofs<<it.getLinkName()<<flush;
		ofs<<it.magic<<flush;
		ofs<<it.version<<flush;
	}
}

int main(void)
{
	libtarpp::TarArchive ta;
	ta.addFile("/etc/apache2/apache2.conf");
	ta.save("untisitai.tar");
}
