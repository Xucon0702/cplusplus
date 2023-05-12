#include "classTemplate.h"
#include <iostream>

using namespace std;

template <typename T1>
class phone<double, T1>{

};

int main()
{
    phone<int, char> MI;        //实例化时确定各个typename的类型：T0,T1等等
    MI.name0 = 10;
    MI.name1 = 'c';
    phone<double, char> HW;     //同上&&特化

    // HW.name1 = 10;

    cout<<MI.getname0()<<"\t"<<MI.name1<<"\n";

    showName("swh");
    prtName();
    
    cout<<"game over"<<endl;
}
