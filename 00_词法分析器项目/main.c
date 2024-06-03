// main.c
#include "scanner.h"

// ��Ԫ���Դ���
static void run(const char* source) {
	initScanner(source);	// ��ʼ������ʷ�������
	int line = -1;
	// ��ӡToken, ����TOKEN_EOFΪֹ
	for (;;) {
		Token token = scanToken();	// ��ȡ��һ��Token
		if (token.line != line) {
			// �����ǰToken���кź���һ��Token��һ��, �൱�ڴ��뻻��
			// ���Ǵ�ӡ�к�
			printf("%4d ", token.line);
			line = token.line;
		}
		else {
			// �����ǰToken���кź���һ��Tokenһ��, ����û�л���
			// û�л��оʹ�ӡ����
			printf("   | ");
		}
		printf("%2d '%.*s'\n", token.type, token.length, token.start);

		if (token.type == TOKEN_EOF) {
			break;
		}
	}
}

static void repl() {
	// ���û��������û�ÿ����һ�д��룬����һ�д��룬����������
	// Ȼ������ȴ��û�����
	// repl��"read evaluate print loop"����д
	/*
		�������,���ȿ϶�����ѭ��
		��Ҫһ��һ�еĶ��û��ļ�������,fgets�������������ж�һ��
		run�����Ǻ��Ĵ���,�����߼�, ���Ҫȥ����run����
	*/
}

static char* readFile(const char* path) {
	// �û������ļ������������ļ������ݶ����ڴ棬����ĩβ���'\0'
	// ע��: ����Ӧ��ʹ�ö�̬�ڴ���䣬���Ӧ������ȷ���ļ��Ĵ�С��
	// TODO
}

static void runFile(const char* path) {
	// ����'.c'�ļ�:�û������ļ��������������ļ�������������
	char* source = readFile(path);
	run(source);
	free(source);
}

// �������в�����main����
int main(int argc, const char* argv[]) {
	if (argc == 1) {
		// ./scanner û�в���,����뽻��ʽ����
		repl();
	}
	else if (argc == 2) {
		// ./scanner file �����һ������,����������ļ�
		runFile(argv[1]);
	}
	else {
		fprintf(stderr, "Usage: scanner [path]\n");
		exit(1);
	}

	return 0;
}