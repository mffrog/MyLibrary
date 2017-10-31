//
//  ProceedManager.cpp
//  C++practice
//
//  Created by Tomoya Fujii on 2017/10/31.
//  Copyright © 2017年 TomoyaFujii. All rights reserved.
//

#include "ProceedManager.hpp"

int ProceedManager::m_idCounter = 0;

ProceedManager::ProceedManager():ID(ProceedManager::m_idCounter){
    ++ProceedManager::m_idCounter;
}
ProceedManager::~ProceedManager(){
    auto itr = processes.begin();
    while (itr != processes.end()){
        delete (*itr);
        ++itr;
    }
}

void ProceedManager::update(){
    if(currentProcess >= 0){
        processes[currentProcess]->updateFunc();
        if(processes[currentProcess]->judgeFunc()){
            ++currentProcess;
            
            if(currentProcess >= processes.size()){
                currentProcess = -1;
                PMManager::getInstance()->deleteProceedManager(ID);
            }
        }
    }
}


PMManager::~PMManager(){
    auto itr = proceedManagers.begin();
    while (itr!=proceedManagers.end()) {
        SAFE_DELETE(itr->second);
        ++itr;
    }
}

PMManager* PMManager::getInstance(){
    static PMManager instance;
    return &instance;
}
//進行管理クラスの作成
ProceedManager* PMManager::createProceedManager(){
    auto pPM = new ProceedManager();
    proceedManagers.insert({pPM->getId(),pPM});
    return pPM;
}

void PMManager::deleteProceedManager(int ID){
    auto itr = proceedManagers.find(ID);
    if(itr!=proceedManagers.end()){
        proceedManagers.erase(itr);
    }
}
