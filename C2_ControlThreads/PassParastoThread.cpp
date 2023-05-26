#include "public.h"

class A {
private:
    /* data */
public:
    A(/* args */)
    {
        cout << "默认构造函数" << endl;
    }
    A(const A&)
    {
        cout << "拷贝构造函数" << endl;
    }
    A& operator=(const A&)
    {
        cout << "赋值运算符函数" << endl;
    }
    ~A()
    {
        cout << "析构函数" << endl;
    }
};
using namespace std;
void f(int i, const A& s)
{
    cout << "address s = " << &s << endl;
}
void g(int i, A& s)
{
    cout << "address s = " << &s << endl;
}

int main(int argc, char const* argv[])
{
    // string p = "hello";
    // string q = "world";
    // cout << "addrss p = " << &p << endl;

    A obj;
    cout << "------------------- A const& s -----------------" << endl;
    // thread t1(f, 3, "hello");
    // thread t2(f, 3, p);
    cout << ".. Pass By Object ..." << endl;
    thread t1(f, 3, obj);
    cout << "------------------- A & s -----------------" << endl;
    cout << ".. Pass By Object ..." << endl;
    // thread t3(g, 3, obj);
    cout << ".. Pass By ref(Object) ..." << endl;
    thread t4(g, 3, ref(obj));

    t1.join();
    // t3.join();
    t4.join();

    return 0;
}