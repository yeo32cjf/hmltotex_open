#pragma once
#include "processHML.h"


int strsize;
int hmlsize;


vector<statement> *text;		//본문의 내용이 저장됨
vector<char *> resDirectory;	//결과 파일이 저장될 폴더
char **filename;


wchar_t *buf;
wchar_t *p;
wchar_t *r;
wstring tmp;
vector<position> hmlpos;
int fileNum;

int onlySpaceNum = 0;
int tmpint = 0;

#ifdef _WIN32
HANDLE *thread;
#endif
#ifdef __APPLE__
pthread_t *thread;
#endif
bool beginC = false;


vector<funcList> textSorted;
vector<wstring> textSorted_t;


wstring textPrev[] = {L"<TAB/><TAB/>",L"<TAB/>",L"<NBSPACE/>",L"<LINEBREAK/>",L"<FWSPACE/>"
	,L"ㄱ.",L"ㄴ.",L"ㄷ.",L"&lt;",L"&gt;",L"%",L"&amp;",L"#"};

wstring textNext[] = {L" ",L" ",L" ",L"\n",L" "
	,L"\\item ",L"\\item ",L"\\item ",L"<",L">",L"\\%",L"\\&",L"\\#"};

vector<wstring> hmltext;


const bool operator<(const position &a,const position &b)
{
	return a.pos<b.pos;
};


bool initializeHMLValue(int argc,char **argv)
{
	
	strsize = sizeof(textPrev)/sizeof(wstring);

	hmltext.push_back(L"<CHAR");
	hmltext.push_back(L"<SCRIPT");
	hmltext.push_back(L"</P");
	hmltext.push_back(L"<P");
	
	hmlsize = hmltext.size();
	
#ifdef _WIN32
	_wsetlocale(LC_ALL, L"");
	std::wcout.imbue(std::locale("korean"));
#endif

	text = new vector<statement>[argc-1];
	filename = new char*[argc-1];

	
	//TeX에서 필요 없는 hml만의 문자열 읽기
	for(int i=0;i<strsize;i++)
	{
		textSorted.push_back(funcList());
		textSorted[i].hwp = &textPrev[i];
		textSorted[i].Tex = &textNext[i];
	}

	sort(textSorted.begin(),textSorted.end());

	
	for(int fileNum=1;fileNum<argc;fileNum++)
	{
		filename[fileNum-1] = new char[1024];

		getFileNameOnly(argv[fileNum],filename[fileNum-1]);
		resDirectory.push_back(NULL);
		resDirectory[fileNum-1] = new char[1024];
		sprintf(resDirectory[fileNum-1],"hmltotex_%s",filename[fileNum-1]);
#ifdef _WIN32
		_mkdir(resDirectory[fileNum-1]);
#endif
#ifdef __APPLE__
		mkdir(resDirectory[fileNum-1],0775);
#endif
	}

	initializeValue2(argc,argv);
	return true;
}

bool hwpStrToTex(wstring &str)    //문자열 변환
{
	int startPos;
	bool tt = false;

	for(int i=0;i<strsize;i++)
	{
		startPos = 0;
		int hwpstrlen = textPrev[i].length();
		while(1)
		{
			int n = str.find(textPrev[i],startPos);
			if(n != wstring::npos)    //부분 문자열 발견
			{
				tt = true;
				str.replace(n,hwpstrlen,textNext[i]);
				startPos = n+textNext[i].length();
			}
			else
			{
				break;
			}
		}
	}
	return tt;
}


void getFileNameOnly(const char *path,char *namebuf)
{
	const char *dirpos,*dotpos;
#ifdef _WIN32
	dirpos = strrchr(path,'\\');
	++dirpos;
	dotpos = strrchr(path,'.');
	memmove(namebuf,dirpos,dotpos-dirpos);
	namebuf[dotpos-dirpos] = 0;
#endif
#ifdef __APPLE__
	dirpos = strrchr(path,'/');
	dotpos = strrchr(path,'.');
	if(dirpos == NULL)
		dirpos = path;
	else
		++dirpos;
	if(dotpos == NULL)
		dotpos = path+strlen(path);
	memmove(namebuf,dirpos,dotpos-dirpos);
	namebuf[dotpos-dirpos] = 0;
#endif
}


