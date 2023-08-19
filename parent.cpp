#include <stdio.h>

class CParent 
{
    public:
        CParent() { printf("Parent Cons\n"); }
       virtual ~CParent() { printf("Parent Dist\n"); }
};

class CChild : public CParent
{
    public:
        CChild() { printf("Child Cons\n"); }
       ~CChild() { printf("Child Dist\n"); }
};

int main(int argc, char**argv)
{
    CParent * parent = new CParent();


    delete parent;

    return 1;
}
