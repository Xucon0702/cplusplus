#include <vector>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

struct student
{
    int age;
    std::string name;
};

struct Person
{
    int age;
    int grade;
    Person(int age, int grade) : age(age), grade(grade) {}
    // Person(){};
};

void TestPerson()
{
    Person person1(20,1);
    std::vector<Person> persons;
    // person1.age = 20;
    // person1.grade = 1;
    persons.emplace_back(person1);

    persons.emplace_back(30,50);

    for (const auto& element : persons) {
        std::cout << element.age << ' ' << element.grade << ' ';
    }
    std::cout << '\n'; 

}

int main()
{
    student s1;
    s1.age = 20;
    s1.name = "xiaoming";

    std::vector<student> students;
    students.emplace_back(s1);

    std::cout<<students[0].age<<std::endl;
    std::cout<<students[0].name<<std::endl;

    TestPerson();

    return 0;
}