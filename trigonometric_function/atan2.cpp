#include<stdio.h>
#include<math.h>


int main(int argc,char** argv)
{
    if(argc<5)
    {
        printf("请依次输入p0.x p0.y p1.x,p1,y\n");
        return 0;
    }

    
	float fAngle = stof(argv[2]);
    

    printf("atan2(%f) = %f\n",fAngle,atan2());
    

    return 0;
}