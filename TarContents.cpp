#include <string>
#include <iostream>
#include "TarContents.hpp"

libtarpp::TarContents::TarContents()
{
	setName("");
	setMode("0100777");
	setUid("0");
	setGid("0");
	setSize("0");
	setMTime("0");
	setTypeFlag("0");
	setLinkName("");
	setUName("root");
	setGName("root");
	setDevMajor("0");
	setDevMinor("0");
}

const string libtarpp::TarContents::magic = "ustar ";
const string libtarpp::TarContents::version = "00";

bool libtarpp::TarContents::isOctal(const string numstring)
{
	for(auto c: numstring)
	{
		if(
		c=='0' ||
		c=='1' ||
		c=='2' ||
		c=='3' ||
		c=='4' ||
		c=='5' ||
		c=='6' ||
		c=='7'
		)
		{
			continue;
		}else{
			return false;
		}
	}
	return true;
}

bool libtarpp::TarContents::isNum(const string numstring)
{
	for(auto c:numstring)
	{
		for(int i=0;i<=9;i++)
		{
			if(c == to_string(i)[0])
			{
				break;
			}else{
				if(i==9)
				{
					return false;
				}
			}
		}
	}
	return true;
}

void libtarpp::TarContents::setName(const string x)
{
	if(x.size()>100)
	{
		throw "error: The length of filename must be less than 100.";
	}
	
	string tmp;
	tmp+=x;
	for(int i=1; i<=100 - x.size();i++)
	{
		tmp+='\0';
	}
	name = tmp;
}

string libtarpp::TarContents::getName()
{
	return name;
}

void libtarpp::TarContents::setMode(const string m)
{
	//cout<<m.size() << endl;
	if(m.size() == 8)
	{
		if(isOctal(m.substr(0,7)))
		{
			mode = m;
		}
	}
	if(m.size() == 7)
	{
		if(isOctal(m))
		{
			mode = m;
			mode += '\0';
		}
	}
	if(m.size() <=6)
	{
		if(isOctal(m))
		{
			string tmp = m;
			for(int i = 1;i<=8-m.size()-1;i++)
			{
				tmp = "0" + tmp;
			}
			tmp += '\0';
			mode = tmp;
		}
	}

}

string libtarpp::TarContents::getMode()
{
	return mode;
}

string libtarpp::TarContents::regulateUGId(const string id)
{
	if(id.size() == 8 && isNum(id.substr(0,7)) )
	{
		return id;
	}
	if(id.size() == 7 && isNum(id))
	{
		string tmp;
		tmp = id;
		tmp += '\0';
		return tmp;
	}
	if(id.size() <=6 && isNum(id))
	{
		string tmp;
		tmp = id;
		for(int i = 1;i<=8-id.size()-1;i++)
		{
			tmp = "0" + tmp;
		}
		tmp += '\0';
		return tmp;
	}

	throw "Invalid uid/gid.";
}


string libtarpp::TarContents::getUid()
{
	return uid;
}

void libtarpp::TarContents::setUid(const string u)
{
	uid = regulateUGId(u);
}

string libtarpp::TarContents::getGid()
{
	return gid;
}

void libtarpp::TarContents::setGid(const string g)
{
	gid = regulateUGId(g);
}

string libtarpp::TarContents::getSize()
{
	return size;
}

void libtarpp::TarContents::setSize(const string s)
{
	if(s.size() == 12 && isNum(s.substr(0,11)))
	{
		size = s;
	}
	if(s.size() == 11 && isNum(s))
	{
		string tmp;
		tmp = s;
		tmp += '\0';
		size = tmp;
	}
	if(s.size()<=10 && isNum(s))
	{
		string tmp;
		tmp = s;
		for(int i=1; i<=12-s.size()-1;i++)
		{
			tmp = "0" + tmp;
		}
		tmp += '\0';
		size = tmp;
	}
}
//We need to fix following by 2038!!
string libtarpp::TarContents::getMTime()
{
	return mtime;
}

void libtarpp::TarContents::setMTime(const string m)
{
	if(m.size() == 12 && isNum(m.substr(0,11)))
	{
		mtime = m;
	}
	if(m.size() == 11 && isNum(m))
	{
		string tmp;
		tmp = m;
		tmp += '\0';
		mtime = tmp;
	}
	if(m.size()<=10 && isNum(m))
	{
		string tmp;
		tmp = m;
		for(int i=1; i<=12-m.size()-1;i++)
		{
			tmp = "0" + tmp;
		}
		tmp += '\0';
		mtime = tmp;
	}
}

string libtarpp::TarContents::getChkSum()
{
	return chksum;
}

void libtarpp::TarContents::setChkSum(const string c)
{
	string tmp;
	tmp = "0000000";
	tmp+='\0';
	chksum = tmp;
}

string libtarpp::TarContents::getTypeFlag()
{
	return typeflag;
}

void libtarpp::TarContents::setTypeFlag(const string f)
{
	if(f.size() == 1)
	{
		typeflag = f;
		return;
	}
	throw "error: invalid typeflag.";
}

string libtarpp::TarContents::getLinkName()
{
	return linkname;
}

void libtarpp::TarContents::setLinkName(const string ln)
{
	if(ln.size() == 100)
	{
		linkname = ln;
		return;
	}
	if(ln.size()<100)
	{
		string tmp;
		tmp = ln;
		for(int i=1;i<=100-ln.size();i++)
		{
			tmp += '\0';
		}
		linkname = tmp;
		return;
	}
}

string libtarpp::TarContents::getUName()
{
	return uname;
}

void libtarpp::TarContents::setUName(const string un)
{
	if(un.size() == 32 && un[31] == '\0')
	{
		uname =un;
		return;
	}
	if(un.size() <= 31)
	{
		string tmp;
		tmp = un;
		for(int i = 1;i<=32-un.size();i++)
		{
			tmp +='\n';
		}
		uname = tmp;
		return;
	}
	throw "Too long uname.";
}

string libtarpp::TarContents::getGName()
{
	return gname;
}

void libtarpp::TarContents::setGName(const string gn)
{
	if(gn.size() == 32 && gn[31] == '\0')
	{
		gname =gn;
		return;
	}
	if(gn.size() <= 31)
	{
		string tmp;
		tmp = gn;
		for(int i = 1;i<=32-gn.size();i++)
		{
			tmp +='\n';
		}
		gname = tmp;
		return;
	}
	throw "Too long uname.";

}

string libtarpp::TarContents::getDevMajor()
{
	return devmajor;
}

void libtarpp::TarContents::setDevMajor(const string dma)
{
	string tmp;
	for(int i=1; i<=8;i++)
	{
		tmp+='\0';
	}
	devmajor = tmp;
}

string libtarpp::TarContents::getDevMinor()
{
	return devminor;
}

void libtarpp::TarContents::setDevMinor(const string dmi)
{
	string tmp;
	for(int i=1; i<=8;i++)
	{
		tmp+='\0';
	}
	devminor = tmp;

}
