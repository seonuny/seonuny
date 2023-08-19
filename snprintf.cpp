#include <stdio.h>

int main(int argc, char **argv)
{
    char cWDay = 1+'0';
    printf("%c\n",cWDay);
    printf(" [%%*.*d][%*.*d]\n"  ,4,5,21);
    printf("%8d [%%02.2s][%02.2d]\n"   ,123,123);
    printf("%8s [%%7.*s][%7.*s]\n"   ,"123"  ,2,"123");
    printf("%8s [%%*.s][%*.s]\n"    ,"123"  ,2,"123");
    printf("%8s [%%-*s][%-*s]\n"    ,"123"  ,2,"123");
    printf("%8s [%%-.*s][%-.*s]\n"  ,"123"  ,2,"123");
    printf("%8s [%%*.*s][%*.*s]\n"  ,"123",5,2,"123");
    printf("%8s [%%-*.*s][%-*.*s]\n","123",5,2,"123");

    char message[] = "Hello 123, it's good to see you.";

    snprintf(&message[6],4,"Joe");
    printf("message[%s]\n",message);

	char buffer [100];
	int cx;

	cx = snprintf ( buffer, 7, "The half of %d is %d", 60, 60/2 );

    printf("[%d]\n",cx);

	if (cx>=0 && cx<100)      // check returned value

		snprintf ( buffer+cx, 100-cx, ", and the half of that is %d.", 60/2/2 );

	puts (buffer);


    printf("%*.*s%s\n",0,0,">","abc");
    printf("%*.*s%s\n",1,1,">","abc");
    printf("%*.*s%s\n",2,2,">","abc");
    printf("%*.*s%s\n",3,3,">","abc");
    printf("%*.*s%s\n",4,4,">","abc");
    printf("%*.*s%s\n",5,5,">","abc");
    printf("%*.*s%s\n",6,6,">","abc");
    printf("%*.*s%s\n",7,7,">","abc");
    printf("%*.*s%s\n",8,8,">","abc");
    printf("%*.*s%s\n",9,9,">","abc");
    printf("%*.s%s\n",0,">","abc");
    printf("%*.s%s\n",1,">","abc");
    printf("%*.s%s\n",2,">","abc");
    printf("%*.s%s\n",3,">","abc");
    printf("%*.s%s\n",4,">","abc");
    printf("%*.s%s\n",5,">","abc");
    printf("%*.s%s\n",6,">","abc");
    printf("%*.s%s\n",7,">","abc");
    printf("%*.s%s\n",8,">","abc");
    printf("%*.s%s\n",9,">","abc");
    printf("%.*s%s\n",0,">","abc");
    printf("%.*s%s\n",1,">","abc");
    printf("%.*s%s\n",2,">","abc");
    printf("%.*s%s\n",3,">","abc");
    printf("%.*s%s\n",4,">","abc");
    printf("%.*s%s\n",5,">","abc");
    printf("%.*s%s\n",6,">","abc");
    printf("%.*s%s\n",7,">","abc");
    printf("%.*s%s\n",8,">","abc");
    printf("%.*s%s\n",9,">","abc");
    printf("%*s%s\n",0,">","abc");
    printf("%*s%s\n",1,">","abc");
    printf("%*s%s\n",2,">","abc");
    printf("%*s%s\n",3,">","abc");
    printf("%*s%s\n",4,">","abc");
    printf("%*s%s\n",5,">","abc");
    printf("%*s%s\n",6,">","abc");
    printf("%*s%s\n",7,">","abc");
    printf("%*s%s\n",8,">","abc");
    printf("%*s%s\n",9,">","abc");


    return 0;
}
