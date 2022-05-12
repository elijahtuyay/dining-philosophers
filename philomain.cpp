#include <iostream>
#include <mutex>
#include <thread>
#include <array>
#include <algorithm>
#include <chrono>
#include <random>
#include <numeric>

using namespace std;

struct chopsticks{
  mutex mu;
};


int main(){
  const int count = 5;
  //int hungry=0;
  array<int,5> rngsus={0,1,2,3,4};
  unsigned rngseed = chrono::system_clock::now().time_since_epoch().count();

  auto eat = [](chopsticks &left, chopsticks &right, int id){
    //cout<<"thread "<<id<<" active\n";
    unique_lock<mutex> llock(left.mu);
    unique_lock<mutex> rlock(right.mu);
    cout << "Philosopher " << id << " is eating" << endl;
    
    chrono::milliseconds timeout(500);
    this_thread::sleep_for(timeout);
    
    cout << "Philosopher " << id << " has finished eating" << endl;
    llock.unlock();
    rlock.unlock();
  };

  //creates the chopsticks
  chopsticks sticks[count];
  
  std::thread phil[count];
  //shuffle(rngsus.begin(),rngsus.end(),default_random_engine(rngseed));
  while(true)
  { 
    //string debug = "fuck you noob";
    shuffle(rngsus.begin(),rngsus.end(),default_random_engine(rngseed));
    phil[rngsus[0]] = thread(eat, ref(sticks[rngsus[0]]),ref(sticks[rngsus[count-1]]),rngsus[0]+1);
    
    for(int i=1;i<count;i++)
    {
      //cout<<"philosopher "<<rngsus[i]+1<<" is reading\n";
      //cout<<i<<endl;
      phil[rngsus[i]] = thread(eat, ref(sticks[rngsus[i]]), ref(sticks[rngsus[i-1]]),rngsus[i]+1);
      //cout<<"\n";
      
    }
    
    

    /*
    shuffle(rngsus.begin(),rngsus.end(),default_random_engine(rngseed));
    phil[rngsus[0]] = thread(eat, ref(sticks[rngsus[0]]),ref(sticks[rngsus[count-2]]),rngsus[0]+1);
    
    for(int i=1;i<count;i++)
    {
      //cout<<"philosopher "<<rngsus[i]+1<<" is reading\n";
      phil[rngsus[i]] = thread(eat, ref(sticks[rngsus[i]]),ref(sticks[rngsus[i]-1]),rngsus[i]+1);
      //cout<<"\n";
    }
    */
    //int checksum = accumulate(sticks->ready,sticks->ready+5,checksum);
    //if(checksum>=5){
    
    //}

    
    for(auto &philosopher: phil) 
    {
        philosopher.join();
    }
    
    /*
    cout << "Philosopher " << (0+1) << " is reading.." << endl;
    phil[0] = thread(eat, ref(sticks[0]), ref(sticks[count-1]), (0+1));
    
    for(int i = 1; i < count; i-=-1) 
    { 
        
        cout << "Philosopher " << (i+1) << " is reading.." << endl;
        phil[i] = thread(eat, ref(sticks[i]), ref(sticks[i-1]), (i+1));
          //cout<<endl;
    }
    */
   /*
    for(auto &ph: phil) 
    {
        ph.join();
    }
  */
    
  }


}