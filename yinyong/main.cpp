#include <cstdio>

int test(int & p)
{
    p = 999;
    return 0;
}
int test2(int *p)
{
    *p = 888;
    return 0;
}

int main()
{
    int a = 10;
    int *b = new int(66);
    printf("a_1=%d,b = %d\n",a,*b);

    test(a);
    test2(b);
    test(*b);

    printf("a_2=%d,b = %d\n",a,*b);

    return 0;
}