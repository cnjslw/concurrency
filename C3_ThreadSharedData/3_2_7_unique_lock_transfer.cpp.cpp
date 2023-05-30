#include <memory>
#include <mutex>

class Data {
public:
    int getValue() const { return value; }
    void setValue(int v) { value = v; }

private:
    int value;
};

class Channel {
public:
    explicit Channel(std::unique_ptr<Data> data)
        : mData(std::move(data))
    {
    }

    std::unique_lock<std::mutex> getLock()
    {
        return std::unique_lock<std::mutex>(mMutex);
    }

    int getValue() const
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return mData->getValue();
    }

    void setValue(int v)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mData->setValue(v);
    }

private:
    std::unique_ptr<Data> mData;
    mutable std::mutex mMutex;
};

int main()
{
    auto data = std::make_unique<Data>();
    Channel channel(std::move(data));

    // 通过通道对象访问受保护数据
    {
        std::unique_lock<std::mutex> lock = channel.getLock();
        int value = channel.getValue();
        channel.setValue(value + 1);
    }

    // 另一种访问方式
    {
        std::lock_guard<std::mutex> lock(channel.getLock());
        int value = channel.getValue();
        channel.setValue(value + 2);
    }

    return 0;
}

/*
在上述代码中，Data类表示需要受保护的数据。Channel类表示通道（gate way），
其数据成员为指向Data对象的智能指针mData，以及一个可变的互斥量mMutex。
Channel类有三个成员函数：getLock()用于获取互斥锁对象；getValue()用于获取数据的值；
setValue()用于设置数据的值。
在main()函数中，我们首先创建了一个Data对象，
并将其转移给一个Channel对象。然后，
我们使用两种方式访问受保护的数据：
第一种方式是通过调用channel.getLock()获取互斥锁对象，
再使用channel.getValue()和channel.setValue()访问数据。
第二种方式是使用std::lock_guard<std::mutex>构造函数自动获取互斥锁对象，
并使用channel.getValue()和channel.setValue()访问数据。

需要注意的是，在上述代码中，Channel类的对象是可移动的，
因此其数据成员mMutex也必须是可移动的。在实现中，我们使用了std::unique_lock<std::mutex>作为返回值类型，
以支持锁对象的转移。


两种访问方式的主要不同在于获取互斥锁对象的方式不同。
第一种方式通过调用channel.getLock()显式获取互斥锁对象，
而第二种方式通过使用std::lock_guard<std::mutex>构造函数自动获取互斥锁对象。

第二种方式相对来说更加简洁、方便，可以减少代码量和易错点。
同时，由于使用了RAII技术，确保了互斥锁对象在作用域结束时会被自动释放，
从而避免了因忘记释放锁对象而导致的死锁等问题。

但是，第二种方式也有一些缺点。比如，如果需要在获取锁对象之后执行一些其他操作，
就需要单独创建一个代码块，增加了代码复杂度。
此外，如果需要在获取锁之后将锁对象传递给其他函数或线程，就必须手动管理锁对象的生命周期，
容易出现错误。

总的来说，两种访问方式各有优劣，应根据具体场景选择合适的方式。
当需要执行多个操作时，建议使用第一种方式显式获取锁对象，
并在所有操作完成后手动释放锁对象；
当只需要执行单个操作时，建议使用第二种方式自动获取和释放锁对象。
*/
