// scanner.c
#include "scanner.h"

// �ʷ�����������
typedef struct {
	// ��¼��ǰ���ڴ����Token�ĵ�һ���ַ�, ����������Tokenʱ, ��token.start��ֵ
	// start��Token����Ĺ�����, �ǲ����ƶ���
	const char* start;

	// ��ǰ�ʷ����������ڴ���������ַ�
	// curr���Token��startλ�ÿ�ʼ, ֱ������Token��������һ���ַ�
	const char* current;
	int line;	// ָʾ��ǰ���ڴ���Ĵ�����
} Scanner;

// ȫ�ֱ���
Scanner scanner;

void initScanner(const char* source) {
	// ��ʼ��scanner
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

/***************************************************************************************
 *                                   ��������											*
 ***************************************************************************************/
 // �жϵ�ǰ�ַ��ǲ�����ĸ�����»���
static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

// �жϵ�ǰ�ַ��ǲ�������
static bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

// �жϵ�ǰ���ڴ�����ַ��ǲ��ǿ��ַ�, ����Ǿͱ�ʾ���������
static bool isAtEnd() {
	return *scanner.current == '\0';
}

// ǰ��, ���ص�ǰ���ڴ�����ַ�, ������curr�ƶ�����һ���ַ�
static char advance() {
	return *scanner.current++;
}

// ���ص�ǰ���ڴ�����ַ�, ���ǲ��ƶ�curr
static char peek() {
	return *scanner.current;
}

// �����ǰ���ڴ�����ַ����ǿ��ַ�, �ͷ�����һ����������ַ�
static char peekNext() {
	if (isAtEnd()) {
		return '\0';
	}
	return *(scanner.current + 1);
}
// �жϵ�ǰ���ڴ�����ַ��ǲ���expected,�������Ԥ��,��ô���ƶ�currָ��
static bool match(char expected) {
	if (isAtEnd()) {
		return false;
	}
	if (peek() != expected) {
		return false;
	}
	scanner.current++;
	return true;
}

// ����TokenType, ������Ӧ���͵�Token�������ء�
static Token makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.length = (int)(scanner.current - scanner.start);
	token.line = scanner.line;
	return token;
}

// �������ܽ��������ʱ�����Ǵ���һ��ERROR Token. ���磺����@��$�ȷ���ʱ�������ַ������ַ�û�ж�Ӧ��������ʱ��
static Token errorToken(const char* message) {
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = scanner.line;
	return token;
}

static void skipWhitespace() {
	// �����հ��ַ�: ' ', '\r', '\t', '\n'��ע��
	// ע����'//'��ͷ, һֱ����β
	// ע�����scanner.line��

	/*
		��ʾ:
		1.���ȿ϶�Ҫ��ѭ��, ��Ϊ��֪���ж��ٸ��ַ�
		2.��switch, �ѵ��ַ�ֱ������: ����س���, �ո�, �Ʊ�
			���з�: ����line��Ա
			ע��: ����/, ��Ƴһ����һ���ַ�, �������/ ����ע�� ����Ҫ��whileѭ��
	*/
}
static TokenType check_keyword(const char* rest, int start, int len, TokenType type) {
	/*
		rest, ������ʣ������, ������Ĺؼ�����break, b�Ѿ���switch�м����
			rest����reak
		start, ����reak�����������е��±�(��scanner.start��ʼ��һ���±�)
		len, ������ʣ�����еĳ���,������Ĺؼ�����break,len����4
	*/
	// У��Token�ĳ����Ƿ����
	if ((scanner.current - scanner.start) != (start + len)) {
		return TOKEN_IDENTIFIER;
	}
	/*
	int memcmp(const void *s1, const void *s2, size_t n);
	����Ĳ����ֱ��ǣ�
		s1��ָ���һ���ڴ������ָ�롣
		s2��ָ��ڶ����ڴ������ָ�롣
		n��Ҫ�Ƚϵ��ֽ�����
	memcmp ���������ֽڱȽ� s1 �� s2 ָ����ڴ�����ֱ���в���ȵ��ֽڻ�Ƚ��� n ���ֽ�Ϊֹ��
	��������ڴ�������ȫ��ͬ��
	�� memcmp ���� 0�������һ����ͬ���ֽ��� s1 �е�ֵС�� s2 �ж�Ӧ��ֵ�����ظ�����
	��֮������������
	*/
	if (!memcmp(scanner.start + start, rest, len)) {
		return type;
	}
	return TOKEN_IDENTIFIER;
}
static TokenType identifierType() {
	// �����������ʱ, sc.startָ��Token�Ŀ�ͷ�ַ�, currָ����Ǹ�Token�����һ���ַ�
	// �ú����������ж�start��curr֮���Token�����Ǳ�ʶ�����ǹؼ���
	char c = scanner.start[0];
	switch (c)
	{
	case 'b':
		return check_keyword("reak", 1, 4, TOKEN_BREAK);
		break;
	default:
		break;
	}
}
// ����Token�Ǳ�ʶ���Լ��ؼ��ֵ��߼�
static Token identifier() {
	// IDENTIFIER����: ��ĸ�����ֺ��»���
	// �жϵ�ǰ���ڴ�����ַ� �ǲ�����ĸ �»��� ����
	// �����, currָ���ǰ��
	while (isAlpha(peek()) || isDigit(peek())) {
		advance();
	}
	// ������Token�����Ǳ�ʶ��, Ҳ�����ǹؼ���, identifierType()������ȷ��Token���͵�
	return makeToken(identifierType());
}

