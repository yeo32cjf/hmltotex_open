#include "hwpEqToTex.h"

#define OPENPARTMP 1
#define CLOSEPARTMP 2

int spcsize;
int funcsize;
int spcnextsize;
int spcunicodesize;
int remainsize;
int hwpParOutSize;
int oneparsize;
int twoparsize;


vector<wstring> texFuncSorted;
vector<wstring> hwpParameterOutKeyword;

vector<funcList> funcSorted;
vector<funcList> funcRemainSorted;

vector<wstring> funcSorted_t;
vector<wstring> funcRemainSorted_t;

vector<funcList> func;


wstring oneParameter[] = {L"\\sqrt",L"\\mrm",L"\\vec",L"\\vrm",L"\\ovr",L"\\ovl",L"\\begintabu"
							,L"\\sqrt",L"_",L"^",L"\\begin",L"\\end",L"\\framebox"
					,L"\\overleftrightarrow",L"\\acute",L"\\grave",L"\\ddot",L"\\dot"
					,L"\\underline",L"\\hat",L"\\check",L"\\arc",L"\\widetilde"};
wstring twoParameter[] = {L"\\dfrac"};

//괄호로 인수가 묶여있는지 확인해야 하는 경우
wstring needToWrap[] = {L"\\ovr",L"\\vec",L"\\framebox"
					,L"\\overleftrightarrow",L"\\acute",L"\\grave",L"\\ddot",L"\\dot"
					,L"\\underline",L"\\hat",L"\\check",L"\\arc",L"\\widetilde"};

wchar_t openpartmp[] = {OPENPARTMP,0};        //여는 중괄호 대체 문자열
wchar_t endpartmp[] = {CLOSEPARTMP,0};        //닫는 중괄호 대체 문자열

wstring spchwp[] = {L"&lt;",L"&gt;",L"&amp;"};
wstring spcTex[] = {L"<",L">",L"&"};

wstring funchwp[] = {L"sinh",L"cosh",L"tanh",L"sech",L"csch",L"cosech",L"coth"
	,L"arcsin",L"arccos",L"arctan"
	,L"csc",L"cosec",L"sec",L"cot",L"sin",L"cos",L"tan",L"log",L"ln",L"lg"
	,L"max",L"min",L"arc",L"arg",L"Exp",L"exp",L"det",L"gcd",L"mod"
	,L"if",L"for",L"and",L"hom",L"ker",L"deg",L"dim",L"DEG"

	,L"APPROX",L"SIMEQ",L"CONG",L"ASYMP",L"ISO",L"DIAMOND",L"DSUM",L"FORALL"
	,L"Partial",L"LNOT",L"PROPTO",L"XOR",L"NABLA",L"DDAGGER",L"DAGGER"

	,L"CDOTS",L"LDOTS",L"VDOTS",L"DDOTS",L"TRIANGLE",L"MSANGLE",L"SANGLE",L"RTANGLE"
	,L"VDASH",L"DASHV",L"TOP",L"MODELS",L"LAPLACE",L"CENTIGRADE",L"FAHRENHEIT"
	,L"LSLANT",L"RSLANT",L"ATT",L"HUND",L"THOU",L"WELL",L"BASE",L"BENZENE"

	,L"SIM",L"INF",L"INFTY",L"rightarrow"
	,L"BULLET",L"AST",L"STAR",L"BIGCIRC",L"THEREFORE",L"BECAUSE",L"IDENTICAL",L"EXIST",L"DOTEQ",L"image",L"REIMAGE"
	,L"NOTOWNS",L"notOWNS",L"NOTIN",L"OWNS",L"SQSUBSETEQ",L"SQSUBSET",L"UPLUS"
	,L"SMALLSUM",L"SMALLPROD",L"SMCOPROD",L"SQCAP",L"SQCUP"
	,L"OPLUS",L"OMINUS",L"OTIMES",L"ODIV",L"ODOT",L"LOR",L"LAND"
	,L"SUBSETEQ",L"SUPSETEQ",L"SUBSET",L"SUPERSET"

	,L"larrow",L"rarrow",L"uparrow",L"downarrow",L"LARROW",L"LRARROW",L"DOWNARROW",L"UPARROW",L"udarrow"
	,L"UDARROW",L"RARROW",L"NWARROW",L"SEARROW",L"NEARROW",L"SWARROW"
	,L"HOOKLEFT",L"HOOKRIGHT",L"MAPSTO",L"PVER"

	,L"CUP",L"CAP",L"SMALLINTER",L"SMALLUNION",L"EMPTYSET",L"cdot",L"CDOT"

	,L"IN",L"SUCC",L"PREC"

	,L"ALEPH",L"hbar",L"imath",L"jmath",L"Ω",L"LITER",L"WP",L"ANGSTROM"
	,L"vartheta",L"varpi",L"varsigma",L"varupsilon",L"varphi",L"varepsilon"

	,L"LEFT",L"RIGHT"

	,L"bar",L"vec",L"box"
	,L"dyad",L"acute",L"grave",L"ddot",L"dot",L"under",L"hat",L"check",L"arch",L"tilde"

	,L"ANGLE",L"rm",L"it",L"BOT"

	,L"ALPHA",L"BETA",L"GAMMA",L"DELTA",L"EPSILON",L"ZETA",L"ETA",L"THETA"
	,L"IOTA",L"KAPPA",L"LAMBDA",L"MU",L"NU",L"XI",L"PI",L"RHO"
	,L"SIGMA",L"TAU",L"UPSILON",L"PHI",L"CHI",L"PSI",L"OMEGA"
	,L"alpha",L"beta",L"gamma",L"delta",L"epsilon",L"zeta",L"eta",L"theta"
	,L"iota",L"kappa",L"lambda",L"mu",L"nu",L"xi",L"pi",L"rho"
	,L"sigma",L"tau",L"upsilon",L"phi",L"chi",L"psi",L"omega"

	,L"sum",L"prod",L"coprod",L"inter",L"union",L"BIGSQCAP",L"BIGSQCUP"
	,L"BIGOPLUS",L"BIGOMINUS",L"BIGOTIMES",L"BIGODIV",L"BIGODOT",L"BIGUPLUS"

	,L"int",L"dint",L"tint",L"oint",L"odint",L"otint"

	,L"leq",L"geq",L"le",L"ge"
	,L"sqrt",L"root",L"lim",L"Lim",L"over"
	,L"times",L"prime",L"circ"

	,L"pile"
};


