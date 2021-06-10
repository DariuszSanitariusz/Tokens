#include <LPC21xx.H>
#define HEX_bm 0x000F
#define MAX_TOKEN_NR 3
#define MAX_KEYWORD_STRING_LTH 10
#define MAX_KEYWORD_NR 3

typedef enum CompResult 
{ DIFFERENT , EQUAL } CompResult;

typedef enum Result 
{ OK, ERROR } Result;

typedef enum TokenType 
{KEYWORD, NUMBER, STRING} TokenType;

typedef enum KeywordCode 
{ LD, ST, RST} KeywordCode;

typedef union TokenValue
{
enum KeywordCode eKeyword;
unsigned int uiNumber;
char *pcString; 
} TokenValue;

typedef struct Token
{
enum TokenType eType; 
union TokenValue uValue; 
} Token;

struct Token asToken[MAX_TOKEN_NR];

typedef struct Keyword
{
enum KeywordCode eCode;
char cString[MAX_KEYWORD_STRING_LTH + 1];
} Keyword;

struct Keyword asKeywordList[MAX_KEYWORD_NR]= 
{
{RST,"reset"},
{LD, "load" },
{ST, "store"}
};

void CopyString(char pcSource[], char pcDestination[])
{
	unsigned char ucCharCounter;
	for(ucCharCounter=0;pcSource[ucCharCounter]!='\0';ucCharCounter++)
	{
		pcDestination[ucCharCounter] = pcSource[ucCharCounter];
	}
	pcDestination[ucCharCounter] = '\0';
}

enum Result TestOf_CopyString()
{
	unsigned char ucCharCounter;
	char pcTestSource[] = "Test 1";
	char pcTestDest[7];
	//Test 1 sprawdza czy funkcja dobrze kopiuje stringi
	CopyString(pcTestSource,pcTestDest);
	for(ucCharCounter=0;ucCharCounter<8;ucCharCounter++)
	{
		if (pcTestDest[ucCharCounter] != pcTestSource[ucCharCounter]) 
			return ERROR;
	}
	return OK;
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[])
{
	unsigned char ucCharCounter;
	for(ucCharCounter=0;pcStr1[ucCharCounter] != '\0';ucCharCounter++)
	{
		if (pcStr1[ucCharCounter] != pcStr2[ucCharCounter]) return DIFFERENT;
	}
	return EQUAL;
}

enum Result TestOf_eCompareString()
{
	char pcTest1[] = "Test 1";
	char pcTest2[] = "Tescik 2";
	//Test 2 sprawdza czy funkcja dobrze porownuje stringi
	if (eCompareString(pcTest1,pcTest1) == DIFFERENT) 
		return ERROR;
	if (eCompareString(pcTest1,pcTest2) == EQUAL) 
		return ERROR;
	return OK;
}
void AppendString (char pcSourceStr[],char pcDestinationStr[])
{
	unsigned char ucEndPointer;
	for(ucEndPointer=0;pcDestinationStr[ucEndPointer]!='\0';ucEndPointer++) {}
	CopyString(pcSourceStr,pcDestinationStr+ucEndPointer);
}

enum Result TestOf_AppendString()
{
	char pcTestDest[] = "Test";
	char pcTestSource[] = " 3";
	char pcTestWynik[] = "Test 3";
	//Test 3 sprawdza czy funkcja dobrze dopisuje stringi
	AppendString(pcTestSource,pcTestDest);
	if (eCompareString(pcTestDest,pcTestWynik) == DIFFERENT) 
		return ERROR;
	return OK;
}

void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar)
{
	unsigned char ucCharCounter;
	for(ucCharCounter=0;pcString[ucCharCounter]!='\0';ucCharCounter++)
	{
		if(pcString[ucCharCounter] == cOldChar) pcString[ucCharCounter] = cNewChar;
	}
}

enum Result TestOf_ReplaceCharactersInString()
{
	char pcTestString[] = "Testowy String";
	char pcTestWynik[] = "Tesxowy String";
	//Test 4 sprawdza czy funkcja dobrze zamienia znaki
	ReplaceCharactersInString(pcTestString,'t','x');
	if (eCompareString(pcTestString,pcTestWynik) == DIFFERENT) 
		return ERROR;
	return OK;
}

void UIntToHexStr(unsigned int uiValue, char pcStr[])
{
	unsigned char ucNibbleCounter;
	unsigned char ucCurrentNibble;
	pcStr[0] = '0';
	pcStr[1] = 'x';
	for(ucNibbleCounter=0;ucNibbleCounter<4;ucNibbleCounter++)
	{
		ucCurrentNibble = ((uiValue >> ucNibbleCounter*4) & HEX_bm);
		if(ucCurrentNibble>9) 
			pcStr[5-ucNibbleCounter] = ucCurrentNibble - 10 + 'A';
		else 
			pcStr[5-ucNibbleCounter] = ucCurrentNibble + '0';
	}
	pcStr[6] = '\0';
}

enum Result eTestOf_UIntToHexStr()
{
	char pcString[7];
	char pcTestString[7] = "0xAF19";
	//Test 5 sprawdza czy funkcja dobrze zamienia na hex
	UIntToHexStr(0xAF19,pcString);
	if (eCompareString(pcString,pcTestString) == DIFFERENT) 
		return ERROR;
	return OK;
}

enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue)
{
	unsigned char ucCharCounter;
	if((pcStr[0] != '0') | (pcStr[1] != 'x') | (pcStr[2] == '\0'))
		return ERROR;
	*puiValue = 0;
	for(ucCharCounter=2;ucCharCounter<7;ucCharCounter++)
	{
		if(pcStr[ucCharCounter] == '\0') 
			return OK;
	  *puiValue = *puiValue << 4;
		if(pcStr[ucCharCounter] >= 'A') 
			*puiValue = *puiValue | (pcStr[ucCharCounter] - 'A' + 10); 
		else 
			*puiValue = *puiValue | (pcStr[ucCharCounter] - '0');
	}
	return OK;
}
enum Result eTestOf_eHexStringToUInt()
{
	char liczba[] = "0xAB12";
	unsigned int uiResult;
	//Test 6 sprawdza czy funkcja dobrze zamienia hex na uint
	eHexStringToUInt(liczba,&uiResult);
	if (uiResult != 0xAB12) 
		return ERROR;
	return OK;
}

void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[])
{
	unsigned char ucEndPointer;
	for(ucEndPointer=0;pcDestinationStr[ucEndPointer]!='\0';ucEndPointer++) {}
	UIntToHexStr(uiValue,pcDestinationStr+ucEndPointer);
}

enum Result eTestOf_AppendUIntToString()
{
	unsigned int uiNumber = 0xAB12;
	char pcString[14] = "Test 7 ";
	char pcTestString[13] = "Test 7 0xAB12";
	//Test 7 sprawdza czy funkcja dobrze dopisuje string (z hexa) na koniec stringa
	AppendUIntToString(uiNumber,pcString);
	if (eCompareString(pcString,pcTestString) == DIFFERENT) 
		return ERROR;
	return OK;
}

unsigned char ucFindTokensInString(char *pcString)
{
	unsigned char ucTokenPointer;
	unsigned char ucDelimiterCounter;
	char cCurrentChar;
	enum State {TOKEN, DELIMITER};
	enum State eState = DELIMITER;
	ucDelimiterCounter = 0;
	
	for(ucTokenPointer=0;;ucTokenPointer++)
	{
		cCurrentChar = pcString[ucTokenPointer];
		switch(eState)
		{
			case DELIMITER:
				if(cCurrentChar == '\0') 
					return ucDelimiterCounter;
				else if(cCurrentChar == ' ') {}
				else 
				{
					eState = TOKEN;
					asToken[ucDelimiterCounter].uValue.pcString = pcString+ucTokenPointer;
					ucDelimiterCounter++;
				}
				break;
			case TOKEN:
				if(cCurrentChar == '\0') 
					return ucDelimiterCounter;
				else if(ucDelimiterCounter == MAX_TOKEN_NR) 
					return ucDelimiterCounter;
				else if(cCurrentChar != ' ') {}
				else 
					eState = DELIMITER;
				break;
		}
	}
}

enum Result eTestOf_ucFindTokensInString()
{
	char test1[] = "            ";
	char test2[] = "Token1 Token2";
	char test3[] = "Token1     Token2";
	unsigned char wynik;
	
	wynik = ucFindTokensInString(test1);
	if (wynik != 0) 
		return ERROR;
	if (asToken[0].uValue.pcString != '\0') 
		return ERROR;
	wynik = ucFindTokensInString(test2);
	if (wynik != 2) 
		return ERROR;
	wynik = ucFindTokensInString(test3);
	if (wynik != 2) 
		return ERROR;
	return OK;
}

enum Result eStringToKeyword (char pcStr[],enum KeywordCode *peKeywordCode)
{
	unsigned char ucTokenCounter;
	for(ucTokenCounter=0;ucTokenCounter<MAX_TOKEN_NR;ucTokenCounter++)
	{
		if (eCompareString(pcStr,asKeywordList[ucTokenCounter].cString) == EQUAL) 
		{
			*peKeywordCode = asKeywordList[ucTokenCounter].eCode;
			return OK;
		}
	}
	return ERROR;
}

enum Result eTestOf_eStringToKeyword()
{
	char cTest[] = "load ";
	enum KeywordCode eKeywordCode;
	ReplaceCharactersInString(cTest,' ','\0');
	eStringToKeyword(cTest,&eKeywordCode);
	if (eKeywordCode != LD) 
		return ERROR;
	else 
		return OK;
}

void DecodeTokens()
{
	unsigned char ucTokenCounter;
	Token* tValue;
	for(ucTokenCounter=0;ucTokenCounter<MAX_TOKEN_NR;ucTokenCounter++)
	{
		tValue = &asToken[ucTokenCounter];
		if (eStringToKeyword(tValue->uValue.pcString,&tValue->uValue.eKeyword) == OK) tValue->eType = KEYWORD;
		else if (eHexStringToUInt(tValue->uValue.pcString,&tValue->uValue.uiNumber) == OK) tValue->eType = NUMBER;
		else tValue->eType = STRING;
	}
}

enum Result eTestOf_DecodeTokens()
{
	char cTest[] = "load   0x1CD2 ala ";
	Token* tValue;
	ucFindTokensInString(cTest);
	ReplaceCharactersInString(cTest,' ','\0');
	DecodeTokens();
	
	tValue = &asToken[0];
	if (tValue->eType != KEYWORD) 
		return ERROR;
	else if (tValue->uValue.eKeyword != LD) 
		return ERROR;
	tValue = &asToken[1];
	if (tValue->eType != NUMBER) 
		return ERROR;
	else if (tValue->uValue.uiNumber != 0x1CD2) 
		return ERROR;
	tValue = &asToken[2];
	if (tValue->eType != STRING) 
		return ERROR;
	else if (eCompareString("ala",tValue->uValue.pcString) == DIFFERENT) 
		return ERROR;
	return OK;
}

void DecodeMsg(char *pcString)
{
	ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString,' ','\0');
	DecodeTokens();
}


int main()
{
	///
}