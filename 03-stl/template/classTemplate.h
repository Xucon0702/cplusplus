#include<iostream>


using namespace std;


// 函数模板
template<typename T>  void showName(T *name);


template<typename T>
void showName(T *name)
{
    cout<<*name<<endl;
}




// 类模板
template <typename T0, typename T1> //除了特殊用法外所有的模板T都需要定义在这
class phone
{
public:
    phone(){};
    ~phone(){};
    T0 name0;
    T1 name1;
    T0 getname0()
    {
        return name0;
    }
};


void prtName();
