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

//locks threads
struct chopsticks
{
  mutex mu;
};

//checks if the philosopher is eating
struct check
{
  int eating[5];
};


void eatfunc(chopsticks &left, chopsticks &right, check &eaters, int id)
{
  //locks the mutex
  unique_lock<mutex> llock(left.mu);
  unique_lock<mutex> rlock(right.mu);
  
  //the act of eating
  eaters.eating[id]=1;
  chrono::milliseconds timeout(100); 
  this_thread::sleep_for(timeout);
  
  //unlocks the mutexes
  llock.unlock();
  rlock.unlock();
  //eaters are not eating anymore
  eaters.eating[id]=0;
}


void showstats(check &eaters, vector<int> &eat, vector<int> &wait)
{
  //press enter to show whos eating
    //ONLY WORKS ON WINDOWS BECAUSE IT USES WINDOWS.H
    if(GetAsyncKeyState(VK_RETURN)==1)
    {
        system("CLS");
        for(int i=0;i<5;i++){
          //checks which philosophers are eating or waiting
          if (eaters.eating[i]==1){
            eat.push_back(i+1);
          }
          else{
            wait.push_back(i+1);
          }
        }
        //lists out eating and waiting people
        cout<<"\nEating: \n";
        for(int i=0;i<eat.size();i++)
        {
          cout<<"Philosopher "<<eat[i]<<"\n";
        }
        cout<<"\nWaiting: \n";
        for(int i=0;i<wait.size();i++)
        {
          cout<<"Philosopher "<<wait[i]<<"\n";
        }
        cout<<"\npress enter to display results\n\n";
      }
}

int main(){
  const int count = 5;
  array<int,5> rngsus={0,1,2,3,4}; //to randomly order thread generation
  unsigned rngseed = chrono::system_clock::now().time_since_epoch().count();
  check eaters;
  
  for(int i=0;i<5;i++)
  {
    eaters.eating[i]=0;
  }

  //creates the chopsticks
  chopsticks sticks[count];
  //creates the philosopher threads
  thread phil[count];
  thread gui;

  cout<<"press enter to display results\n";
  //permanent loop
  while(true)
  { 
    //starts the lists of eaters and waiters
    vector<int> eat,wait;
    //force shuffle to randomize philosopher order
    shuffle(rngsus.begin(),rngsus.end(),default_random_engine(rngseed));
    //seeding the chopsticks randomly
    phil[rngsus[0]] = thread(eatfunc, ref(sticks[rngsus[0]]),ref(sticks[rngsus[count-1]]), ref(eaters),rngsus[0]+1);
    //loop the thread creations
    for(int i=1;i<count;i++)
    {
      phil[rngsus[i]] = thread(eatfunc, ref(sticks[rngsus[i]]), ref(sticks[rngsus[i-1]]),ref(eaters),rngsus[i]+1);
    }
    //puts gui on another thread
    gui = thread(showstats,ref(eaters),ref(eat),ref(wait));

    
    //synchronizes the philosophers
    for(auto &philosopher: phil) 
    {
        philosopher.join();
    }
    gui.join();
    //resets the eat/wait lists to be reused next loop
    eat.clear();
    wait.clear();
  }
}