wstring funcTex[] = {L"\\sinh",L"\\cosh",L"\\tanh"
	,L"\\operatorname{sech}",L"\\operatorname{sech}",L"\\operatorname{cosech}",L"\\operatorname{coth}"
	,L"\\arcsin",L"\\arccos",L"\\arctan"
	,L"\\csc",L"\\operatorname{cosec}",L"\\sec",L"\\cot",L"\\sin",L"\\cos",L"\\tan",L"\\log",L"\\ln",L"\\lg"
	,L"\\max",L"\\min",L"\\mrm{arc}",L"\\arg",L"\\operatorname{Exp}",L"\\exp",L"\\det",L"\\gcd",L"\\bmod"
	,L"\\mrm{if}",L"\\mrm{for}",L"\\mrm{and}",L"\\hom",L"\\ker",L"\\deg",L"\\dim",L"^{\\circ}"

	,L"\\approx",L"\\simeq",L"\\cong",L"\\asymp",L"\\Bumpeq",L"\\diamond",L"\\dotplus",L"\\forall"
	,L"\\partial",L"\\lnot",L"\\propto",L"\\veebar",L"\\nabla",L"\\ddagger",L"\\dagger"

	,L"\\cdots",L"\\ldots",L"\\vdots",L"\\ddots",L"\\triangle",L"\\measuredangle",L"\\sphericalangle",L"rtangle"
	,L"\\vdash",L"\\dashv",L"\\top",L"\\models",L"\\laplace",L"\\centigrade",L"\\fahrenhe"
	,L"\\diagup",L"\\diagdown",L"att",L"hund",L"thou",L"well",L"base",L"benzene"

	,L"\\sim",L"\\infty",L"\\infty",L"\\to"
	,L"\\bullet",L"\\ast",L"\\star",L"\\bigcirc",L"\\therefore",L"\\because",L"identical",L"\\exists",L"\\doteq",L"\\fallingdotseq",L"\\risingdotseq"
	,L"\\not\\ni",L"\\not\\ni",L"\\notin",L"\\ni",L"\\sqsubseteq",L"\\sqsubset",L"\\uplus"
	,L"\\Sigma",L"\\Pi",L"\\amalg",L"\\sqcap",L"\\sqcup"
	,L"\\oplus",L"\\ominus",L"\\otimes",L"\\oslash",L"\\odot",L"\\lor",L"\\land"
	,L"\\subseteq",L"\\supseteq",L"\\subset",L"\\supset"

	,L"\\leftarrow",L"\\rightarrow",L"\\uparrow",L"\\downarrow",L"\\Leftarrow",L"\\Leftrightarrow",L"\\Downarrow",L"\\Uparrow",L"\\updownarrow"
	,L"\\Updownarrow",L"\\Rightarrow",L"\\nwarrow",L"\\searrow",L"\\nearrow",L"\\swarrow"
	,L"\\hookleftarrow",L"\\hookrightarrow",L"\\mapsto",L"\\Vert"

	,L"\\cup",L"\\cap",L"\\cap",L"\\cup",L"\\varnothing",L"\\bcd",L"\\bcd"

	,L"\\in",L">",L"<"

	,L"\\aleph",L"\\hbar",L"\\imath",L"\\jmath",L"\\Omega",L"\\ell",L"\\wp",L"ANGSTROM"
	,L"\\vartheta",L"\\varpi",L"\\varsigma",L"\\varupsilon",L"\\varphi",L"\\varepsilon"

	,L"\\left",L"\\right"

	,L"\\ovl",L"\\vec",L""	//L"\\framebox"
	,L"\\overleftrightarrow",L"\\acute",L"\\grave",L"\\ddot",L"\\dot",L"\\underline",L"\\hat",L"\\check",L"\\arc",L"\\widetilde"

	,L"\\angle",L"\\xrm",L"\\it",L"\\bot"

	,L"A",L"B",L"\\Gamma",L"\\Delta",L"E",L"Z",L"H",L"\\Theta"
	,L"I",L"K",L"\\Lambda",L"M",L"N",L"\\Xi",L"\\Pi",L"P"
	,L"\\Sigma",L"T",L"Y",L"\\Phi",L"X",L"\\Psi",L"\\Omega"
	,L"\\alpha",L"\\beta",L"\\gamma",L"\\delta",L"\\epsilon",L"\\zeta",L"\\eta",L"\\theta"
	,L"\\iota",L"\\kappa",L"\\lambda",L"\\mu",L"\\nu",L"\\xi",L"\\pi",L"\\rho"
	,L"\\sigma",L"\\tau",L"\\upsilon",L"\\phi",L"\\chi",L"\\psi",L"\\omega"

	,L"\\sum",L"\\prod",L"\\coprod",L"\\bigcap",L"\\bigcup",L"\\bigsqcap",L"\\bigsqcup"
	,L"\\bigoplus",L"BIGOMINUS",L"\\bigotimes",L"BIGODIV",L"\\bigodot",L"\\biguplus"

	,L"\\int",L"\\iint",L"\\iiint",L"\\oint",L"oiint",L"oiiint"

	,L"\\le",L"\\ge",L"\\le",L"\\ge"
	,L"\\sqrt",L"\\sqrt",L"\\lim",L"\\operatorname{Lim}",L"\\kfrac"
	,L"\\times",L"'",L"\\circ"

	,L"\\pile"
};

wstring spchwpNext[] = {L",",L"%",L"-+",L"!=",L"<->",L"->"
	,L"<<<",L">>>",L"<<",L">>",L"==",L"<=",L">=",L"+-",L"-+"
};
wstring spcTexNext[] = {L",\\: ",L"\\% ",L"\\mp ",L"\\ne ",L"\\leftrightarrow ",L"\\to "
	,L"\\lll ",L"\\ggg ",L"\\ll ",L"\\gg ",L"\\equiv ",L"\\le ",L"\\ge ",L"\\pm ",L"\\mp "
};

//± ÷ ⊒ ⊐  °
wchar_t spcUnicode1[] = {0xC2,0xB1,0x00};
wchar_t spcUnicode2[] = {0xC3,0xB7,0x00};
wchar_t spcUnicode3[] = {0xE2,0x8A,0x92,0x00};
wchar_t spcUnicode4[] = {0xE2,0x8A,0x90,0x00};
wchar_t spcUnicode5[] = {0xEE,0x81,0x8D,0x00};
wchar_t spcUnicode6[] = {0xC2,0xB0,0x00};
wchar_t spcUnicode7[] = {0xE2,0x88,0xA0,0x00};

wchar_t *hwpUnicodeSpc[] = {spcUnicode1,spcUnicode2,spcUnicode3,spcUnicode4,spcUnicode5,spcUnicode6,spcUnicode7};
wchar_t *texUnicodeSpc[] = {L"\\pm ",L"\\div ",L"\\sqsupseteq ",L"\\sqsupset ",L"\\vert ",L"^{\\circ}",L"\\angle "};

int hwpUnicodeSpcLen[] = {2,2,3,3,3,2,3};
int texUnicodeSpcLen[] = {4,5,12,10,6,8,7};

wstring funcRemain[] = {L"\\xrm",L"\\beginttabuu",L"\\laplace",L"\\centigrade",L"\\fahrenhe"
	,L"$ ",L" $",L"{ ",L" {",L" }",L"} ",L"( ",L" (",L" )",L") ",L"[ ",L" [",L" ]",L"] "
	,L" \\",L" '",L"' ",L" ^",L"^ ",L" _",L"_ "
	,L"\\mrm{}",L"#",L"\\it",L"~",L"\n ",L"\n\n"};

wstring funcRemainNext[] = {L"\\mrm",L"\\begin{tabu}",L"\\mathcal{L}",L"^{\\circ}\\mrm{C}",L"{}^{\\circ}\\mrm{F}"
	,L"$",L"$",L"{",L"{",L"}",L"}",L"(",L"(",L")",L")",L"[",L"[",L"]",L"]"
	,L"\\",L"'",L"'",L"^",L"^",L"_",L"_"
	,L"",L"\\\\",L"",L"",L"\n",L"\n"};





wstring parameterWrap[] = {L"sinh",L"cosh",L"tanh",L"sech",L"csch",L"cosech",L"coth"
	,L"arcsin",L"arccos",L"arctan"
	,L"csc",L"cosec",L"sec",L"cot",L"sin",L"cos",L"tan",L"log",L"ln",L"lg"
	,L"max",L"min",L"arg",L"Exp",L"exp",L"gcd",L"mod"
	,L"if",L"for",L"and",L"hom",L"ker",L"deg",L"dim"

	,L"NOTOWNS",L"notOWNS"

	,L"LEFT",L"RIGHT"

	,L"bar",L"vec",L"box"
	,L"dyad",L"acute",L"grave",L"ddot",L"dot",L"under",L"hat",L"check",L"arch",L"tilde"

	,L"rm",L"it"

	,L"sum",L"prod",L"coprod",L"inter",L"union",L"BIGSQCAP",L"BIGSQCUP"
	,L"BIGOPLUS",L"BIGOMINUS",L"BIGOTIMES",L"BIGODIV",L"BIGODOT",L"BIGUPLUS"

	,L"int",L"dint",L"tint",L"oint",L"odint",L"otint"

	,L"sqrt",L"root",L"lim",L"Lim",L"over"

	,L"pile"
};

wstring doubleSubSupList[] = {L"\\sum",L"\\prod",L"\\int",L"\\iint",L"\\iiint",L"\\oint"
	,L"\\bigcup",L"\\bigcap", L"\\bigsqcup", L"\\biguplus"
	,L"\\bigvee  \\bigwedge  \\bigoplus",L" \\bigotimes",L" \\bigodot"
};


const bool operator<(const funcList &a,const funcList &b)
{
	int n = _wcsicmp(a.hwp->data(),b.hwp->data());
	if(n)
		return n<0;
	return wcscmp(a.hwp->data(),b.hwp->data()) < 0;
}


void initializeEqValue(int argc,char **argv)
{
	spcsize = sizeof(spchwp)/sizeof(wstring);	//특수기호 개수
	funcsize = sizeof(funchwp)/sizeof(wstring);	//
	spcnextsize = sizeof(spchwpNext)/sizeof(wstring);
	spcunicodesize = sizeof(hwpUnicodeSpc)/sizeof(wchar_t *);
	remainsize = sizeof(funcRemain)/sizeof(wstring);
	hwpParOutSize = sizeof(parameterWrap)/sizeof(wstring);

	oneparsize = sizeof(oneParameter)/sizeof(wstring);
	twoparsize = sizeof(twoParameter)/sizeof(wstring);
	
	
	//한글 수식에서 의미가 있는 문자열 읽기
	for(int i=0;i<funcsize;i++)
	{
		funcSorted.push_back(funcList());
		funcSorted[i].hwp = &funchwp[i];
		funcTex[i] += CLOSEPARTMP;
		funcSorted[i].Tex = &funcTex[i];
	}

	for(int i=0;i<funcsize;i++)
	{
		if( isAlphabet(funcTex[i][0]) == false)
		{
			texFuncSorted.push_back(funcTex[i]);

			int n = texFuncSorted.back().find(' ');
			if(n != wstring::npos)
				texFuncSorted.back().erase(n,1);
		}
	}
	texFuncSorted.push_back(L"\\circ");

	sort(texFuncSorted.begin(),texFuncSorted.end(),compareStrIgnore());

	for(int i=0;i<hwpParOutSize;i++)
	{
		hwpParameterOutKeyword.push_back(parameterWrap[i]);
	}
	sort(hwpParameterOutKeyword.begin(),hwpParameterOutKeyword.end(),compareStrIgnore());

	//항상 바뀌어야 하는 문자열 읽기
	for(int i=0;i<remainsize;i++)
	{
		funcRemainSorted.push_back(funcList());
		funcRemainSorted[i].hwp = &funcRemain[i];
		funcRemainSorted[i].Tex = &funcRemainNext[i];
	}

	//이분 검색을 위해 정렬
	sort(funcSorted.begin(),funcSorted.end());
	sort(funcRemainSorted.begin(),funcRemainSorted.end());

}


