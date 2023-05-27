#include <public.h>

list<int> some_list;
mutex some_mutex;

void add_to_list(int new_value)
{
    lock_guard<mutex> guard(some_mutex);
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find)
{
    std::lock_guard<mutex> guard(some_mutex);
    // C++17
    // std::lock_guard(some_mutex);
    // or
    // scoped_lock(some_mutex);
    return find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

int main(int argc, char const* argv[])
{

    return 0;
}