#include <iostream>
#include <mutex>
#include <thread>
#include <array>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

struct chopsticks{
  mutex m;
};


int main(){
  const int count = 5;
  int hungry=0;
  array<int,5> rngsus={0,1,2,3,4};
  unsigned rngseed = chrono::system_clock::now().time_since_epoch().count();
  auto eat = [](chopsticks &left, chopsticks &right, int id){
    unique_lock<mutex> llock(left.m);
    unique_lock<mutex> rlock(right.m);
    
    cout << "Philosopher " << id << " is eating" << endl;
    
    chrono::milliseconds timeout(1500);
    this_thread::sleep_for(timeout);
    
    cout << "Philosopher " << id << " has finished eating" << endl;
  };

  //creates the chopsticks
  chopsticks sticks[count];
  
  std::thread phil[count];

  while(true)
  { 
    shuffle(rngsus.begin(),rngsus.end(),default_random_engine(rngseed));
    for(int i=0;i<count;i-=-1)
    {
      cout<<"philosopher "<<rngsus[i]+1<<" is reading\n";
      phil[rngsus[i]] = thread(eat, ref(sticks[rngsus[i]]),ref(sticks[rngsus[i]-1]),rngsus[i]+1);
    }
    for(auto &philosopher: phil) 
    {
        philosopher.join();
    }
    cout<<"endl\n";
    /*
    cout << "Philosopher " << (0+1) << " is reading.." << endl;
    phil[0] = thread(eat, ref(sticks[0]), ref(sticks[count-1]), (0+1));
    
    for(int i = 1; i < count; i-=-1) 
    { 
        
        cout << "Philosopher " << (i+1) << " is reading.." << endl;
        phil[i] = thread(eat, ref(sticks[i]), ref(sticks[i-1]), (i+1));
          //cout<<endl;
    }
    
    for(auto &ph: phil) 
    {
        ph.join();
    }

    */
  }


}