void readXML(char *filename,wifstream &fp,vector<position> &hmlpos,wchar_t **buf)
{
	fp.open(filename,std::ifstream::in);

	fp.seekg(0,std::ios_base::end);
	int length = fp.tellg();
	*buf = new wchar_t[length+100];
	fp.seekg(0,std::ios_base::beg);
	fp.read(*buf,length);
	fp.close();

	(*buf)[length] = 0;
	wchar_t *start = *buf;

	for(int i=0;i<10;i++)
	{
		position x;
		x.pos = 0;
		x.type = HMLPADDING;
		hmlpos.push_back(x);
	}

	for(int i=0;i<hmlsize;i++)
	{
		start = *buf;
		const wchar_t *searchstr = hmltext[i].data();
		int pos = hmltext[i].size();

		while(1)
		{
			wchar_t *pt = wcsstr(start,searchstr);
			if(pt)
			{
				if(pt[pos] == ' ' || pt[pos] == '>')
				{
					position x;
					x.pos = pt;
					x.type = i;
					hmlpos.push_back(x);
				}
				start = pt + hmltext[i].size();
			}
			else
				break;
		}
	}

	sort(hmlpos.begin()+10,hmlpos.end());

	for(int i=0;i<5;i++)
	{
		position x;
		x.pos = 0;
		x.type = HMLPADDING;
		hmlpos.push_back(x);
	}
}



void errorCert()
{
	printf("인증 실패. 아무 키를 누르면 종료됩니다...");
	getch();
}



int certificateProgram()
{

	return 0;
}


void processPlainText()
{
	r = wcsstr(p+6,L"</CHAR>");
	tmp = wstring(p+6,r);

	int onlySpace = isOnlySpace(tmp,&tmpint);

	if(onlySpace)
	{
		if(tmpint==0)
			++onlySpaceNum;
		else
			onlySpaceNum = 0;
	}
	else
		onlySpaceNum = 0;

	for(int i=0;tmp[i];i++)
	{
		if(tmp[i] == '{' || tmp[i] == '}')
		{
			tmp.insert(i,L"\\");
			++i;
		}
	}

	processPlainText2();
}

void processEquation()
{
	r = wcsstr(p+8,L"</SCRIPT>");
	tmp = L"$";
	tmp += wstring(p+8,r);
	tmp += L"$";
	text[fileNum-1].push_back(statement(tmp,EQTYPE));
	onlySpaceNum = 0;
}

void processParEnd()
{
	if(onlySpaceNum < 2)
	{
		text[fileNum-1].push_back(statement(wstring(L"\n"),PLAINTYPE));
		++onlySpaceNum;
	}
}

void processParStart()
{
	if(beginC == false)
	{
		if(onlySpaceNum < 2)
		{
			text[fileNum-1].push_back(statement(wstring(L"\n"),PLAINTYPE));
			++onlySpaceNum;
		}
	}
}


