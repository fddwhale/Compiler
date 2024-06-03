// main.c
#include "scanner.h"

// 单元测试代码
static void run(const char* source) {
	initScanner(source);	// 初始化这个词法分析器
	int line = -1;
	// 打印Token, 遇到TOKEN_EOF为止
	for (;;) {
		Token token = scanToken();	// 获取下一个Token
		if (token.line != line) {
			// 如果当前Token的行号和上一个Token不一样, 相当于代码换行
			// 于是打印行号
			printf("%4d ", token.line);
			line = token.line;
		}
		else {
			// 如果当前Token的行号和上一个Token一样, 代码没有换行
			// 没有换行就打印竖线
			printf("   | ");
		}
		printf("%2d '%.*s'\n", token.type, token.length, token.start);

		if (token.type == TOKEN_EOF) {
			break;
		}
	}
}

static void repl() {
	// 与用户交互，用户每输入一行代码，分析一行代码，并将结果输出
	// 然后继续等待用户输入
	// repl是"read evaluate print loop"的缩写
	/*
		这个过程,首先肯定是死循环
		需要一行一行的读用户的键盘输入,fgets从任意输入流中读一行
		run函数是核心代码,核心逻辑, 最后要去调用run函数
	*/
}

static char* readFile(const char* path) {
	// 用户输入文件名，将整个文件的内容读入内存，并在末尾添加'\0'
	// 注意: 这里应该使用动态内存分配，因此应该事先确定文件的大小。
	// TODO
}

static void runFile(const char* path) {
	// 处理'.c'文件:用户输入文件名，分析整个文件，并将结果输出
	char* source = readFile(path);
	run(source);
	free(source);
}

// 带命令行参数的main函数
int main(int argc, const char* argv[]) {
	if (argc == 1) {
		// ./scanner 没有参数,则进入交互式界面
		repl();
	}
	else if (argc == 2) {
		// ./scanner file 后面跟一个参数,则分析整个文件
		runFile(argv[1]);
	}
	else {
		fprintf(stderr, "Usage: scanner [path]\n");
		exit(1);
	}

	return 0;
}