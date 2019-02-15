#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<Windows.h>
#include<algorithm>

using namespace std;


class funcList
{
public:
	friend const bool operator<(const funcList &a,const funcList &b);
	wstring *hwp;
	wstring *Tex;
	bool operator<(const funcList &Other)
	{
		int n = _wcsicmp(hwp->data(),Other.hwp->data());
		if(n)
			return n<0;
		return wcscmp(hwp->data(),Other.hwp->data()) < 0;
	}
};

class compareStrIgnore
{
public:
	bool operator()(wstring &str1,wstring &str2)
	{
		return wcsicmp(str1.data(),str2.data())<0;
	}
};

extern int spcsize;
extern int funcsize;
extern int spcnextsize;
extern int spcunicodesize;
extern int remainsize;
extern int hwpParOutSize;
extern int oneparsize;
extern int twoparsize;

extern vector<wstring> texFuncSorted;
extern vector<wstring> hwpParameterOutKeyword;
extern vector<funcList> funcSorted;
extern vector<funcList> funcRemainSorted;
extern vector<wstring> funcSorted_t;
extern vector<wstring> funcRemainSorted_t;
extern vector<funcList> func;

extern wstring oneParameter[];
extern wstring twoParameter[];
extern wstring needToWrap[];
extern wchar_t openpartmp[];
extern wchar_t endpartmp[];
extern wstring spchwp[];
extern wstring spcTex[];
extern wstring funchwp[];
extern wstring funcTex[];
extern wstring spchwpNext[];
extern wstring spcTexNext[];

extern wchar_t spcUnicode1[];
extern wchar_t spcUnicode2[];
extern wchar_t spcUnicode3[];
extern wchar_t spcUnicode4[];
extern wchar_t spcUnicode5[];
extern wchar_t spcUnicode6[];
extern wchar_t *hwpUnicodeSpc[];
extern wchar_t *texUnicodeSpc[];
extern int hwpUnicodeSpcLen[];
extern int texUnicodeSpcLen[];

extern wstring funcRemain[];
extern wstring funcRemainNext[];
extern wstring parameterWrap[];
extern wstring doubleSubSupList[];

extern const bool operator<(const funcList &a,const funcList &b);
extern inline int findParMatch(wstring &str,int start);
extern bool getParameterRange(wstring &str,int start,int *from,int *to);
extern bool getParameterRangeBack(wstring &str,int start,int *from,int *to);
extern int smartInsert(wstring &str,int pos,wchar_t *p,int *insertedPos=NULL);
extern int bSearchAlphabet(wstring &source,int start,vector<funcList> &ar,int num);
extern int bSearchOutParameter(wstring &source,int start,vector<wstring> &arr);
extern void getFileNameOnly(const char *path,char *namebuf);
extern inline bool isAlphabet(wchar_t p);
extern inline int compareSubStr(const wchar_t *str1,const wchar_t *str2,int num,int *overlapNum);
extern inline void findNotSpace(wstring &str,int &val);
extern inline void findNotSpaceBack(wstring &str,int &val);
extern inline void findRangeStart(wstring &str,int start,int &val);		//현재 위치에서 중괄호로 둘러싸인 영역의 끝을 찾음
extern inline void findRangeEnd(wstring &str,int start,int &val);	//현재 위치에서 중괄호로 둘러싸인 영역의 시작점을 찾음

extern int deleqalign(wstring &str);
extern int wrapSubSup(wstring &str);
extern int wrapSqrt(wstring &str);
extern int wrapAnother(wstring &str);
extern int addSpaceAfterLRPar(wstring &str);
extern int overToFrac(wstring &str);
extern int setrmBound(wstring &str,int from,int to);
extern int setcaseBound(wstring &str);
extern int setmatrixBound(wstring &str);
extern int setLRPar(wstring &str);
extern int checkSmallPar(wstring &str);
extern int autoalign(wstring &str);
extern void changepile(wstring &str);
extern void deleteOverusedPar(wstring &str);
extern void checkDoubleSub(wstring &str);
extern void deleteMultipleSpace(wstring &str);
extern void hwpEqToTex(wstring &str);
extern void initializeEqValue(int argc,char **argv);