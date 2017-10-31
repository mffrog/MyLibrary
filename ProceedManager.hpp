//
//  ProceedManager.hpp
//  C++practice
//
//  Created by Tomoya Fujii on 2017/10/31.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#ifndef ProceedManager_hpp
#define ProceedManager_hpp

#define SAFE_DELETE(p) if(p){delete p ; p = nullptr;}

#include <vector>
#include <functional>
#include <map>

/**
 -- How to use --
 1.Make ProceedManager by PMManager::createProceedManager(). (PMManager is singleton class so you can get instance by PMManager::getInstance())
 2.Make Process by made ProceedManager's method ProceedManager::createProcess(). ( if you need any value use proceedManager::createProcessWithValue<Ty>())
 3.assignment updateFunc and judgeFunc to made Process.
 4.if you made all process just use ProceedManager::update() !
 */


using namespace std;

class Process{
public:
    virtual ~Process() = default;
    function<void()> updateFunc;
    function<bool()> judgeFunc;
};

template <typename ty>
class ProcessWithValue : public Process {
public:
    ty value;
};


class PMManager;
class ProceedManager{
    friend PMManager;
private:
    //コンストラクタ
    ProceedManager();
    ProceedManager(const ProceedManager&) = delete;
    ProceedManager& operator=(const ProceedManager&) = delete;
    
    vector<Process*> processes;
    int currentProcess = 0;
    
    int ID = 0;
    static int m_idCounter;
    
public:
    int getId(){return ID;}
    
    void update();
    Process* createProcess(){
        auto p = new Process();
        processes.push_back(p);
        return p;
    }
    template<typename ty>
    ProcessWithValue<ty>* createProcessWithValue(){
        auto p = new ProcessWithValue<ty>();
        processes.push_back(p);
        return p;
    }
    
    bool getEndFlag(){
        return currentProcess < 0;
    }
    
    ~ProceedManager();
};


class PMManager{
public:
    static PMManager* getInstance();
    //進行管理クラスの作成
    ProceedManager* createProceedManager();
    void deleteProceedManager(int ID);
private:
    PMManager() = default;
    ~PMManager();
    PMManager(const PMManager&) = delete;
    PMManager& operator=(const PMManager&) = delete;
    
    //進行管理クラスの保持
    map<int, ProceedManager*> proceedManagers;
};



#endif /* ProceedManager_hpp */