void processClipboard()
{
#ifndef TESTMODE
	cout << "스페이스 바 키를 누르면 클립보드에 저장된 한글 수식이 텍 수식으로 변경됩니다." << endl;
	cout << "P 키를 누르면 출력된 모든 기록을 지웁니다." << endl;
	cout << "ESC 키를 누르면 프로그램이 종료됩니다." << endl << endl;

	wstring str;

	while(wchar_t ch=_getch())
	{
		if(ch == SPACE)
		{
			cout << "클립보드 읽는 중..." << endl;

			//클립보드의 데이터를 wstring형 변수 tmp에 저장
#ifdef _WIN32
			::OpenClipboard(NULL);
			HANDLE hClipboard = GetClipboardData(CF_UNICODETEXT);
			if (!hClipboard)
				return;
			wstring tmp = (wchar_t*)GlobalLock(hClipboard);
			GlobalUnlock(hClipboard);
			CloseClipboard();
#endif

#ifdef __APPLE__
			FILE *pipe = popen("pbpaste","r");
			string x = "";
			char buffer[1024];

			while(!feof(pipe))
			{
				if(fgets(buffer,1024,pipe) != NULL)
				{
					x += buffer;
				}
			}
			pclose(pipe);

			wstring tmp = L"";
			for(int i=0;x[i];i++)
			{
				tmp.push_back(x[i]);
			}
#endif
			//클립보드 데이터 가져오기 끝

			cout << "==================<현재 클립보드의 내용>==================" << endl;
			wcout << tmp << endl;
			cout << "==========================================================" << endl;

			if(tmp[0] == '$' || tmp.find(L"\\begin{align*}") == 0)
			{
				cout << "현재 클립보드의 내용은 이미 TeX 수식입니다." << endl;
				cout << "변환을 수행하지 않습니다." << endl << endl;

				continue;
			}

			//한글에서 스크립트 매크로를 이용하여 수식 앞뒤에 딸라 붙이는 처리를 한 경우
			if(tmp.find(L"$") != wstring::npos)        //$를 포함하는 문자열인 경우
			{
				int startStr = 0;
				int startEq = 0;
				int endEq = 0;

				cout << "변경 시작..." << endl;
				while(1)
				{
					startEq = tmp.find(L"$",startStr);        //수식 시작하는 $
					if(startEq == wstring::npos)
					{
						str += wstring(tmp.data()+startStr);
						break;
					}
					endEq = tmp.find(L"$",startEq+1)+1;        //수식 끝나는 $

					str += wstring(tmp.data()+startStr,tmp.data()+startEq);        //수식 이전의 문자열 연결
					wstring eqStr = wstring(tmp.data()+startEq,tmp.data()+endEq);    //수식 문자열 얻기
					hwpEqToTex(eqStr);                                            //수식 변환
					str += eqStr;                                                //변환된 수식을 연결

					startStr = endEq;
				}
			}
			else
			{
				str = L"$";
				str += tmp;
				str += L"$";
				cout << "변경 시작..." << endl;
				hwpEqToTex(str);
			}

			//결과물을 클립보드에 저장
#ifdef _WIN32

			::OpenClipboard(NULL);
			EmptyClipboard();

			HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 2*str.size()+2);
			char *mem = (char*)GlobalLock(hglbCopy);
			memmove(mem,str.data(),2*str.size()+2);
			GlobalUnlock(hglbCopy);
			::SetClipboardData(CF_UNICODETEXT, mem);
			CloseClipboard();
#endif

#ifdef __APPLE__
			int searchPos = -1;
			while(1)    //"를 \"로 변경
			{
				searchPos = str.find(L"\"",searchPos+1);
				if(searchPos == wstring::npos)
					break;
				str.insert(searchPos,L"\\");
				searchPos += 2;
			}
			const char proto_cmd[] = "echo \"%S\" | pbcopy";
			char *cmd;
			cmd = new char[str.size()+strlen(proto_cmd)];
			sprintf(cmd ,proto_cmd, str.data());

			system(cmd);
#endif
			//결과물을 클립보드에 저장 완료

			wcout << "변경 완료." << endl;

			cout << "==================<현재 클립보드의 내용>==================" << endl;
			wcout << str << endl;
			cout << "==========================================================" << endl << endl << endl;
		}
		else if(ch=='p' || ch=='P')
		{
#ifdef _WIN32
			system("cls");
#endif

#ifdef __APPLE__
			system("clear");
#endif
			cout << "스페이스 바 키를 누르면 클립보드에 저장된 한글 수식이 텍 수식으로 변경됩니다." << endl;
			cout << "P 키를 누르면 출력된 모든 기록을 지웁니다." << endl;
			cout << "ESC 키를 누르면 프로그램이 종료됩니다." << endl << endl;
		}
		else if(ch == ESC)
			break;
	}
#endif
}

void writeCustomDef(const char *dir)
{
	char def[128];
	sprintf(def,"%s\\customDef.sty",dir);

	wofstream customDef;
	customDef.open(def);

	customDef << "\\RequirePackage[c]{esvect}\n";
	customDef << "\\RequirePackage{amsmath}\n";
	customDef << "\\RequirePackage{amssymb}\n";
	customDef << "\\RequirePackage{tabu}\n";
	customDef << "\\RequirePackage{tikz}\n\n";
		

	customDef << "\\renewcommand{\\vec}[1]{\\vv{#1}}\n"
			"\\let\\nvec\\vec\n"
			"\\let\\nsqrt\\sqrt\n"
			"\\def\\vec#1{\\nvec{\\vphantom A\\smash{#1}}}\n"
			"\\def\\SQRT#1{\\nsqrt{\\vphantom{B^2}\\smash{#1}}}\n\n"

			"\\newcommand{\\VEC}[1]{\\vv{#1}}\n"
			"\\let\\nVEC\\VEC\n"
			"\\def\\VEC#1{\\nVEC{\\vphantom A\\hspace{1pt}\\smash{#1}\\hspace{.5pt}}}\n\n"

			"\\newcommand{\\mrm}[1]{\\mathrm{#1}}\n"
			"\\newcommand{\\ovl}[1]{\\overline{#1}}\n"
			"\\newcommand{\\ovr}[1]{\\overline{\\mathrm{#1}}}\n"
			"\\newcommand{\\vrm}[1]{\\VEC{\\mathrm{#1}}}\n\n"

			"\\makeatletter\n"
			"\\newcommand*\\bigcdot@[2]{\\mathbin{\\vcenter{\\hbox{\\scalebox{#2}{$\\m@th#1\\bullet$}}}}}\n"
			"\\newcommand*\\bigcdot{\\mathpalette\\bigcdot@{.5}}\n"
			"\\newcommand*\\bcd@[2]{\\mathbin{\\vcenter{\\hbox{\\scalebox{#2}{$\\m@th#1\\bullet$}}}}}\n"
			"\\newcommand*\\bcd{\\mathpalette\\bigcdot@{.5}}\n"
			"\\makeatother\n\n"

			"\\makeatletter\n"
			"\\DeclareFontFamily{U}{tipa}{}\n"
			"\\DeclareFontShape{U}{tipa}{m}{n}{<->tipa10}{}\n"
			"\\newcommand{\\arc@char}{{\\usefont{U}{tipa}{m}{n}\\symbol{62}}}%\n"

			"	\\newcommand{\\arc}[1]{\\mathpalette\\arc@arc{\\mrm{#1}}}\n"

			"\\newcommand{\\arc@arc}[2]{%\n"
			"	\\sbox0{$\\m@th#1#2$}%\n"
			"	\\vbox{\n"
			"		\\hbox{\\resizebox{\\wd0}{\\height}{\\arc@char}}\n"
			"		\\nointerlineskip\n"
			"			\\box0\n"
			"}%\n"
			"}\n"
			"\\makeatother\n";

	customDef.close();
}



