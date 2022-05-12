#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

struct chopsticks{
  mutex mu;
};


int main(){
  const int count = 5;
  
  auto eat = [](chopsticks &left, chopsticks &right, int id){
    unique_lock<mutex> llock(left.mu);
    unique_lock<mutex> rlock(right.mu);
    
    cout << "Philosopher " << id << " is eating" << endl;
    
    chrono::milliseconds timeout(1500);
    this_thread::sleep_for(timeout);
    
    cout << "Philosopher " << id << " has finished eating" << endl;
  };


}