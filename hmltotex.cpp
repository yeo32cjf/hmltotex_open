#define _CRT_SECURE_NO_WARNINGS
#define PRINTLOG

#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<math.h>

#ifdef _WIN32
#include<conio.h>
#include<WinSock2.h>
#include<Windows.h>
#include<direct.h>
#include<process.h>
#endif

#ifdef __APPLE__
#include <termios.h>
#include <unistd.h>
#include <wchar.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "processHML.h"

using namespace std;


int main(int argc,char *argv[])
{
	wcout << "hmltotex, From HWP to LaTeX! 김환철 제작." << endl;

	if(certificateProgram())
		return 1;
	initializeEqValue(argc,argv);
	initializeHMLValue(argc,argv);

	//클립보드의 내용을 바꾸는 모드
	if(argc == 1)
	{
		wcout << endl;
		processClipboard();
		return 1;
	}
	
	for(fileNum=1;fileNum<argc;fileNum++)
	{
		wifstream fp;
		readXML(argv[fileNum],fp,hmlpos,&buf);
		
		tmp = L"";
		for(int i=0;i<hmlpos.size();i++)
		{
			if(checkProcess(hmlpos[i]) == false)
				continue;
			
			p = hmlpos[i].pos;
			int hmlPosType = hmlpos[i].type;

			if(hmlPosType == HMLCHAR)	//<CHAR> 발견 : 일반 문자열
			{
				processPlainText();
			}
			else if(hmlPosType == HMLSCRIPT)                //<SCRIPT> 발견 : 수식
			{
				processEquation();
			}
			else if(hmlPosType == HMLENDP)               //</P> 발견
			{
				processParEnd();
			}
			else if(hmlPosType == HMLOPENP)                //<P 발견
			{
				processParStart();
			}
			else
			{
				processAnother(i);
			}
		}
		cleanMemory();
		//delete [] buf;
#ifdef _WIN32
		thread[fileNum-1] = (HANDLE)_beginthreadex(NULL,2048,processThread,(void *)fileNum,NULL,NULL);
#endif
#ifdef __APPLE__
		pthread_create(&thread[fileNum-1],NULL,processThread,(void *)fileNum);
#endif
	}
	
#ifdef _WIN32
	WaitForMultipleObjects(argc-1,thread,TRUE,INFINITE);
#endif
#ifdef __APPLE__
	for(int i=0;i<argc-1;i++)
	{
		pthread_join(thread[i],NULL);
	}
#endif
	printf("\n변환 완료. 아무 키나 누르면 종료됩니다.");
	getch();
	return 0;
}