#ifdef HMLTOTEX

void cleanMemory()
{
	
}

int isOnlySpace(wstring &str,int *p)
{
	for(int i=str.size()-1;i>=0;i--)
	{
		if(str[i]!=0x0D && str[i]!=0x0A && str[i]!=' ' && str[i]!='\t')
			return 0;
	}
	return 1;
}

void processPlainText2()
{
	text[fileNum-1].push_back(statement(tmp,PLAINTYPE));
}

void initializeValue2(int argc,char **argv)
{
#ifdef _WIN32
	thread = new HANDLE[argc-1];
#endif
#ifdef __APPLE__
	thread = new pthread_t[argc-1];
#endif
}


void processAnother(int i)
{
	if(hmlpos[i].type == HMLPADDING)
		text[fileNum-1].push_back(statement(wstring(L""),PADDINGTYPE));
}

bool checkProcess(position &pos)
{
	if(pos.type > HMLOPENP && pos.type < 10)
		return false;
	return true;
}


#ifdef _WIN32
unsigned int __stdcall processThread(void *p)
#endif
#ifdef __APPLE__
	void *processThread(void *p)
#endif
{
	int fileNum = (size_t)p - 1;
	int textsz = text[fileNum].size();

	wofstream fpw;
	wofstream log;
	
	char fullname[1024];

	int tmpint = 0;
	
#ifdef __APPLE__
	sprintf(fullname,"%s/%s.tex",resDirectory[fileNum],filename[fileNum]);
	//sprintf(fullname,"%s/%s_log.txt",resDirectory[fileNum],filename[fileNum]);
#endif
	sprintf(fullname,"%s\\%s.tex",resDirectory[fileNum],filename[fileNum]);

#ifdef PRINTLOG
	log.open("log.txt");
#endif
	
	for(int i=10;i<textsz-5;i++)
	{
#ifdef PRINTLOG
		
#endif
		//getch();
		if(text[fileNum][i].isEquation == EQTYPE)
		{
#ifdef PRINTLOG
			log << text[fileNum][i].element<< endl;
			hwpEqToTex(text[fileNum][i].element);
			log << text[fileNum][i].element<< endl << endl;
#else
			hwpEqToTex(text[fileNum][i].element);
#endif
			if(text[fileNum][i].element.size() <= 2)
				text[fileNum][i].isEquation = PADDINGTYPE;
		}
		else if(text[fileNum][i].isEquation == PLAINTYPE)
		{
			hwpStrToTex(text[fileNum][i].element);
		}
	}
	
#ifdef PRINTLOG
	log.close();
#endif
	
	fpw.open(fullname);

	fpw << "%!TEX program=xelatex\n";
	fpw << "\\documentclass{oblivoir}\n\n";
	fpw << "\\usepackage{customDef}\n\n";
	
	writeCustomDef(resDirectory[fileNum]);
	
	fpw << "\\begin{document}\n";

	for(int i=10;i<textsz-5;i++)
	{
		if(text[fileNum][i].isEquation != PADDINGTYPE)
			fpw << text[fileNum][i].element;
	}

	fpw << "\\end{document}\n";
	fpw.close();

	return 0;
}

#endif