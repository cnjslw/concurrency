#include <limits.h>
#include <public.h>
class hierarchical_mutex {
private:
    mutex internal_mutex;
    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    static thread_local unsigned long this_thread_hierarchy_value;
    void check_for_hierarchy_violation()
    {
        if (this_thread_hierarchy_value <= hierarchy_value) {
            throw logic_error("mutex hierarchy violated");
        }
    }

    void update_hierarchy_value()
    {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }

public:
    explicit hierarchical_mutex(unsigned long value)
        : hierarchy_value(value)
        , previous_hierarchy_value(0)
    {
    }
    void lock()
    {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }

    void unlock()
    {
        if (this_thread_hierarchy_value != hierarchy_value)
            throw logic_error("mutex hierarchy violated");
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }

    bool try_lock()
    {
        check_for_hierarchy_violation();
        if (internal_mutex.try_lock())
            return false;
        update_hierarchy_value();
        return true;
    }
    ~hierarchical_mutex() { }
};

thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

hierarchical_mutex high_level_mutex(10000); // ①

hierarchical_mutex low_level_mutex(5000); // ②

hierarchical_mutex other_mutex(6000); // ③

int do_low_level_stuff();
int low_level_func()
{
    lock_guard<hierarchical_mutex> lk(low_level_mutex); // ④
    return do_low_level_stuff();
}

void high_level_stuff(int some_param);
void high_level_func()
{
    lock_guard<hierarchical_mutex> lk(high_level_mutex); // ⑥
    high_level_stuff(low_level_func()); // ⑤
}

void thread_a() // ⑦
{
    high_level_func();
}

void do_other_stuff();
void other_stuff()
{
    high_level_func(); // ⑩
    do_other_stuff();
}

void thread_b() // ⑧
{
    lock_guard<hierarchical_mutex> lk(other_mutex); // ⑨
    other_stuff();
}