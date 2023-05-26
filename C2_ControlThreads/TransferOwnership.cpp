#include "public.h"

void do_something(int i);

class scoped_thread {
private:
    std::thread t;

public:
    explicit scoped_thread(std::thread t_)
        : t(std::move(t_))
    {
        if (!t.joinable()) {
            throw std::logic_error("No thread");
        }
    }
    ~scoped_thread()
    {
        t.join();
    }

    scoped_thread(scoped_thread const&) = delete;
    scoped_thread& operator=(scoped_thread const&) = delete;
};

struct func {
    int& i;
    func(int& i_)
        : i(i_)
    {
    }
    void operator()()
    {
        for (unsigned j = 0; j < 100; ++j) {
            do_something(i);
        }
    }
};
void do_something(int i)
{
    for (int k = 0; k < i; k++) {
        cout << k << " ";
    }
    cout << endl;
}
void some_function()
{
}
void some_other_function()
{
}

void f()
{
    int some_local_state = 10;
    scoped_thread t { std::thread(func(some_local_state)) };
}
int main(int argc, char const* argv[])
{
    f();
    return 0;
}