static Token number() {
	// ����������ǽ�NUMBER�Ĺ���������:
	// 1. NUMBER���԰������ֺ����һ��'.'��
	// 2. '.'��ǰ��Ҫ������
	// 3. '.'�ź���ҲҪ������
	// ��Щ���ǺϷ���NUMBER: 123, 3.14
	// ��Щ���ǲ��Ϸ���NUMBER: 123., .14
	// ��ʾ: ����������ѭ��һֱ����ȫ����.�Լ�����
}

static Token string() {
	// �ַ�����"��ͷ����"��β�����Ҳ��ܿ���
	// �ƶ�currָ��, ֻҪ������", �Ǿ�һֱ�ƶ�
}

static Token character() {
	// �ַ�'��ͷ����'��β�����Ҳ��ܿ���   
}

/***************************************************************************************
 *                                   	�ִ�											  *
 ***************************************************************************************/
Token scanToken() {	// ���������һ��Token
	// ����ǰ�ÿհ��ַ���ע��
	skipWhitespace();
	// ������, currָ��ָ���һ���ǿհ�,ע�͵����ݵ��ַ�

	// ��¼��ǰ�ʷ����������ڴ����Token�Ŀ�ʼ�ַ�
	scanner.start = scanner.current;

	// ����Ѿ������˿��ַ�, �Ǿͷ���TOKEN_EOF, ��ʾ�ʷ���������
	if (isAtEnd()) {
		return makeToken(TOKEN_EOF);
	}

	// ������ǿ��ַ�, ǰ��, currָ����ǰ�ƶ�
	// ��ǰ�ƶ���ͬʱ, ���ص�ǰ���ڶ����ַ�
	char c = advance();
	if (isAlpha(c)) {
		// �������ĸ���»���, �����ʶ���Ĵ���ģʽ
		return identifier();
	}
	if (isDigit(c)) {
		// ���������, ���������Ĵ���ģʽ
		return number();
	}
	// ��������
	switch (c) {
		// �����ַ�Token
	case '(':
		return makeToken(TOKEN_LEFT_PAREN);
	case ')':
		return makeToken(TOKEN_RIGHT_PAREN);
		// ...TODO

		// ����ɵ���˫���ַ�Token
	case '+':
		if (match('+')) {
			return makeToken(TOKEN_PLUS_PLUS);
		}
		else if (match('=')) {
			return makeToken(TOKEN_PLUS_EQUAL);
		}
		else {
			return makeToken(TOKEN_PLUS);
		}
		// ... TODO

	// ���ַ�Token����, ��ʶ���������Ѿ��������
	case '"':
		return string();
	case '\'':
		return character();
	}
	return errorToken("Unexpected character.");
}