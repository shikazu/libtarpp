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
#include <grp.h>
#endif
using namespace std;
void libtarpp::TarArchive::addFile(string filename)
{
	int pos = filename.find_last_of("/");
	addFile(filename,filename.substr(pos+1,filename.size()-pos));
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
	oss_size<<oct<<s.st_size<<flush;
	ct.setSize(oss_size.str());
	

	ostringstream oss_mtime;
	oss_mtime<<oct<<s.st_mtime<<flush;
	ct.setMTime(oss_mtime.str());


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

	struct passwd *p;
	p=getpwuid(s.st_uid);
	ostringstream oss_uname;
	//oss_uname<<s.st_uname<<flush;
	ct.setUName(p->pw_name);
	
	struct group *g;
	g=getgrgid(s.st_gid);
	ct.setGName(g->gr_name);

	ct.autoChkSum();

	ct.setStream(shared_ptr<ios>(new ifstream(filename)));
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
		ofs<<it.getUName()<<flush;
		ofs<<it.getGName()<<flush;
		ofs<<it.getDevMajor()<<flush;
		ofs<<it.getDevMinor()<<flush;
		ofs<<it.getPrefix()<<flush;
		for(int i=1; i<=12; i++)
		{
			ofs<<'\0';
		}
		ofs<<flush;
		ofs << (it.getStream())->rdbuf()<<flush;
		long size = (it.getStream())->rdbuf()->pubseekoff(0,ios_base::end);
		for(int i=1; i<=512-size%512;i++)
		{
			ofs<<'\0';
		}
	}

		for(int i=1; i<=512*2;i++)
		{
			ofs<<'\0';
		}
		ofs<<flush;
}

int main(void)
{
	libtarpp::TarArchive ta;
	ta.addFile("./tntn");
	ta.addFile("apache2.conf");
	ta.save("untisitai.tar");

	libtarpp::TarArchive t2;
	t2.addFile("./tntn");
	t2.save("unko.tar");
}
