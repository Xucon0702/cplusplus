#include<cstdio>
#include <string.h>


int main()
{

    printf("SIMU=%s\n",SIMU);
    if(!strncmp(SIMU,"ON",2))
    {
        printf("hello world 111 \n");
    }
    else
    {
        printf("simulation off 111\n");
    }


#ifdef  SIMU
    printf("hello world 222\n");
#else
    printf("simulation off\n");
#endif

// #if(SIMU == "hello")
//     printf("hello world 222\n");
// #else
//     printf("simulation off\n");
// #endif

    return 0;
}