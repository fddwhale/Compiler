// scanner.c
#include "scanner.h"

// 词法分析器本人
typedef struct {
	// 记录当前正在处理的Token的第一个字符, 用于在生成Token时, 给token.start赋值
	// start在Token处理的过程中, 是不能移动的
	const char* start;

	// 当前词法分析器正在处理分析的字符
	// curr会从Token的start位置开始, 直到到达Token结束的下一个字符
	const char* current;
	int line;	// 指示当前正在处理的代码行
} Scanner;

// 全局变量
Scanner scanner;

void initScanner(const char* source) {
	// 初始化scanner
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

/***************************************************************************************
 *                                   辅助方法											*
 ***************************************************************************************/
 // 判断当前字符是不是字母或者下划线
static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

// 判断当前字符是不是数字
static bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

// 判断当前正在处理的字符是不是空字符, 如果是就表示处理完成了
static bool isAtEnd() {
	return *scanner.current == '\0';
}

// 前进, 返回当前正在处理的字符, 并且让curr移动到下一个字符
static char advance() {
	return *scanner.current++;
}

// 返回当前正在处理的字符, 但是不移动curr
static char peek() {
	return *scanner.current;
}

// 如果当前正在处理的字符不是空字符, 就返回下一个待处理的字符
static char peekNext() {
	if (isAtEnd()) {
		return '\0';
	}
	return *(scanner.current + 1);
}
// 判断当前正在处理的字符是不是expected,如果符合预期,那么就移动curr指针
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

// 传入TokenType, 创建对应类型的Token，并返回。
static Token makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.length = (int)(scanner.current - scanner.start);
	token.line = scanner.line;
	return token;
}

// 遇到不能解析的情况时，我们创建一个ERROR Token. 比如：遇到@，$等符号时，比如字符串，字符没有对应的右引号时。
static Token errorToken(const char* message) {
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = scanner.line;
	return token;
}

static void skipWhitespace() {
	// 跳过空白字符: ' ', '\r', '\t', '\n'和注释
	// 注释以'//'开头, 一直到行尾
	// 注意更新scanner.line！

	/*
		提示:
		1.首先肯定要死循环, 因为不知道有多少个字符
		2.先switch, 把单字符直接跳过: 比如回车符, 空格, 制表
			换行符: 处理line成员
			注释: 遇到/, 就瞥一眼下一个字符, 如果还是/ 就是注释 跳过要用while循环
	*/
}
static TokenType check_keyword(const char* rest, int start, int len, TokenType type) {
	/*
		rest, 待检查的剩余序列, 比如检查的关键字是break, b已经在switch中检查了
			rest就是reak
		start, 就是reak这个待检查序列的下标(从scanner.start开始的一个下标)
		len, 待检查的剩余序列的长度,比如检查的关键字是break,len就是4
	*/
	// 校验Token的长度是否符合
	if ((scanner.current - scanner.start) != (start + len)) {
		return TOKEN_IDENTIFIER;
	}
	/*
	int memcmp(const void *s1, const void *s2, size_t n);
	这里的参数分别是：
		s1：指向第一块内存区域的指针。
		s2：指向第二块内存区域的指针。
		n：要比较的字节数。
	memcmp 函数会逐字节比较 s1 和 s2 指向的内存区域，直到有不相等的字节或比较了 n 个字节为止。
	如果两个内存区域完全相同，
	则 memcmp 返回 0；如果第一个不同的字节在 s1 中的值小于 s2 中对应的值，返回负数；
	反之，返回正数。
	*/
	if (!memcmp(scanner.start + start, rest, len)) {
		return type;
	}
	return TOKEN_IDENTIFIER;
}
static TokenType identifierType() {
	// 进入这个函数时, sc.start指向Token的开头字符, curr指向的是该Token后面的一个字符
	// 该函数就用于判断start到curr之间的Token到底是标识符还是关键字
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
// 处理Token是标识符以及关键字的逻辑
static Token identifier() {
	// IDENTIFIER包含: 字母，数字和下划线
	// 判断当前正在处理的字符 是不是字母 下划线 数字
	// 如果是, curr指针就前进
	while (isAlpha(peek()) || isDigit(peek())) {
		advance();
	}
	// 这样的Token可能是标识符, 也可能是关键字, identifierType()是用来确定Token类型的
	return makeToken(identifierType());
}

static Token number() {
	// 简单起见，我们将NUMBER的规则定义如下:
	// 1. NUMBER可以包含数字和最多一个'.'号
	// 2. '.'号前面要有数字
	// 3. '.'号后面也要有数字
	// 这些都是合法的NUMBER: 123, 3.14
	// 这些都是不合法的NUMBER: 123., .14
	// 提示: 整个过程用循环一直遍历全部的.以及整数
}

static Token string() {
	// 字符串以"开头，以"结尾，而且不能跨行
	// 移动curr指针, 只要不是右", 那就一直移动
}

static Token character() {
	// 字符'开头，以'结尾，而且不能跨行   
}

/***************************************************************************************
 *                                   	分词											  *
 ***************************************************************************************/
Token scanToken() {	// 用来输出下一个Token
	// 跳过前置空白字符和注释
	skipWhitespace();
	// 跳过后, curr指针指向第一个非空白,注释等内容的字符

	// 记录当前词法分析器正在处理的Token的开始字符
	scanner.start = scanner.current;

	// 如果已经读到了空字符, 那就返回TOKEN_EOF, 表示词法分析结束
	if (isAtEnd()) {
		return makeToken(TOKEN_EOF);
	}

	// 如果不是空字符, 前进, curr指针向前移动
	// 向前移动的同时, 返回当前正在读的字符
	char c = advance();
	if (isAlpha(c)) {
		// 如果是字母或下划线, 进入标识符的处理模式
		return identifier();
	}
	if (isDigit(c)) {
		// 如果是数字, 进入整数的处理模式
		return number();
	}
	// 继续处理
	switch (c) {
		// 处理单字符Token
	case '(':
		return makeToken(TOKEN_LEFT_PAREN);
	case ')':
		return makeToken(TOKEN_RIGHT_PAREN);
		// ...TODO

		// 处理可单可双的字符Token
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

	// 多字符Token处理, 标识符和数字已经处理过了
	case '"':
		return string();
	case '\'':
		return character();
	}
	return errorToken("Unexpected character.");
}