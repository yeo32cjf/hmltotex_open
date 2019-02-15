#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<Windows.h>
#include<algorithm>
#include<direct.h>
#include<process.h>
#include<conio.h>

#include "hwpEqToTex.h"
#define HMLTOTEX
#define PRINTLOG

using namespace std;

#define PLAINTYPE 0
#define EQTYPE 1
#define PADDINGTYPE 1024

#define HMLCHAR 0
#define HMLSCRIPT 1
#define HMLENDP 2
#define HMLOPENP 3
#define HMLPADDING 1024

#define SPACE 32
#define ESC 27

using namespace std;


class position
{
public:
	friend const bool operator<(const position &a,const position &b);
	wchar_t *pos;
	int type;
	bool operator<(const position &Other)
	{
		return pos < Other.pos;
	}
};

class statement
{
public:
	statement(wstring a,int isEq)
	{
		element = a;
		isEquation = isEq;
	}
	wstring element;
	int isEquation;
};


extern int strsize;
extern int hmlsize;


extern vector<statement> *text;		//본문의 내용이 저장됨
extern vector<char *> resDirectory;	//결과 파일이 저장될 폴더
extern char **filename;


extern wchar_t *buf;
extern wchar_t *p;
extern wchar_t *r;
extern wstring tmp;
extern vector<position> hmlpos;
extern int fileNum;

extern int onlySpaceNum;
extern int tmpint;

#ifdef _WIN32
extern HANDLE *thread;
#endif
#ifdef __APPLE__
extern pthread_t *thread;
#endif
extern bool beginC;


extern vector<funcList> textSorted;
extern vector<wstring> textSorted_t;


extern wstring textPrev[];
extern wstring textNext[];
extern vector<wstring> hmltext;



extern const bool operator<(const position &a,const position &b);
extern bool initializeHMLValue(int argc,char **argv);
extern bool hwpStrToTex(wstring &str);
extern void readXML(char *filename,wifstream &fp,vector<position> &hmlpos,wchar_t **buf);
extern int certificateProgram();
extern void processPlainText();
extern void processEquation();
extern void processParEnd();
extern void processParStart();
extern bool checkProcess(position &pos);
extern void processClipboard();
extern void writeCustomDef(const char *dir);

extern void cleanMemory();
extern int isOnlySpace(wstring &str,int *p);
extern void processPlainText2();
extern void initializeValue2(int argc,char **argv);
extern void processAnother(int i);
extern bool checkProcess(position &pos);

#ifdef _WIN32
extern unsigned int __stdcall processThread(void *p);
#endif
#ifdef __APPLE__
extern void *processThread(void *p);
#endif
