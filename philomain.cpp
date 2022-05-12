#include <iostream>
#include <mutex>
#include <thread>
#include <array>
#include <algorithm>
#include <chrono>
#include <random>
#include <numeric>
#include <windows.h>
#include <vector>

using namespace std;

struct chopsticks{
  mutex mu;
};

struct check{
  int eating[5];
};


void eatfunc(chopsticks &left, chopsticks &right, check &eaters, int id){
  //cout<<"thread "<<id<<" active\n";
    unique_lock<mutex> llock(left.mu);
    unique_lock<mutex> rlock(right.mu);
    //if(GetAsyncKeyState(VK_RETURN)==1){
    //cout << "Philosopher " << id << " is eating" << endl;
    //}
    eaters.eating[id]=1;
    chrono::milliseconds timeout(100);
    this_thread::sleep_for(timeout);
    //if(GetAsyncKeyState(VK_RETURN)==1){
    //cout << "Philosopher " << id << " has finished eating" << endl;
    //}
    //eaters.eating[id]=0;

    llock.unlock();
    rlock.unlock();
    eaters.eating[id]=0;
}

int main(){
  const int count = 5;
  array<int,5> rngsus={0,1,2,3,4};
  unsigned rngseed = chrono::system_clock::now().time_since_epoch().count();
  check eaters;
  
  for(int i=0;i<5;i++){
    eaters.eating[i]=0;
  }

  //creates the chopsticks
  chopsticks sticks[count];
  
  std::thread phil[count];
  cout<<"press enter to display results\n";
  //shuffle(rngsus.begin(),rngsus.end(),default_random_engine(rngseed));
  while(true)
  { 
    vector<int> eat,wait;

    shuffle(rngsus.begin(),rngsus.end(),default_random_engine(rngseed));
    //phil[rngsus[0]] = thread(eat, ref(sticks[rngsus[0]]),ref(sticks[rngsus[count-1]]), ref(eaters),rngsus[0]+1);
    phil[rngsus[0]] = thread(eatfunc, ref(sticks[rngsus[0]]),ref(sticks[rngsus[count-1]]), ref(eaters),rngsus[0]+1);


    for(int i=1;i<count;i++)
    {
      //cout<<"philosopher "<<rngsus[i]+1<<" is reading\n";
      //cout<<i<<endl;
      phil[rngsus[i]] = thread(eatfunc, ref(sticks[rngsus[i]]), ref(sticks[rngsus[i-1]]),ref(eaters),rngsus[i]+1);
      //cout<<"\n";
      
    }
    
    //press enter to show whos eating
    
    if(GetAsyncKeyState(VK_RETURN)==1){
        system("CLS");
        //int agg=0;
        for(int i=0;i<5;i++){
          //agg+=eaters.eating[i];
          if (eaters.eating[i]==1){
            eat.push_back(i+1);
          }
          else{
            wait.push_back(i+1);
          }
        }
        
        cout<<"\nEating: \n";
        for(int i=0;i<eat.size();i++){
          cout<<"Philosopher "<<eat[i]<<"\n";
        }
        cout<<"\nWaiting: \n";
        for(int i=0;i<wait.size();i++){
          cout<<"Philosopher "<<wait[i]<<"\n";
        }
        cout<<"\npress enter to display results\n\n";
      }
    
    for(auto &philosopher: phil) 
    {
        philosopher.join();
    }
    eat.clear();
    wait.clear();
    
    
    
  }


}