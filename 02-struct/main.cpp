#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

typedef struct _person
{
    string name;
    int age;
}Person;

int main()
{
    Person a;
    Person b;

    printf("a:0x%x;b:0x%x\n",a,b);

    b.age = 10;
    b.name = "xiaoming";

    cout<<"a:"<<a.name<<a.age<<";"<<"b:"<<b.name<<b.age<<endl;

    a=b;
    printf("a:0x%x;b:0x%x\n",a,b);
    cout<<"a:"<<a.name<<a.age<<";"<<"b:"<<b.name<<b.age<<endl;

    return 0;
}