#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <errno.h>
#define TRUE 1
#define FALSE 0
//setjmp와 longjmp가 같이 사용하는 jmp_buf로
//전역적으로 사용
jmp_buf jmpbuf;
char password[32];

void check_size(){
	if(strlen(password)< 8) longjmp(jmpbuf, 0);
}
void is_possible_word(){
	int i;
	for(i=0;i<strlen(password);i++)
		if(password[i] == '|' || password[i] == '%' || password[i] == '&') longjmp(jmpbuf,2);
}
void verify_input(){
	check_size();
	is_possible_word();
}
int main(){
	int ret = 0;
	//setjmp는 나중에 longjmp를 통해서 점프되었을때실행되는 부분
	if((ret = setjmp(jmpbuf)) != 0){
		printf("setjmp error[%d][%s]\n",ret,strerror(errno));
		return -1;
	}else{
		//longjmp에서 두번째 인자 val이 ret값이 됨.
		switch(ret){
			case 0:
				printf("비밀번호는 8자 이상이어야합니다.\n");
				break;
			case 1:
				printf("비밀번호는 8자 이상이어야합니다.\n");
				break;
			case 2:
				printf("|, %%, &문자는 사용할 수 없습니다.\n");
				break;
		}
	}

	printf("비밀번호:");
	scanf("%s",password);
	verify_input();
	return 0;
}
