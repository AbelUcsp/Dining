// Copyright [year] <Copyright Owner
#include<bits/stdc++.h>
#include <mutex>
#include <random>
#include <thread>


const unsigned int kNumberPhilosophers = 5;
const unsigned int kNumberActions = 2;

int get_random(int low, int high) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distribution(low, high);
  return distribution(gen);
}


class DiningPhilosophers;

struct Philosopher {
  Philosopher(unsigned int id, std::string name, unsigned int number_actions,
              DiningPhilosophers* table)
      : id_(id), name_(name), number_actions_(number_actions), table_(table) {}

  void operator()() {
    for (unsigned int i = 0; i < number_actions_; ++i) {
      Eat();
    }
  }

  void Think();
  void Eat();

  unsigned int id_;
  std::string name_;
  unsigned int number_actions_;
  DiningPhilosophers* table_;
};

class DiningPhilosophers {
 public:
  DiningPhilosophers() {
    std::thread* philosophers[kNumberPhilosophers];
    std::string philosopher_names[] = {
        "Socrates", "Plato", "Aristotle", "Descartes", "Tomas",
    };

    forks.reserve(kNumberPhilosophers);
    for(int i=0; i<kNumberPhilosophers; ++i){
      forks.push_back( std::make_pair( true, false ) );
    }
    forks[4].second = true;
    maximo = 4;


    for (unsigned int i = 0; i < kNumberPhilosophers; ++i) {
      philosophers[i] = new std::thread(
          Philosopher(i, philosopher_names[i], kNumberActions, this));
    }
    for (unsigned int i = 0; i < kNumberPhilosophers; ++i) {
      philosophers[i]->join();
    }
  }

  void get_max_(int id){
    int J;
    int i=0;
    int j=id;
    //for(int i=0; i<kNumberPhilosophers; ++i){
    while(i<kNumberPhilosophers){
        //if((forks[i % kNumberPhilosophers].first == forks[(i+1) % kNumberPhilosophers].first) == true and ) 
        if( (forks[(j+1) % kNumberPhilosophers].first == forks[(j+1) % kNumberPhilosophers].second  ) == true ){
          //std::cout << "LIBRE" << std::endl;
          //J = i+1;
          J = i;
        }
        i++;
        j = (j + 1)% kNumberPhilosophers;
    }
    //return J;
    maximo = J % kNumberPhilosophers;
    std::cout << "maximo " << maximo << std::endl;
    //return maximo;
  }


  void set_max_(){
    int J;
    for(int i=0; i<kNumberPhilosophers; ++i){
        if( (forks[(i+1) % kNumberPhilosophers].first == forks[(i+1) % kNumberPhilosophers].second  ) == true ){
          
          J = i+1;
        }
    }
    maximo = J;

  }

  void PickFork(unsigned int id) {

    forks_[id].lock();
    //int maximo = get_max_();;
    forks_[maximo].lock();
    get_max_(id);

    forks[id].first = false;
    forks[maximo].first = false;
    forks[id].second = false;
    forks[maximo].second = false;
    
  }

  void LeaveFork(unsigned int id) {    
    

    forks[id].first = true;
    forks[maximo].first = true;
    forks[id].second = true;
    forks[maximo].second = true;
    //set_max_();
    forks_[id].unlock();
    forks_[maximo].unlock();

  }

 int maximo;
 std::vector< std::pair <bool, bool> > forks; // first desbloqueado(true) bloqueado(false)
                                              // second max(true)         min(false)
 private:
  std::mutex forks_[kNumberPhilosophers];
};

void Philosopher::Think() {
  std::cout << name_ + " is Thinking." << std::endl;
  // std::this_thread::sleep_for(std::chrono::seconds(get_random(1, 6)));
}

void Philosopher::Eat() {
  table_->PickFork(id_);
  
  std::cout << name_ + " is Eating." << std::endl;
  // std::this_thread::sleep_for(std::chrono::seconds(get_random(1, 6)));
  table_->LeaveFork(id_);
}

