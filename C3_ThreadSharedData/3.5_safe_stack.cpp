#include <exception>
#include <memory>
#include <mutex>
#include <stack>
using namespace std;

struct empty_stack : exception {
    const char* what() const throw();
};

template <typename T>
class threadsafe_stack {
private:
    stack<int> data;
    mutable mutex m;

public:
    threadsafe_stack() { }
    threadsafe_stack(const threadsafe_stack& other)
    { // 拷贝构造函数
        lock_guard<mutex> lock(other.m);
        data = other.data;
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value)
    {
        lock_guard<mutex> lock(m);
        data.push(move(new_value));
    }

    shared_ptr<T> pop() // 方案3
    {
        lock_guard<mutex> lock(m);
        if (data.empty())
            throw empty_stack();
        shared_ptr<T> const res(make_shared<T>(data.pop()));
        data.pop();
        return res;
    }

    void pop(T& value) // 方案1+方案2
    {
        lock_guard<mutex> lock(m);
        if (data.empty())
            throw empty_stack();
        value = data.top();
        data.pop();
    }

    bool empty() const
    {
        lock_guard<mutex> lock(m);
        return data.empty();
    }
};

int main(int argc, char const* argv[])
{

    return 0;
}