bool bSearchTexFunc(wstring &str)
{
	int upper,lower,mid;

	lower = 0;
	upper = texFuncSorted.size()-1;

	while(1)
	{
		mid = (upper+lower)/2;
		int res = wcsicmp(texFuncSorted[mid].data(),str.data());

		if (res == 0)
			return true;

		if (res > 0)
			upper=mid-1;
		else
			lower=mid+1;

		if (upper < lower)
			return false;
	}
}

inline int findParMatch(wstring &str,int start)	//str[start]가 {라고 가정
{
	int parLev = 1;
	for(int i=start+1;str[i];i++)
	{
		if(str[i] == '{')
			++parLev;
		else if(str[i] == '}')
			--parLev;

		if(parLev == 0)
			return i+1;
	}
}


bool getParameterRange(wstring &str,int start,int *from,int *to)
{
	int n = start;

	int res = 0;
	*from = n;

	vector<int> pos;
	pos.push_back(n);

	while(pos.empty() == false)
	{
		n = pos.back();
		pos.pop_back();
		findNotSpace(str,n);

		//최초의 공백 아닌 문자가 여는 괄호일 때
		if(str[n] == '{')
		{
			int parLev = 1;

			while(1)
			{
				++n;

				if(str[n] == '{')
					++parLev;
				else if(str[n] == '}')
					--parLev;
				else if(str[n] == '$')
				{
					if(parLev)
					{
						for(int i=0;i<parLev;i++)
						{
							str.insert(n,L"}");
							parLev = 0;
							break;
						}
					}
				}

				if(parLev == 0)
					break;
			}

			++n;
		}
		else
		{
			if(str[n] == OPENPARTMP)
				str[n++] = ' ';

			while(str[n] != '$')
			{
				if(str[n] == '{' || str[n] == '}' || str[n] == ' ' || str[n] == '^' || str[n] == '_' || str[n] == '`')
					break;
				++n;
			}
			++res;
		}

		int tmp = n;
		findNotSpace(str,tmp);

		if(str[tmp] == '^' || str[tmp] == '_')
		{
			pos.push_back(tmp+1);
			++res;
		}
	}
	*to = n;

	return (res==0);
}

bool getParameterRangeBack(wstring &str,int start,int *from,int *to)
{
	int n = start;

	int res = 0;
	*to = n;
	--n;

	vector<int> pos;
	pos.push_back(n);
	
	while(pos.empty() == false)
	{
		n = pos.back();
		pos.pop_back();
		findNotSpaceBack(str,n);

		//최초의 공백 아닌 문자가 닫는 괄호일 때
		if(str[n] == '}')
		{
			int parLev = -1;
			--n;

			while(n != 1)
			{
				if(str[n] == '{')
					++parLev;
				else if(str[n] == '}')
					--parLev;

				if(parLev == 0)
					break;

				--n;
			}
		}
		else
		{
			while(n != 0)
			{
				if(str[n] == '{' || str[n] == '}' || str[n] == ' ' || str[n] == '^' || str[n] == '_' || str[n] == '`')
					break;

				--n;
			}
			
			++n;
			++res;
		}

		int tmp = n-1;
		findNotSpaceBack(str,tmp);

		if(str[tmp] == '^' || str[tmp] == '_')
		{
			pos.push_back(tmp-1);
			++res;
		}
	}

	*from = n;

	return (res==0);
}

int smartInsert(wstring &str,int pos,wchar_t *p,int *insertedPos)
{
	int min=pos-1,max=pos;
	int len = wcslen(p);

	for(;str[min]==' ';min--){}
	for(;str[max]==' ';max++){}

	++min;
	//[min,max)는 공백의 범위

	if(max-min >= len)
	{
		for(int i=0;i<len;i++)
		{
			str[max-len+i] = p[i];
		}

		if(insertedPos)
			*insertedPos = max-len;

		return 0;
	}
	else
	{
		for(int i=0;i<max-min;i++)
		{
			str[min+i] = p[i];
		}

		str.insert(max,p+max-min);
		if(insertedPos)
			*insertedPos = min;

		return len-max+min;
	}
}


//wstring형 변수 source에서 start의 위치부터 함수명 검색 (함수명 배열:ar,크기:nunm)
//대소문자를 구분하지 않고 한글 함수명 검색, ar는 오름차순으로 정렬되어 있어야 함
int bSearchAlphabet(wstring &source,int start,vector<funcList> &ar,int num)
{
	int upper,lower,mid;

	lower = 0;
	upper = num-1;
	const wchar_t *src = source.data()+start;

	//wcout << source.data()+start << endl;

	while(1)
	{
		mid = (upper+lower)/2;

		wstring *midval = ar[mid].hwp;

		int overlapNum;
		int findRes = compareSubStr(src,midval->data(),midval->size(),&overlapNum);
		int direction = 0;	//대소문자를 고려해야 할 때, 검색 방향
		//wcout << *midval << " : " << overlapNum << " : " << findRes << endl;
		
		if(findRes == 0 || findRes != 0 && overlapNum>=2)	//mid의 위치에서 앞으로도 찾고 뒤로도 찾음
		{
			int m = mid;
			int prevOverlapNum = overlapNum;
			int nextOverlapNum = overlapNum;

			for(int i=m+1;i<num;i++)
			{
				wstring *nextval = ar[i].hwp;
				int oNum;
				int findNext = compareSubStr(src,nextval->data(),nextval->size(),&oNum);
				//wcout << "Next : " << *nextval << " : " << oNum << " : " << findNext << endl;
				if(oNum >= overlapNum)
				{
					if(findNext == 0)
					{
						findRes = 0;
						direction = 1;
						mid = i;
						midval = nextval;
					}
					nextOverlapNum = oNum;
				}
				else if(oNum < overlapNum)
					break;
			}

			if(findRes != 0)
			{
				for(int i=m-1;i>=0;i--)
				{
					wstring *prevval = ar[i].hwp;
					int oNum;
					int findPrev = compareSubStr(src,prevval->data(),prevval->size(),&oNum);
					//wcout << "Prev : " << *prevval << " : " << oNum << " : " << findPrev << endl;
					if(findPrev == 0)
					{
						findRes = 0;
						direction = -1;
						mid = i;
						midval = prevval;
					}
					else if(oNum<2)
						break;
				}
			}
		}

		if(findRes > 0)
			lower = mid+1;
		else if(findRes < 0)
			upper = mid-1;
		else
		{
			if(direction != 0)
			{
				wchar_t firstLattermid = (*midval)[0];
				wchar_t firstLattersrc = source[start];

				//source의 시작 문자, 검색된 문자열의 첫 문자가 모두 대문자이거나 모두 소문자일 때
				if(firstLattermid >= 'A' && firstLattermid <= 'Z' && firstLattersrc >= 'A' && firstLattersrc <= 'Z'
					|| firstLattermid >= 'a' && firstLattermid <= 'z' && firstLattersrc >= 'a' && firstLattersrc <= 'z')
					return mid;

				wstring *compval = ar[mid-direction].hwp;
				//wcout << "comp : " << direction << " : " << *midval << " , " << *compval << endl;

				//mid의 대문자가 prev인 경우
				if(_wcsicmp(midval->data(),compval->data()) == 0)
				{
					//prev는 대문자, mid는 소문자를 가리킴
					//대소문자를 무시하면 prev와 mid는 같음

					//source의 시작 문자가 대문자인 경우
					if(direction < 0 && firstLattersrc >= 'a' && firstLattersrc <= 'z'
						|| direction > 0 && firstLattersrc >= 'A' && firstLattersrc <= 'Z')
						return mid-direction;
					else
						return mid;
				}
			}
			return mid;
		}

		if(upper<lower)
			return wstring::npos;
	}
}


