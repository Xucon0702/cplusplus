#include<cstdio>
#include <cstring>

typedef struct 
{
    float x;
    float y;
}pos_2d;

int delInt(int& a,int b)
{
    return a-b;
}

#define ZERO_JUDGE_VALUE (0.000001)
#define COMPARE_ZEOR(src,IDX)\
    (((src[IDX].x >= -ZERO_JUDGE_VALUE)&&(src[IDX].x <= ZERO_JUDGE_VALUE))&&((src[IDX].y >= -ZERO_JUDGE_VALUE)&&(src[IDX].y <= ZERO_JUDGE_VALUE)))

int main()
{
    pos_2d tPos[4];
    memset(&tPos,0,sizeof(pos_2d));

    tPos[0].x = 1.0;

    if(COMPARE_ZEOR(tPos,0)&&COMPARE_ZEOR(tPos,1))
        printf("zero\n");

    int a_tmp = 12;
    int b_tmp = 10;

    a_tmp = delInt(a_tmp,b_tmp);

    printf("main exit,%d\n",a_tmp);
    return 0;
}