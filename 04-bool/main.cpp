#include <cstdio>
#include <stdint.h>



int64_t Sub(const int64_t lData,const int64_t rData)
{
    return (int64_t)lData - (int64_t)rData;
}


int main()
{
    int a = 1;
    int b = 5;
    if(Sub(a,b))
    {
        printf("11111111111\n");
    }
    else
    {
        printf("222222222222\n");
    }

    uint64_t t1 = 10;
    uint64_t t2 = 25;
    uint64_t t3 = 18;

    float fa = ((float)t3 - (float)t1)/((float)t2 - (float)t1);
    uint64_t t4 = t1 + fa*(t2 - t1);


    printf("fa = %f,t4 = %ld\n",fa,t4);
    return 0;
}