int bSearchOutParameter(wstring &source,int start,vector<wstring> &ar)
{
	int upper,lower,mid;

	int num = ar.size();
	lower = 0;
	upper = num-1;
	const wchar_t *src = source.data()+start;

	//wcout << source.data()+start << endl;

	while(1)
	{
		mid = (upper+lower)/2;

		wstring *midval = &ar[mid];

		int overlapNum;
		int findRes = compareSubStr(src,midval->data(),midval->size(),&overlapNum);
		int direction = 0;	//대소문자를 고려해야 할 때, 검색 방향
		//wcout << *midval << " : " << overlapNum << " : " << findRes << endl;
		
		if(findRes == 0 || findRes != 0 && overlapNum>=2)	//mid의 위치에서 앞으로도 찾고 뒤로도 찾음
		{
			int m = mid;
			int prevOverlapNum = overlapNum;
			int nextOverlapNum = overlapNum;

			for(int i=m+1;i<num;i++)
			{
				wstring *nextval = &ar[i];
				int oNum;
				int findNext = compareSubStr(src,nextval->data(),nextval->size(),&oNum);
				//wcout << "Next : " << *nextval << " : " << oNum << " : " << findNext << endl;
				if(oNum >= overlapNum)
				{
					if(findNext == 0)
					{
						findRes = 0;
						direction = 1;
						mid = i;
						midval = nextval;
					}
					nextOverlapNum = oNum;
				}
				else if(oNum < overlapNum)
					break;
			}

			if(findRes != 0)
			{
				for(int i=m-1;i>=0;i--)
				{
					wstring *prevval = &ar[i];
					int oNum;
					int findPrev = compareSubStr(src,prevval->data(),prevval->size(),&oNum);
					//wcout << "Prev : " << *prevval << " : " << oNum << " : " << findPrev << endl;
					if(findPrev == 0)
					{
						findRes = 0;
						direction = -1;
						mid = i;
						midval = prevval;
					}
					else if(oNum<2)
						break;
				}
			}
		}

		if(findRes > 0)
			lower = mid+1;
		else if(findRes < 0)
			upper = mid-1;
		else
		{
			if(direction != 0)
			{
				wchar_t firstLattermid = (*midval)[0];
				wchar_t firstLattersrc = source[start];

				//source의 시작 문자, 검색된 문자열의 첫 문자가 모두 대문자이거나 모두 소문자일 때
				if(firstLattermid >= 'A' && firstLattermid <= 'Z' && firstLattersrc >= 'A' && firstLattersrc <= 'Z'
					|| firstLattermid >= 'a' && firstLattermid <= 'z' && firstLattersrc >= 'a' && firstLattersrc <= 'z')
					return mid;

				wstring *compval = &ar[mid-direction];
				//wcout << "comp : " << direction << " : " << *midval << " , " << *compval << endl;

				//mid의 대문자가 prev인 경우
				if(_wcsicmp(midval->data(),compval->data()) == 0)
				{
					//prev는 대문자, mid는 소문자를 가리킴
					//대소문자를 무시하면 prev와 mid는 같음

					//source의 시작 문자가 대문자인 경우
					if(direction < 0 && firstLattersrc >= 'a' && firstLattersrc <= 'z'
						|| direction > 0 && firstLattersrc >= 'A' && firstLattersrc <= 'Z')
						return mid-direction;
					else
						return mid;
				}
			}
			return mid;
		}

		if(upper<lower)
			return wstring::npos;
	}
}


inline bool isAlphabet(wchar_t p)
{
	return p >= 'a' && p <= 'z' || p >= 'A' && p <= 'Z';
}


inline int compareSubStr(const wchar_t *str1,const wchar_t *str2,int num,int *overlapNum)
{
	for(int i=0;i<num;i++)
	{
		int c1 = tolower(str1[i]);
		int c2 = tolower(str2[i]);

		if(c1 != c2)
		{
			*overlapNum = i;
			return (c1 > c2) ? 1 : -1;
		}
	}
	*overlapNum = num;
	return 0;
}

inline void findNotSpace(wstring &str,int &val)
{
	for(;str[val]==' ' || str[val]==CLOSEPARTMP;val++);
}

inline void findNotSpaceBack(wstring &str,int &val)
{
	for(;str[val]==' ' || str[val]==CLOSEPARTMP;val--);
}

inline void findRangeStart(wstring &str,int start,int &val)
{
	int parLev = 0;
	int i;

	for(i=start;str[i]!='$';i--)
	{
		if(str[i] == '{' && str[i-1] != '\\')
			++parLev;
		else if(str[i] == '}' && str[i-1] != '\\')
			--parLev;

		if(parLev>0)
			break;
	}
	val = i+1;
}

inline void findRangeEnd(wstring &str,int start,int &val)
{
	int parLev = 0;
	int i;

	for(i=start;str[i]!='$';i++)
	{
		if(str[i] == '{' && str[i-1] != '\\')
			++parLev;
		else if(str[i] == '}' && str[i-1] != '\\')
			--parLev;

		if(parLev<0)
			break;
	}
	val = i;
}

int deleqalign(wstring &str)	//eqalign을 처리함
{
	int start = str.find(L"eqalign");
	if(start == wstring::npos)
		return 1;

	int end = 0;
	vector<int> eqstart;		//eqalign의 위치
	vector<int> eqmid;			//eqalign의 여는 괄호의 위치
	vector<int> eqend;			//eqalign의 닫는 괄호의 위치

	vector<int> searchBack;		//뒤로 검색 여부
	vector<int> searchNext;		//앞으로 검색 여부
	vector<int> isMatrix;		//true이면 행렬로, false이면 \begin{aligned} \end{aligned}로 수식을 표현
	vector<int> sharpNum;		//하나의 eqalign에서 #의 개수

	eqstart.push_back(0);
	eqmid.push_back(0);
	eqend.push_back(0);
	searchBack.push_back(false);
	searchNext.push_back(false);
	isMatrix.push_back(false);
	sharpNum.push_back(0);

	int i=1;
	while(1)			//수식 내에 존재하는 모든 eqalign을 찾음
	{
		end = start + 7;

		for(; str[end]!='{' ; end++){}    //처음으로 등장하는 여는 괄호를 찾음
		
		eqstart.push_back(start);
		eqmid.push_back(end);
		sharpNum.push_back(0);
		
		int parLevel = 0;
		while(1)    //닫는 괄호를 찾음
		{
			++end;
			switch(str[end])
			{
			case '{':
				++parLevel;
				break;
			case '}':
				--parLevel;
				break;
			case '#':
				if(parLevel==0)
					++sharpNum[i];
				break;
			}
			if(parLevel==-1)
				break;
		}
		eqend.push_back(end);
		start = end+1;
		searchBack.push_back(true);
		searchNext.push_back(true);
		isMatrix.push_back(true);

		start = str.find(L"eqalign",start);
		if(start==wstring::npos)
			break;
		++i;
	}

	eqstart.push_back(str.size()-1);
	eqmid.push_back(str.size()-1);
	eqend.push_back(str.size()-1);
	searchBack.push_back(true);
	searchNext.push_back(false);
	isMatrix.push_back(false);
	sharpNum.push_back(0);

	//i번째의 eqend와 i+1번째의 eqstart 사이에 &가 있으면 eqalign은 행렬처럼 사용된 것
	start = 1;		//start번째 eqalign과 end번째의 eqalign이 모두 행렬로 사용된 경우
	end = 1;

	for(i=1;i<eqstart.size()-1;i++)
	{
		int parLevel = 0;
		int beginLevel = 0;
		bool hasLinebreak = false;
		int startSearch = 0;

		int j=0;
		int k=0;
		int andnum = 0;		//인접한 eqalign 사이에 &가 1개인 경우, 행렬로 처리

		if(searchBack[i]==true)
		{
			parLevel = 0;
			beginLevel = 0;
			for(k=eqstart[i]-1;k>=eqend[i-1]+1;k--)
			{
				switch(str[k])
				{
				case '{':
					++parLevel;
					if(parLevel==0)
					{
						if(str.find(L"cases}",k+1,6) == k+1
							|| str.find(L"matrix}",k+2,7) == k+2
							|| str.find(L"tabu}",k+1,5) == k+1)
						{
							if(str.find(L"\\begin",k-6,6) == k-6)
							{
								k-=6;
								++beginLevel;
							}
							else if(str.find(L"\\end",k-4,4) == k-4)
							{
								k-=4;
								--beginLevel;
								isMatrix[i] = false;
								searchNext[i] = false;
							}
						}
						else if(str[k+1] == 'c')
						{
							int a;
							for(a=k+2;str[a]!='c';a++){};

							if(str[a] == '}' && k>=12)		//\begin{tabu}{c...}
							{
								if(str.find(L"\\begin{tabu}",k-12,12) != k-12)
								{
									isMatrix[i] = false;
									searchNext[i] = false;
								}
							}
							else
							{
								isMatrix[i] = false;
								searchNext[i] = false;
							}
						}
						else
						{
							isMatrix[i] = false;
							searchNext[i] = false;
						}
					}
					else if(parLevel>0)
					{
						isMatrix[i] = false;
						searchNext[i] = false;
					}
					break;
				case '}':
					--parLevel;
					break;
				case '&':
					if(beginLevel==0)
					{
						isMatrix[i] = false;
						searchNext[i] = false;
					}
					break;
				case '#':
					if(beginLevel==0)
						hasLinebreak = true;
					break;
				case ' ':
				case '\n':
				case '\r':
					break;
				default:
					if(parLevel==0)
					{
						isMatrix[i] = false;
						searchNext[i] = false;
					}
					break;
				}
				if(parLevel==1 || beginLevel==1 || hasLinebreak==true || searchNext[i]==false)
					break;
			}
		}

		hasLinebreak = false;
		if(searchNext[i]==true && isMatrix[i]==true)
		{
			parLevel = 0;
			beginLevel = 0;
			for(j=eqend[i]+1;j<eqstart[i+1];j++)
			{
				switch(str[j])
				{
				case '{':
					++parLevel;
					isMatrix[i] = false;
					break;
				case '}':
					--parLevel;
					if(parLevel<0)
					{
						isMatrix[i] = false;
					}
					break;
				case '&':
					if(beginLevel==0)
					{
						isMatrix[i] = true;
						++andnum;
					}
					break;
				case '#':
					if(beginLevel==0)
						hasLinebreak = true;
					break;
				case '\\':
					if(str.find(L"end",j+1,3) == j+1)
					{
						j+=3;
						--beginLevel;
					}
					else if(str.find(L"begin",j+1,5) == j+1)
					{
						++beginLevel;
						j+=5;
					}
					break;
				default:
					if(beginLevel==0)
						isMatrix[i] = false;
					break;
				}
				if(parLevel==-1 || beginLevel==-1 || hasLinebreak==true || isMatrix[i]==false)
					break;
			}
		}

		if(j==eqstart[i+1] && andnum==1)	//행렬처럼 취급
			searchBack[i+1] = false;
		
		if(isMatrix[i])
		{
			++end;

			if(searchBack[i+1] == true)
			{
				vector<vector<wstring>> arr;
				wstring tmp = L"";
				for(int a=start;a<end;a++)
				{
					arr.push_back(vector<wstring>());
					int startpos = eqmid[a]+1;
					int b=startpos;
					for(;b<eqend[a];b++)
					{
						switch(str[b])
						{
						case '{':
							++parLevel;
							break;
						case '}':
							--parLevel;
							break;
						case '#':
							if(parLevel==0)
							{
								arr[a-start].push_back(str.substr(startpos,b-startpos));
								startpos = b+2;		//# 다음에 \n이 반드시 온다
							}
							break;
						}
					}
					arr[a-start].push_back(str.substr(startpos,b-startpos));
				}

				for(int a=0;a<sharpNum[i]+1;a++)
				{
					for(int b=0;b<arr.size();b++)
					{
						int k = arr[b][a].find('\n');
						if(k!=wstring::npos)
							arr[b][a].erase(k,1);
						k = arr[b][a].find('\r');
						if(k!=wstring::npos)
							arr[b][a].erase(k,1);
						tmp += arr[b][a];
						if(b != arr.size()-1)
						{
							tmp += L" & ";
						}
					}
					if(a != sharpNum[i])
					{
						tmp += L" # \n";
					}
				}
				str.replace(eqstart[start],eqend[end-1]-eqstart[start]+1,tmp);
				
				int d = tmp.size() - (eqend[end-1]-eqstart[start]+1);
				eqend[i] = eqstart[start] + tmp.size();
				for(int a=i+1;a<eqstart.size();a++)
				{
					eqstart[a] += d;
					eqmid[a] += d;
					eqend[a] += d;
				}

				start = i+1;
				end = i+1;
			}
		}
		else
		{
			str.replace(eqend[i],1,L"\\end{aligned}");
			str.replace(eqstart[i],eqmid[i]-eqstart[i]+1,L"\\begin{aligned}");
			int d = 28-(eqmid[i]-eqstart[i]+2);
			eqend[i] += d-1;
			for(int a=i+1;a<eqstart.size();a++)
			{
				eqstart[a] += d;
				eqmid[a] += d;
				eqend[a] += d;
			}
			start = i+1;
			end = i+1;
		}
	}
	return 1;
}

