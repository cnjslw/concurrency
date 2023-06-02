#include <future>
#include <string>
struct X {
    void foo(int, std::string const&);
    std::string bar(std::string const&);
};
X x;

auto f1 = std::async(&X::foo, &x, 42, "hello"); // p->foo(42,"hello"); p=&x
auto f2 = std::async(&X::bar, x, "goodbye"); // tempx.bar("goodbye");
struct Y {
    double operator()(double);
};
Y y;
auto f3 = std::async(Y(), 3.12); // tmpy(3.12)
auto f4 = std::async(std::ref(y), 2.7); // y(2.7)
X baz(X&);
class move_only {
public:
    move_only();
    move_only(move_only&&);
    move_only(move_only const&) = delete;
    move_only& operator=(move_only&&);
    move_only& operator=(move_only const&) = delete;
    void operator()();
};

auto f5 = std::async(move_only()); // tmp() ->tmp = std::move(move_only())
auto f6 = std::async(std::launch::async, Y(), 1.2);
auto f7 = std::async(std::launch::deferred, baz, std::ref(x));
auto f8 = std::async(std::launch::async | std::launch::deferred, baz, std::ref(x));
auto f9 = std::async(baz, std::ref(x));
f7.wait();