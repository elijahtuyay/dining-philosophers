#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

struct chopsticks{
  mutex mu;
};


int main(){
  const int count = 5;
  
  auto eat = [](chopsticks &left, chopsticks &right, int id);


}