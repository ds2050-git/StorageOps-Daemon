#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<deque>
#include<thread>
#include<mutex>
#include<queue>
#include<condition_variable>

struct Record{int id,tmp,sec,err;};

class ThreadSafeQueue{
private:
    std::queue<Record> q;
    std::mutex m;
    std::condition_variable cv;
    bool finished=false;
public:
    void push(Record r){
        std::lock_guard<std::mutex> lk(m);
        q.push(r);
        cv.notify_one();
    }
    bool pop(Record& r){
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk,[this]{return !q.empty()||finished;});
        if(q.empty())return false;
        r=q.front();
        q.pop();
        return true;
    }
    void set_done(){
        std::lock_guard<std::mutex> lk(m);
        finished=true;
        cv.notify_all();
    }
};

void worker(ThreadSafeQueue& tsq){
    Record r;
    while(tsq.pop(r)){
        if(r.tmp>55||r.sec>50||r.err>10){
            std::cout<<"CRITICAL_ALERT: Drive "<<r.id<<" [Temp:"<<r.tmp<<"C, Sectors:"<<r.sec<<"]\n";
        }
    }
}

int main(int c,char* v[]){
    if(c<2)return 1;
    std::ifstream f(v[1]);
    if(!f.is_open())return 1;
    std::string l;
    std::getline(f,l);
    ThreadSafeQueue tsq;
    std::thread thr(worker,std::ref(tsq));
    while(std::getline(f,l)){
        std::stringstream ss(l);
        std::string val;
        std::vector<std::string> r_vec;
        while(std::getline(ss,val,','))r_vec.push_back(val);
        if(r_vec.size()>=4){
            Record rec;
            rec.id=std::stoi(r_vec[0]);
            rec.tmp=std::stoi(r_vec[1]);
            rec.sec=std::stoi(r_vec[2]);
            rec.err=std::stoi(r_vec[3]);
            tsq.push(rec);
        }
    }
    tsq.set_done();
    thr.join();
    f.close();
    return 0;
}