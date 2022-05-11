#include <chrono>
#include <mutex>
#include <random>
#include <array>
#include <vector>
#include <thread>
#include <iostream>
#include <ctime>

using namespace std;

// operator overload via template metaprogramming to replace default std::lock
template <class L0, class L1>
void lock(L0& l0, L1& l1)
{
    while (true)
    {
        {
            unique_lock<L0> u0(l0);
            if (l1.try_lock())
            {
                u0.release();
                break;
            }
        }
        this_thread::yield();
        
        {
            unique_lock<L1> u1(l1);
            if (l0.try_lock())
            {
                u1.release();
                break;
            }
        }
        this_thread::yield();
    }
}

class Philosopher
{
    // mt19937 eng_{std::random_device{}()};
    mt19937 mt{(time(nullptr))};

    mutex& left_fork_;
    mutex& right_fork_;
    chrono::milliseconds eat_time_{10};
    chrono::seconds full_{5};

public:
    Philosopher(std::mutex& left, std::mutex& right);
    void dine();

private:
    void eat();
    bool flip_coin();
    std::chrono::milliseconds get_eat_duration();
};

chrono::seconds Philosopher::full_;

Philosopher::Philosopher(std::mutex& left, std::mutex& right)
    : left_fork_(left)
    , right_fork_(right)
{}

void Philosopher::dine()
{
    while (eat_time_ < full_)
        eat();
}

void Philosopher::eat()
{
    using Lock = std::unique_lock<std::mutex>;
    Lock first;
    Lock second;
    if (flip_coin())
    {
        first = Lock(left_fork_, std::defer_lock);
        second = Lock(right_fork_, std::defer_lock);
    }
    else
    {
        first = Lock(right_fork_, std::defer_lock);
        second = Lock(left_fork_, std::defer_lock);
    }
    auto d = get_eat_duration();
    ::lock(first, second);
    auto end = std::chrono::steady_clock::now() + d;
    while (std::chrono::steady_clock::now() < end)
        ;
    eat_time_ += d;
}

bool Philosopher::flip_coin()
{
    std::bernoulli_distribution d;
    return d(eng_);
}

std::chrono::milliseconds Philosopher::get_eat_duration()
{
    std::uniform_int_distribution<> ms(1, 10);
    return std::min(std::chrono::milliseconds(ms(eng_)), full_ - eat_time_);
}

int main()
{
    std::cout << "SMART_POLITE\n";

	unsigned nt = 5;
    std::vector<std::mutex> table(nt);
    std::vector<Philosopher> diners;
    for (unsigned i = 0; i < table.size(); ++i)
    {
        int j = i;
        int k = j < table.size() -1 ? j+1 : 0;
        diners.push_back(Philosopher(table[j], table[k]));
    }
    
    std::vector<std::thread> threads(diners.size());
    unsigned i = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    
    for (auto& t : threads)
    {
        t = std::thread(&Philosopher::dine, diners[i]);
        ++i;
    }
    
    for (auto& t : threads)
        t.join();
    
    auto t1 = std::chrono::high_resolution_clock::now();
    using secs = std::chrono::duration<float>;
    std::cout << "nt = " << nt << " : " << secs(t1-t0).count() << std::endl;
}

/*
#include <chrono>
#include <mutex>
#include <random>
#include <array>
#include <vector>
#include <thread>
#include <iostream>

// operator overload via template metaprogramming to replace default std::lock
template <class L0, class L1>
void lock(L0& l0, L1& l1)
{
    while (true)
    {
        {
            std::unique_lock<L0> u0(l0);
            if (l1.try_lock())
            {
                u0.release();
                break;
            }
        }
        std::this_thread::yield();
        
        {
            std::unique_lock<L1> u1(l1);
            if (l0.try_lock())
            {
                u1.release();
                break;
            }
        }
        std::this_thread::yield();
    }
}

class Philosopher
{
    std::mt19937_64 eng_{std::random_device{}()};

    std::mutex& left_fork_;
    std::mutex& right_fork_;
    std::chrono::milliseconds eat_time_{0};
    static constexpr std::chrono::seconds full_{5};

public:
    Philosopher(std::mutex& left, std::mutex& right);
    void dine();

private:
    void eat();
    bool flip_coin();
    std::chrono::milliseconds get_eat_duration();
};

constexpr std::chrono::seconds Philosopher::full_;

Philosopher::Philosopher(std::mutex& left, std::mutex& right)
    : left_fork_(left)
    , right_fork_(right)
{}

void Philosopher::dine()
{
    while (eat_time_ < full_)
        eat();
}

void Philosopher::eat()
{
    using Lock = std::unique_lock<std::mutex>;
    Lock first;
    Lock second;
    if (flip_coin())
    {
        first = Lock(left_fork_, std::defer_lock);
        second = Lock(right_fork_, std::defer_lock);
    }
    else
    {
        first = Lock(right_fork_, std::defer_lock);
        second = Lock(left_fork_, std::defer_lock);
    }
    auto d = get_eat_duration();
    ::lock(first, second);
    auto end = std::chrono::steady_clock::now() + d;
    while (std::chrono::steady_clock::now() < end)
        ;
    eat_time_ += d;
}

bool Philosopher::flip_coin()
{
    std::bernoulli_distribution d;
    return d(eng_);
}

std::chrono::milliseconds Philosopher::get_eat_duration()
{
    std::uniform_int_distribution<> ms(1, 10);
    return std::min(std::chrono::milliseconds(ms(eng_)), full_ - eat_time_);
}

int main()
{
    std::cout << "SMART_POLITE\n";

	unsigned nt = 5;
    std::vector<std::mutex> table(nt);
    std::vector<Philosopher> diners;
    for (unsigned i = 0; i < table.size(); ++i)
    {
        int j = i;
        int k = j < table.size() -1 ? j+1 : 0;
        diners.push_back(Philosopher(table[j], table[k]));
    }
    
    std::vector<std::thread> threads(diners.size());
    unsigned i = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    
    for (auto& t : threads)
    {
        t = std::thread(&Philosopher::dine, diners[i]);
        ++i;
    }
    
    for (auto& t : threads)
        t.join();
    
    auto t1 = std::chrono::high_resolution_clock::now();
    using secs = std::chrono::duration<float>;
    std::cout << "nt = " << nt << " : " << secs(t1-t0).count() << std::endl;
}
*/