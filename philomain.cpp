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
    unique_lock<mutex> llock(left_chopstics.mu);
    unique_lock<mutex> rlock(right_chopstics.mu);
    
    cout << "Philosopher " << philosopher_number << " is eating" << endl;
    
    chrono::milliseconds timeout(1500);
    this_thread::sleep_for(timeout);
    
    cout << "Philosopher " << philosopher_number << " has finished eating" << endl;
  }


}