int wrapSubSup(wstring &str)	//지수와 밑첨자를 {}로 감쌈
{
	int startPos = 0;
	while(1)
	{
		startPos = str.find(L"^",startPos);

		if(startPos == wstring::npos)
			break;

		int start,end;
		
		if(getParameterRange(str,startPos+1,&start,&end) == false)
		{
			str.insert(end,L"}");
			str.insert(start,L"{");
		}

		startPos = start;
	}

	startPos = 0;
	while(1)
	{
		startPos = str.find(L"_",startPos);

		if(startPos == wstring::npos)
			break;

		bool startWithPar = false;
		int n = startPos+1;
		findNotSpace(str,n);

		if(str[n] == '{')
		{
			startWithPar = true;
			int parLev = 1;
			while(1)
			{
				++n;

				if(str[n] == '{')
					++parLev;
				else if(str[n] == '}')
					--parLev;

				if(parLev == 0)
					break;
			}
		}
		else
		{
			while(str[n] != '$')
			{
				if(str[n] == '{' || str[n] == '}' || str[n] == ' ' || str[n] == '^' || str[n] == '_')
					break;
				++n;
			}
		}

		findNotSpace(str,n);

		if(str[n] == '_')
		{
			int start,end;
			getParameterRange(str,n+1,&start,&end);
			str.insert(end,L"}");
			str.insert(startPos+1,L"{");
		}
		else
		{
			if(startWithPar == false)
			{
				str.insert(n,L"}");
				str.insert(startPos+1,L"{");
			}
		}
		
		++startPos;
	}

	return 1;
}

int wrapSqrt(wstring &str)	//\sqrt를 {}로 감쌈
{
	int startPos = str.size();
	while(1)
	{
		startPos = str.rfind(L"\\sqrt",startPos-1);
		if(startPos == wstring::npos)
			break;

		int start,end;
		
		bool p1 = getParameterRange(str,startPos+6,&start,&end);

		int n = end;

		findNotSpace(str,n);
		//for(;str[n]==' ' || str[n]==CLOSEPARTMP;n++);
		if(str[n] == 'o' && str[n+1] == 'f')
		{
			str[n] = str[n+1] = ' ';
			int nextStart,nextEnd;

			
			bool p2 = getParameterRange(str,n+2,&nextStart,&nextEnd);

			if(p2 == false)
			{
				if(smartInsert(str,nextEnd,L"}"))
					++nextEnd;
				if(smartInsert(str,nextStart,L"{"))
					++nextEnd;
			}
			if(p1 == false)
			{
				if(smartInsert(str,end,L"]"))
					++nextEnd;
				if(smartInsert(str,start,L"["))
					++nextEnd;
			}
			else
			{
				for(;str[start]!='{';start++);
				str[start] = '[';
				str[end-1] = ']';
			}

			smartInsert(str,nextEnd,L"}");
		}
		else
		{
			if(p1 == false)
			{
				smartInsert(str,end,L"}",&end);
				if(smartInsert(str,start,L"{"))
					++end;
			}
			smartInsert(str,end,L"}");
		}
		smartInsert(str,startPos,L"{");
	}

	return 1;
}

int wrapAnother(wstring &str)
{
	int size = sizeof(needToWrap)/sizeof(wstring);
	vector<pair<int,int>> position;

	for(int i=0;i<size;i++)
	{
		int pos = 0;
		while(1)
		{
			pos = str.find(needToWrap[i],pos);
			if(pos == wstring::npos)
				break;
			
			position.push_back(pair<int,int>(pos,needToWrap[i].size()));

			pos += needToWrap[i].size();
		}
	}

	sort(position.begin(),position.end());


	for(int i=position.size()-1;i>=0;i--)
	{
		int start,end;
		
		bool p = getParameterRange(str,position[i].first+position[i].second,&start,&end);

		if(p == false)
		{
			if(smartInsert(str,end,L"}"))
				++end;
			if(smartInsert(str,start,L"{"))
				++end;
		}

		smartInsert(str,end,L"}");
		smartInsert(str,position[i].first,L"{");
	}

	return 1;
}

int addSpaceAfterLRPar(wstring &str)	//\left 다음과 \right 이전에 공백이 없으면 공백 추가
{
	int pos = 0;
	while(1)
	{
		pos = str.find(L"\\left",pos);
		if(pos == wstring::npos)
			break;

		pos += 5;
		findNotSpace(str,pos);
		
		//처음으로 공백이 아닌 문자를 찾음
		if(str[pos+1] != ' ')
		{
			if(str[pos] != '\\')
				str.insert(++pos,1,' ');
		}
	}

	pos = 0;
	while(1)
	{
		pos = str.find(L"\\right",pos);
		if(pos == wstring::npos)
			break;

		if(str[pos-1] != ' ')
			str.insert(pos,1,' ');
		pos += 7;
	}

	return 0;
}

