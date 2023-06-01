#include <condition_variable>
#include <iostream>
#include <queue>
using namespace std;

class data_chunk { };
bool more_data_to_prepare()
{
    return true;
}

queue<data_chunk> data_queue;
condition_variable data_cond;
mutex mut;

void data_preparation_thread(data_chunk& data)
{
    while (more_data_to_prepare()) {
        lock_guard<mutex> lk(mut);
        data_queue.push(data);
    }
    data_cond.notify_one();
}

void process(data_chunk& data)
{
    cout << "processing the data" << endl;
}

bool is_last_chunk(data_chunk& data)
{
    return true;
}

void data_processing_thread()
{
    while (true) {
        unique_lock<mutex> lk(mut);
        data_cond.wait(
            lk,
            [] { return !data_queue.empty(); });
        data_chunk data = data_queue.front();
        /*
        如果线程甲重新获得互斥，并且查验条件，
        而这一行为却不是直接响应线程乙的通知，
        则称之为伪唤醒（spurious wake）。
        按照C++标准的规定，这种伪唤醒出现的数量和频率都不确定。
        故此，若判定函数有副作用，则不建议选取它来查验条件。
        */
        data_queue.pop();
        lk.unlock();
        process(data);
        if (is_last_chunk(data))
            break;
    }
}

int main()
{
    return 0;
}