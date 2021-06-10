#include <LPC21xx.H>
#define MAX_TOKEN_NR 3              //maksymalna dopuszczalna ilosc tokenów
#define MAX_KEYWORD_STRING_LTH 10   //maksymalna dlugosc komendy
#define MAX_KEYWORD_NR 3

typedef enum TokenType {KEYWORD, NUMBER, STRING};
typedef enum KeywordCode {LD, ST, RST};
struct Token asToken[MAX_TOKEN_NR];
typedef enum TokenType { KEYWORD, NUMBER, STRING};	




typedef union TokenValue
{
enum KeywordCode 	eKeyword;     // jezeli KEYWORD
unsigned int 			uiNumber;     // jezeli NUMBER
char * 						pcString;     // jezeli STRING
}TokenValue;


typedef struct Token
{
enum TokenType eType;           // KEYWORD, NUMBER, STRING
union TokenValue uValue;        // enum, unsigned int, char*
}Token;


typedef struct Keyword
{
enum KeywordCode eCode;
char cString[MAX_KEYWORD_STRING_LTH + 1];
}Keyword;


struct Keyword asKeywordList[MAX_KEYWORD_NR]=
{
{RST,"reset"},
{LD, "load" },
{ST, "store"}
};





enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue)
{
	unsigned char ucCharCounter;
	if((pcStr[0] != '0') | (pcStr[1] != 'x') | (pcStr[2] == '\0'))
		return ERROR;
	*puiValue = 0;
	for(ucCharCounter=2;ucCharCounter!='\0';ucCharCounter++)
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

enum CompResult eCompareString(char pcStr1[], char pcStr2[])
{
	unsigned char ucCharCounter;
	for(ucCharCounter=0;pcStr1[ucCharCounter] != '\0';ucCharCounter++)
	{
		if (pcStr1[ucCharCounter] != pcStr2[ucCharCounter]) return DIFFERENT;
	}
	return EQUAL;
}

void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar)
{
	unsigned char ucCharCounter;
	for(ucCharCounter=0;pcString[ucCharCounter]!='\0';ucCharCounter++)
	{
		if(pcString[ucCharCounter] == cOldChar) pcString[ucCharCounter] = cNewChar;
	}
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

void DecodeMsg(char *pcString)
{
	ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString,' ','\0');
	DecodeTokens();
}




/*------------------------------------------------------------------------------------*/
int main(){
	char* pcString="load 0x20 immediately";
	DecodeMsg(pcString);
}