int overToFrac(wstring &str)
{
	int pos = 0;

	while(1)
	{
		pos = str.find(L"\\kfrac",pos);

		if(pos == wstring::npos)
			break;

		int frontParStart,frontParEnd;
		int backParStart,backParEnd;

		bool f = getParameterRange(str,pos+6,&frontParStart,&frontParEnd);
		bool b = getParameterRangeBack(str,pos,&backParStart,&backParEnd);

		//cout << backParStart << " " << backParEnd << " " << frontParStart << " " << frontParEnd << " " << endl;
		//wcout << str << endl;

		if(f == false)
		{
			smartInsert(str,frontParEnd,L"}",&frontParEnd);
			++frontParEnd;
			if(smartInsert(str,frontParStart,L"{",&frontParStart))
				++frontParEnd;
		}

		if(b == false)
		{
			if(smartInsert(str,backParEnd,L"}",&backParEnd))
			{
				++frontParEnd;
				++frontParStart;
			}
			if(smartInsert(str,backParStart,L"{",&backParStart))
			{
				++frontParEnd;
				++frontParStart;
				++backParEnd;
			}

			++backParEnd;
		}

		//wcout << backParStart << L" " << backParEnd << L" " << frontParStart << L" " << frontParEnd << L" " << endl;
		//wcout << str << endl;
		

		for(int i=0;i<backParEnd-backParStart;i++)
		{
			str[frontParStart-1-i] = str[backParEnd-1-i];
		}

		str[backParStart] = '\\';
		str[backParStart+1] = 'd';
		str[backParStart+2] = 'f';
		str[backParStart+3] = 'r';
		str[backParStart+4] = 'a';
		str[backParStart+5] = 'c';

		for(int i=backParStart+6;str[i]!='{';i++)
			str[i] = ' ';

		smartInsert(str,frontParEnd,L"}");
		smartInsert(str,backParStart,L"{");

		pos = backParStart+5; 
		//wcout << str << endl << endl;
	}

	return 1;
}

int setrmBound(wstring &str,int from,int to)    //중괄호가 없다고 가정하고 문자열 변환
{
	vector<int> isRm;
	vector<int> type;

	isRm.push_back(false);
	//wcout << str << endl;

	//각각의 문자열의 type을 저장함
	//0:rm영향 안받는 문자, 1:이탤릭, 2:rm
	for(int i=0;i<str.size();i++)
	{
		if(str[i] == '{')
		{
			type.push_back(0);
			isRm.push_back(isRm.back());
		}
		else if(str[i] == '}')
		{
			type.push_back(0);
			isRm.pop_back();
		}
		else if(str[i] == '\\')
		{
			int min = i;

			type.push_back(0);
			++i;

			while(1)
			{
				if(isAlphabet(str[i]))
				{
					type.push_back(0);
					++i;
				}
				else
				{
					if(str[i] == CLOSEPARTMP)
					{
						type.push_back(0);
						++i;
					}
					break;
				}
			}

			wstring searchStr = wstring(str.begin()+min,str.begin()+i);
			bool n = bSearchTexFunc(searchStr);
			//wcout << searchStr << endl;

			if(n == false)
			{
				for(int j=min+1;j<i;j++)
				{
					type[j] = 2;
				}
				str[min] = OPENPARTMP;
				isRm.back() = true;
			}
			else
			{
				if(str[min+1] == 'x' && str[min+2] == 'r' && str[min+3] == 'm')
				{
					isRm.back() = true;
					str[min] = str[min+1] = str[min+2] = str[min+3] = str[min+4] = ' ';
				}
				else if(str[min+1] == 'i' && str[min+2] == 't')
				{
					isRm.back() = false;
					str[min] = str[min+1] = str[min+2] = str[min+3] = ' ';
				}
			}

			--i;
		}
		else if( isAlphabet(str[i]))
		{
			if(isRm.back() == true)
				type.push_back(2);
			else
				type.push_back(1);
		}
		else
			type.push_back(0);
	}

	//wcout << str << endl;
	//for(int i=0;i<type.size();i++)
	//	wcout << type[i];
	//wcout << endl;

	vector<int> vecovlPos;

	int pos = 0;
	while(1)
	{
		pos = str.find(L"\\ovl",pos);
		if(pos == wstring::npos)
			break;
		vecovlPos.push_back(pos);
		pos += 4;
	}

	pos = 0;
	while(1)
	{
		pos = str.find(L"\\vec",pos);
		if(pos == wstring::npos)
			break;
		vecovlPos.push_back(pos);
		pos += 4;
	}

	sort(vecovlPos.begin(),vecovlPos.end());

	//for(int i=0;i<vecovlPos.size();i++)
	//{
	//	wcout << vecovlPos[i] << " ";
	//}
	//wcout << endl;
	//wcout << str << endl;
	for(int i=vecovlPos.size()-1;i>=0;i--)
	{
		int n = vecovlPos[i];
		int from,to;

		if(getParameterRange(str,n+5,&from,&to) == false)
		{
			if(smartInsert(str,to,L"}"))
				type.insert(type.begin()+to,0);
			if(smartInsert(str,from,L"{"))
				type.insert(type.begin()+from,0);
		}

		int j;
		for(j=from;j<to;j++)
		{
			if(type[j] == 1)
				break;
		}

		if(j == to)
		{
			for(j=from;j<to;j++)
				type[j] = 0;

			if(str[n+1] == 'o')
			{
				str[n+3] = 'r';
			}
			else
			{
				str[n+2] = 'r';
				str[n+3] = 'm';
			}
		}
	}

	//wcout << str << endl;
	for(int i=type.size()-1;i>=0;i--)
	{
		if(type[i] == 2)
		{
			int end = i+1;
			while(1)
			{
				if(type[i] == 2 || ( type[i] != 1 && (str[i] == ' ' || str[i] == '_' || str[i] == '^')))
					--i;
				else
					break;
			}
			++i;

			smartInsert(str,end,L"}");
			smartInsert(str,i,L"\\mrm{");
		}
	}

	return 1;
}

int setcaseBound(wstring &str)
{
	int startPos = 0;
	while(1)
	{
		
		startPos = str.find(L"cases",startPos);
		if(startPos == wstring::npos)
			break;
		int parStart = str.find(L"{",startPos);
		int len = parStart - startPos;
		int parLevel = 1;

		int i;
		int andnum=0;
		int sharpnum=0;
		for(i=parStart+1;str[i];i++)        //i는 닫는 괄호의 위치
		{
			if(str[i] == '{')
				++parLevel;
			else if(str[i] == '}')
				--parLevel;
			else if(str[i] == '#')
			{
				if(parLevel==1)
				{
					if(str[i+1] != '\n')
					{
						i+=	smartInsert(str,i+1,L"\n");
					}
					++sharpnum;
				}
			}
			else if(str[i] == '&')
			{
				if(parLevel==1 && sharpnum==0)
					++andnum;
			}
			if(parLevel==0)
				break;
		}
		if(andnum<=1)
		{
			str.insert(i,L"\n\\end{cases}");
			str.replace(startPos,len+1,L"{\\begin{cases}\n");
			startPos = str.find(L"\\end",startPos)+13;
		}
		else
		{
			wstring t = L"{\\left\\{ \\beginttabuu{";
			for(int j=0;j<andnum+1;j++)
			{
				t += 'c';
			}
			t += '}';
			
			str.insert(i,L"\\end{tabu} \\right.\n");
			str.replace(startPos,len+1,t.data());
			startPos = str.find(L"\\end",startPos+t.size())+18;
		}
	}

	return 1;
}

int setmatrixBound(wstring &str)
{
	int startPos = 0;
	while(1)
	{
		startPos = str.find(L"matrix",startPos);
		if(startPos == wstring::npos)
			break;
		int parStart = str.find(L"{",startPos);
		int len = parStart - startPos;
		int parLevel = 1;
		wstring type = L"";
		if(str[startPos-1] == 'b' || str[startPos-1] == 'd' || str[startPos-1] == 'p')
			type += str[startPos-1];
		type += L"matrix";

		int i;
		for(i=parStart+1;str[i];i++)        //i는 닫는 괄호의 위치
		{
			switch(str[i])
			{
			case '{':
				++parLevel;
				break;
			case '}':
				--parLevel;
				break;
			}
			if(parLevel==0)
				break;
		}
		str.insert(i,L"\\end{" + type + L"}");
		if(type[0]!='m') 
			str.replace(startPos-1,len+2,L"{\\begin{" + type + L"}");
		else
			str.replace(startPos,len+1,L"{\\begin{" + type + L"}");
		startPos = str.find(L"\\end",startPos)+13;
	}

	return 1;
}

