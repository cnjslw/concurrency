#include <public.h>
using namespace std;

class some_data {
    int a;
    int b;

public:
    void do_something()
    {
        cout << "更改数据" << endl;
    }
};

class data_wrapper {
private:
    some_data data;
    mutex m;

public:
    template <typename Function>
    void proccess_data(Function func)
    {
        lock_guard<mutex> l(m);
        func(data); // malicious_function(data) 引用传递
    }
};

some_data* unprotected;
void malicious_function(some_data& protected_data)
{
    unprotected = &protected_data;
}

data_wrapper x;

void foo()
{
    x.proccess_data(malicious_function); // 传入恶意函数
    unprotected->do_something(); // 绕过保护直接处理数据
}

int main(int argc, char const* argv[])
{

    return 0;
}