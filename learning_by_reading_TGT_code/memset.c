#include <stdio.h>
//sizeof 头文件
// C 库函数 void *memset(void *str, int c, size_t n) 复制字符 c（一个无符号字符）到参数 str 所指向的字符串的前 n 个字符。
struct mem {
	int a;
	int b;
	int c;
	int d;
	int e;
	int f;
};
int main(void){
    struct mem test;
	struct men* p = &test;
    //memset((void *)p, 0, sizeof(struct mem));
	memset(&test, 0, sizeof(struct mem));
	//详细说明见 引用与指针.md
    printf("down\n");
    printf("%d\n", test.f);
    return 0;
}