int max_(std::vector< std::pair <bool, bool> > forks){
  //int max = forks[0].first
  int J;
  for(int i=0; i<kNumberPhilosophers; ++i){
      forks.push_back( std::make_pair( true, false ) );
  }
  forks[2].second = true;

  for(int i=0; i<kNumberPhilosophers-1; ++i){
      //if((forks[i % kNumberPhilosophers].first == forks[(i+1) % kNumberPhilosophers].first) == true and ) 
      if( (forks[(i+1) % kNumberPhilosophers].first == forks[(i+1) % kNumberPhilosophers].second  ) == true ){
         std::cout << "LIBRE" << std::endl;
        J = i+1;
      }
  }
  return J;
}

int main() {
  DiningPhilosophers table;

  /*

  std::vector< std::pair <bool, bool> > forks;
  forks.reserve(kNumberPhilosophers);
  max_(forks);
  

  std::cout << forks.size() << std::endl;
  std::cout << "MAXIMP "<< max_(forks);
*/
  return 0;
}

/*

class DiningPhilosophers;
class Waitress;

struct Philosopher {
  
  Philosopher(unsigned int id, std::string name, unsigned int number_actions,
              DiningPhilosophers* table, Waitress *waitress_)
      : id_(id), name_(name), number_actions_(number_actions), table_(table), waitress(waitress_){}

  void operator()();

  void Eat();

  unsigned int id_;
  std::string name_;
  unsigned int number_actions_;
  DiningPhilosophers* table_;
  Waitress *waitress;
};


class Waitress {
 public:
  Waitress(DiningPhilosophers *table): table_(table){
    for(int i=0; i<kNumberPhilosophers; ++i)
      forks[i] = true;

    std::thread* philosophers[kNumberPhilosophers];
    std::string philosopher_names[] = {
        "Socrates", "Plato", "Aristotle", "Descartes", "Tomas", };

    for (unsigned int i = 0; i < kNumberPhilosophers; ++i) {
      philosophers[i] = new std::thread(
          Philosopher(i, philosopher_names[i], kNumberActions, table_, this));
    }
    for (unsigned int i = 0; i < kNumberPhilosophers; ++i) {
      philosophers[i]->join();
    }

  }


  bool forks[kNumberPhilosophers];
  DiningPhilosophers *table_;
  std::mutex camarera;

};


class DiningPhilosophers {
 public:
  DiningPhilosophers() {
    waitress_ = new Waitress(this);
    
  }

  void PickFork(unsigned int id) {
    forks_[id].lock();
    forks_[(id + 1) % kNumberPhilosophers].lock();
    waitress_->forks[id] = false;
    waitress_->forks[(id + 1) % kNumberPhilosophers] = false;
  }

  void LeaveFork(unsigned int id) {
    forks_[id].unlock();
    forks_[(id + 1) % kNumberPhilosophers].unlock();

    waitress_->forks[id] = true;
    waitress_->forks[(id + 1) % kNumberPhilosophers] = true;
  }

  Waitress *waitress_;
 private:
  std::mutex forks_[kNumberPhilosophers];
};


void Philosopher::Eat(){
  table_->PickFork(id_);
  std::cout << name_ + " is Eating." << std::endl;
  // std::this_thread::sleep_for(std::chrono::seconds(get_random(1, 6)));
  table_->LeaveFork(id_);
}


void Philosopher::operator()(){
  for (unsigned int i = 0; i < number_actions_; ++i) {
      while(true){
        if( (waitress->forks[id_] == waitress->forks[(id_+1) % kNumberPhilosophers])==true ){
          waitress->camarera.lock();
          Eat();
          waitress->camarera.unlock();
          break;
        }
      }
  }
}



int main() {
  DiningPhilosophers table;


  std::cout << " Termino " << std::endl;
  return 0;
}
*/