int setLRPar(wstring &str)
{
	//wcout << str << endl;
	int n=0;
	while(1)
	{
		n = str.find(L"\\left",n+1);
		if(n==wstring::npos)
			break;

		n+=5;
		findNotSpace(str,n);

		if(str[n] == '{' || str[n] == '}')
		{
			smartInsert(str,n,L"\\");
		}
		else
		{
			if(str[n] == '(' || str[n] == '|' || str[n] == '[' || str[n] == '.' || str[n] == '<' || str[n] == ')' || str[n] == ']' || str[n] == '>' || str[n] == '\\')
				;
			else
				smartInsert(str,n,L".");
		}
	}

	n = 0;
	while(1)
	{
		n = str.find(L"\\right",n+1);
		if(n==wstring::npos)
			break;

		n+=6;
		findNotSpace(str,n);

		if(str[n] == '}' || str[n] == '{')
		{
			smartInsert(str,n,L"\\");
		}
		else
		{
			if(str[n] == ')' || str[n] == '|' || str[n] == ']' || str[n] == '.' || str[n] == '>' || str[n] == '(' || str[n] == '[' || str[n] == '<' || str[n] == '\\')
				;
			else
				smartInsert(str,n,L".");
		}
	}
	//wcout << str << endl;

	int parLevel = 0;
	for(int i=1;str[i];i++)
	{
		switch(str[i])
		{
		case '{':
			if(str[i-1] != '\\')
				++parLevel;
			break;
		case '}':
			if(str[i-1] != '\\')
				--parLevel;
			break;
		}
	}

	if(parLevel>0)
	{
		for(int i=0;i<parLevel;i++)
		{
			smartInsert(str,str.size()-1,L"}");
		}
	}
	else if(parLevel<0)
	{
		for(int i=0;i<-parLevel;i++)
		{
			smartInsert(str,1,L"{");
		}
	}

	int insertedNum = 0;
	vector<pair<int,int>> LRlev;
	LRlev.push_back(pair<int,int>(0,1));

	
	int i=0;
	for(i=1;str[i]!='$';i++)
	{
		if(str[i] == '{' && str[i-1] != '\\')
			LRlev.push_back(pair<int,int>(0,i+1));
		else if(str[i] == '}' && str[i-1] != '\\')
		{
			if(LRlev.back().first < 0)	//left가 부족함
			{
				for(int j=LRlev.back().first;j<0;j++)
				{
					i += smartInsert(str,LRlev.back().second,L"\\left. ");
					++insertedNum;
				}
			}
			else if(LRlev.back().first > 0)	//right가 부족함
			{
				for(int j=LRlev.back().first;j>0;j--)
				{
					i += smartInsert(str,i,L"\\right. ");
					++insertedNum;
				}
			}

			LRlev.pop_back();
		}
		else if(str[i] == '\\')
		{
			if(i < str.size()-5)
			{
				if(str[i+1] == 'l' && str[i+2] == 'e' && str[i+3] == 'f' && str[i+4] == 't')
				{
					++LRlev.back().first;
					i += 5;
				}
				else if(str[i+1] == 'r' && str[i+2] == 'i' && str[i+3] == 'g' && str[i+4] == 'h' && str[i+5] == 't')
				{
					--LRlev.back().first;
					i += 6;
				}
			}
		}
	}
	//wcout << str << endl;
	
	if(LRlev[0].first > 0)
	{
		++insertedNum;
		for(int j=LRlev.back().first;j>0;j--)
			smartInsert(str,i,L"\\right. ");
	}
	else if(LRlev[0].first < 0)
	{
		++insertedNum;
		for(int j=LRlev.back().first;j<0;j++)
			smartInsert(str,1,L"\\left. ");
	}

	//추가된 LEFT나 RIGHT가 있으면 오류 가능성이 있으므로 true를 리턴
	return (insertedNum!=0);
}

int checkSmallPar(wstring &str)        //\left, \right가 필요 없으면 제거
{
	int start = -1;
	int end = -1;
	int prevstart = -1;

	while(1)
	{
		end = str.find(L"\\right",end+1);
		if(end == wstring::npos)
			break;
		start = str.rfind(L"\\left",end-1);
		if(start == prevstart)
			continue;
		prevstart = start;

		int i;
		for(i=start+5;i<end;i++)    //괄호를 길어지게 하는 문자 검색
		{
			wchar_t p = str[i];
			if(p=='\\')                //\ 문자인 경우 dfrac 또는 \begin을 검사
			{
				if(str.find(L"\\dfrac",i) == i || str.find(L"\\begin",i) == i)
					break;
			}
			else
			{
				if(p=='^' || p=='_')//위 첨자나 밑 첨자인 경우
					break;
			}
		}
		if(i==end)    //괄호를 길어지게 하는 문자열이 없는 경우, left, right를 없앰
		{
			int parchar = end+6;
			findNotSpace(str,parchar);

			if(str[parchar] == '.')					//right가 .이면 .까지 같이 삭제
			{
				str.erase(end,parchar-end+1);
				end -= parchar-end+1;
			}
			else
			{
				str.erase(end,6);
				end -= 6;
			}

			parchar = start+5;
			findNotSpace(str,parchar);

			if(str[parchar] == '.')					//left가 .이면 .까지 같이 삭제
				str.erase(start,parchar-start+1);
			else
				str.erase(start,5);
		}
		else        //괄호를 길어지게 하는 문자열이 있는 경우, 그대로 놔움
		{
			end += 6;
		}
	}
	return 1;
}

int autoalign(wstring &str)
{
	vector<int> startCasePos;
	vector<int> endCasePos;
	vector<int> startMatrixPos;
	vector<int> endMatrixPos;
	vector<int> startAlignedPos;
	vector<int> endAlignedPos;
	int scp=0,smp=0,sap=0;
	int andPos = -1;
	int newlnPos = -1;
	int cNum;
	int mNum;
	int aNum;

	while(1)
	{
		scp = str.find(L"\\begin{cases}",scp)+13;
		if(scp-13 != wstring::npos)
		{
			startCasePos.push_back(scp);                        //\begin{cases}의 위치
			scp = str.find(L"\\end{cases}",scp);                    //\end{cases}의 위치
			endCasePos.push_back(scp);
			scp += 11;
		}
		else
			break;
	}

	while(1)
	{
		smp = str.find(L"matrix",smp) + 7;
		if(smp-7 != wstring::npos)
		{
			startMatrixPos.push_back(smp);                        //\begin{?matrix}의 위치
			smp = str.find(L"matrix",smp)-6;                        //\end{?matrix}의 위치
			endMatrixPos.push_back(smp);
			smp += 7;
		}
		else
			break;
	}

	while(1)
	{
		sap = str.find(L"\\begin{aligned}",sap) + 15;
		if(sap-15 != wstring::npos)
		{
			startAlignedPos.push_back(sap);                        //\begin{aligned}의 위치
			sap = str.find(L"\\end{aligned}",sap);               //\end{aligned}의 위치
			endAlignedPos.push_back(sap);
			sap += 13;
		}
		else
			break;
	}

	cNum = startCasePos.size();
	mNum = startMatrixPos.size();
	aNum = startAlignedPos.size();

	while(1)
	{
		andPos = str.find(L"&",andPos+1);
		newlnPos = str.find(L"#",newlnPos+1);
		if(andPos == wstring::npos && newlnPos == wstring::npos)
		{
			break;
		}

		int i=0;
		for(i=0;i<cNum;i++)
		{
			if(andPos >= startCasePos[i] && andPos <= endCasePos[i]
			|| newlnPos >= startCasePos[i] && newlnPos <= endCasePos[i])        //\begin{cases}와 \end{cases} 사이의 &인 경우
			{
				break;
			}
		}

		int j=0;
		for(j=0;j<mNum;j++)
		{
			if(andPos >= startMatrixPos[j] && andPos <= endMatrixPos[j]
			|| newlnPos >= startMatrixPos[j] && newlnPos <= endMatrixPos[j])    //\begin{?matrix}와 \end{?matrix} 사이의 &인 경우
			{
				break;
			}
		}

		int k=0;
		for(k=0;k<aNum;k++)
		{
			if(andPos >= startAlignedPos[j] && andPos <= endAlignedPos[j]
			|| newlnPos >= startAlignedPos[j] && newlnPos <= endAlignedPos[j])    //\begin{aligned}와 \end{aligned} 사이의 &인 경우
			{
				break;
			}
		}

		if(i==cNum && j==mNum && k==aNum)        //&가 case 또는 matrix의 &가 아닌 경우 (정렬에 사용되는 &일 경우)
		{
			if(andPos >=0 && andPos<newlnPos)
				str.insert(andPos,L"\n");
			str.replace(str.find(L"$"),1,L"\\begin{align*}\n");
			str.replace(str.rfind(L"$"),1,L"\n\\end{align*}");
			break;
		}
	}
	return 1;
}

void changepile(wstring &str)
{
	int start = str.find(L"\\pile");
	if(start == wstring::npos)
		return;
	int i=0;
	while(1)
	{
		int mid = str.find(L"{",start+5);
		int end = mid+1;
		int parLevel=0;
		for(;str[end];end++)
		{
			switch(str[end])
			{
			case '{':
				++parLevel;
				break;
			case '}':
				--parLevel;
				break;
			}
			if(parLevel == -1)
				break;
		}
		str.replace(end,1,L"\\end{aligned}");
		str.replace(start,mid-start+1,L"\\begin{aligned}");
		start = str.find(L"\\pile",start+15);
		if(start == wstring::npos)
			break;
	}
}

