#include "public.h"

class some_big_object;
void swap(some_big_object& lhs, some_big_object& rhs);
class X {
private:
    some_big_object some_detail;
    mutex m;

public:
    X(some_big_object const& sd)
        : some_detail(sd)
    {
    }
    friend void swap(X& lhs, X& rhs)
    {
        if (&lhs == &rhs)
            return;
        unique_lock<mutex> lock_a(lhs.m, defer_lock);
        unique_lock<mutex> lock_b(rhs.m, defer_lock);
        lock(lock_a, lock_b);
        swap(lhs.some_detail, rhs.some_detail);
    }
    ~X() { }
};

class some_big_object {
private:
    /* data */
public:
    some_big_object(/* args */) { }
    ~some_big_object() { }
};