void deleteOverusedPar(wstring &str)
{
	int parlev = 0;
	vector<int> pos;
	vector<int> parNum;

	for(int i=0;i<str.size();i++)
	{
		if(str[i] == '{' && str[i-1] != '\\')
		{
			pos.push_back(i);
			if(parlev >= parNum.size() || parNum[parlev] == 0)
				parNum.push_back(0);
			++parlev;
		}
		else if(str[i] == '}' && str[i-1] != '\\')
		{
			bool deletePar = true;

			int start = pos[--parlev];
			int end = i;
			pos.pop_back();

			if(parNum[parlev] == 0)
			{
				parNum.pop_back();
				
				int j = start-1;
				for(;str[j] == ' ';j--){}
				if(str[j] == ']')
				{
					int bigparlev = 1;
					int midparlev = 0;
					--j;
					for(;j>=0;j--)
					{
						switch(str[j])
						{
						case '[':
							--bigparlev;
							break;

						case ']':
							++bigparlev;
							break;

						case '{':
							if(str[j-1] != '\\')
								--midparlev;
							break;

						case '}':
							if(str[j-1] != '\\')
								++midparlev;
							break;
						}
						if(bigparlev==0 || midparlev<0)
							break;
					}

					if(j==-1)		//대응하는 여는 대괄호가 없을 때
					{
						deletePar = false;
					}
					else
					{
						--j;
						for(;str[j] == ' ';j--){}
					}
				}
				if(deletePar == true && isAlphabet(str[j]))	//여는 괄호 이전의 문자가 알파벳일 때
				{
					int funcEnd = j;
					--j;
					for(;isAlphabet(str[j]);j--){}

					if(str[j] == '\\')		//tex 명령어일 때
					{
						for(int k=0;k<oneparsize;k++)
						{
							if(str.find(oneParameter[k],j) == j)
							{
								deletePar = false;
								break;
							}
						}

						for(int k=0;k<twoparsize;k++)
						{
							if(str.find(twoParameter[k],j) == j)
							{
								parNum.push_back(1);
								deletePar = false;
								break;
							}
						}
					}
				}
				else if(deletePar == true && (str[j] == '_' || str[j] == '^'))		//여는 괄호 이전의 문자가 첨자나 지수윌 때
				{
					int k = start+1;
					for(;k<end && str[k]==' ';k++){}
					if(k!=end)
						deletePar = false;
					else
						str[j] = ' ';
				}
			}
			else
			{
				if(--parNum[parlev] == 0)
					parNum.pop_back();

				deletePar = false;
			}

			if(deletePar == true)
			{
				str[start] = str[end] = ' ';
			}
		}
	}
}

void checkDoubleSub(wstring &str)
{
	for(int i=0;i<sizeof(doubleSubSupList)/sizeof(wstring);i++)
	{
		int startPos = 0;
		while(1)
		{
			startPos = str.find(doubleSubSupList[i],startPos);
			if(startPos == wstring::npos)
				break;

			bool findSub = false;
			bool findSup = false;
			bool insertPar = false;
			int n = startPos + doubleSubSupList[i].size();
			findNotSpace(str,n);

			if(str[n] == '_')
			{
				findSub = true;
				++n;
			}
			else if(str[n] == '^')
			{
				findSup = true;
				++n;
			}
			else
			{
				startPos = n;
				continue;
			}

			findNotSpace(str,n);		//첨자 또는 지수 다음으로, 최초의 공백 아닌 문자 = 여는 괄호
			n = findParMatch(str,n);	//닫는 괄호 짝 찾기
			findNotSpace(str,n);		//점자 또는 지수 검색

			if(str[n] == '_')
			{
				if(findSub == false)
				{
					findSub = true;
					++n;
				}
				else
					insertPar = true;
			}
			else if(str[n] == '^')
			{
				if(findSup == false)
				{
					findSup = true;
					++n;
				}
				else
					insertPar = true;
			}
			else
			{
				startPos = n;
				continue;
			}

			//현재 n은 두번째 지수 또는 첨자의 바로 다음 문자를 가리키는 중
			if(insertPar == false)
			{
				findNotSpace(str,n);		//첨자 또는 지수 다음으로, 최초의 공백 아닌 문자 = 여는 괄호
				n = findParMatch(str,n);	//닫는 괄호 짝 찾기
				findNotSpace(str,n);		//점자 또는 지수 검색

				if(str[n] == '_' || str[n] == '^')
				{
					insertPar = true;
				}
			}

			if(insertPar == true)
			{
				smartInsert(str,n,L"{}");
			}
			startPos = n;
		}
	}
}

void deleteMultipleSpace(wstring &str)
{
	for(int i=str.size()-1;i>0;i--)
	{
		if(str[i] == ' ' && str[i-1] == ' ')
		{
			int j = i-1;
			for(;str[j] == ' ';j--){}
			str.erase(j+2,i-j-1);
			i = j;
		}
	}
}

void hwpEqToTex(wstring &str)    //한글 수식을 텍 수식으로 변환
{
	bool errorProbability = false;
	
	//wcout << str << endl;
	for(int i=str.size()-1;i>1;i--)
	{
		if(isAlphabet(str[i-1]) == false && isAlphabet(str[i]) == true)
		{
			if(str[i-1] != ' ' && bSearchOutParameter(str,i,hwpParameterOutKeyword) != wstring::npos)
			{
				str.insert(i,L" ");
			}
		}
	}

	//wcout << str << endl;
	wrapSubSup(str);

	for(int i=0;str[i+1];i++)        //소문자 다음에 대문자가 나오면 공백 추가
	{
		if(str[i] >= 'a' && str[i] <= 'z' && str[i+1] >= 'A' && str[i+1] <= 'Z')
			str.insert(i+1,L" ");
	}
	
	
	int startPos;
	for(int i=0;i<spcsize;i++)    //특수문자 선 처리
	{
		startPos = 0;
		int hwpstrlen = spchwp[i].size();
		while(1)
		{
			int n = str.find(spchwp[i],startPos);
			if(n != wstring::npos)    //부분 문자열 발견
			{
				str.replace(n,hwpstrlen,spcTex[i]);
				startPos = n+spcTex[i].size();
			}
			else
			{
				break;
			}
		}
	}

	for(int i=0;str[i];i++)        //알파벳 검색
	{
		if(!isAlphabet(str[i]))    //i는 최초의 알파벳을 가리킴
			continue;

		int n = bSearchAlphabet(str,i,funcSorted,funcsize);
		
		if(n != wstring::npos)
		{
			int hwpsz = funcSorted[n].hwp->size();
			int texsz = funcSorted[n].Tex->size();
			str.replace(i,hwpsz,*(funcSorted[n].Tex));
			i += texsz-1;
		}
	}
	//wcout << str << endl;

	setrmBound(str,0,str.size());
	//wcout << str << endl;
	setcaseBound(str);
	//wcout << str << endl;
	setmatrixBound(str);
	//wcout << str << endl;
	errorProbability = setLRPar(str);
	//wcout << str << endl;
	addSpaceAfterLRPar(str);
	wrapAnother(str);
	wrapSqrt(str);
	overToFrac(str);
	for(int i=0;str[i];i++)
	{
		if(str[i] == CLOSEPARTMP || str[i] == OPENPARTMP || str[i] == '`')
			str[i] = ' ';
	}
	
	checkSmallPar(str);
	deleqalign(str);
	changepile(str);
	
	autoalign(str);
	deleteOverusedPar(str);
	checkDoubleSub(str);
	deleteMultipleSpace(str);


	for(int i=0;i<spcnextsize;i++)    //특수문자 후처리
	{
		startPos = 0;
		int hwpstrlen = spchwpNext[i].size();

		while(1)
		{
			int n = str.find(spchwpNext[i],startPos);
			if(n != wstring::npos)    //부분 문자열 발견
			{
				str.replace(n,hwpstrlen,spcTexNext[i]);
				startPos = n+spcTexNext[i].size();
			}
			else
			{
				break;
			}
		}
	}

	for(int i=0;i<spcunicodesize ;i++)    //특수문자 후처리
	{
		for(int j=0;str[j];j++)
		{
			int k=0;
			for(;hwpUnicodeSpc[i][k];k++)
			{
				if(str[j+k] != hwpUnicodeSpc[i][k])
					break;
			}
			if(hwpUnicodeSpc[i][k]==NULL)		//검색 성공
			{
				str.replace(j,hwpUnicodeSpcLen[i],texUnicodeSpc[i]);
				j += texUnicodeSpcLen[i] - 1;
			}
			if(str.size()-j <= hwpUnicodeSpcLen[i])
				break;
		}
	}

	for(int i=0;i<remainsize;i++)
	{
		startPos = 0;
		int hwpstrlen = funcRemain[i].size();
		while(1)
		{
			int n = str.find(funcRemain[i],startPos);
			if(n != wstring::npos)    //부분 문자열 발견
			{
				str.replace(n,hwpstrlen,funcRemainNext[i]);
				startPos = n;
			}
			else
			{
				break;
			}
		}
	}

	//LEFT, RIGHT 짝이 맞지 않는 경우 ****을 붙여서 알려줌
	if(errorProbability)
		str += L"****";
}
