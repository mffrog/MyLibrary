//
//  main.cpp
//  prac
//
//  Created by Tomoya Fujii on 2016/10/15.
//  Copyright © 2016年 TomoyaFujii. All rights reserved.
//

#include <iostream>
#include <vector>
#include <memory>


#define SAFE_DELETE(p) if(p){ delete p; p = nullptr;}

template<typename Ty>
class active_root_ptr;

template<typename Ty>
class active_ptr{
public:
    friend active_root_ptr<Ty>;
    bool IsActive(){return isActive;}
    Ty* Get(){return isActive ? pointer : nullptr;}
    
private:
    active_ptr() = default;
    ~active_ptr(){
        std::cout << "active link destructor called" << std::endl;
    }
    Ty* pointer = nullptr;
    bool isActive = false;
};

template<typename Ty>
class active_root_ptr{
public:
    using  ActLinkPtr = std::shared_ptr<active_ptr<Ty>>;
    
    active_root_ptr(Ty* target):target(target){}
    
    ActLinkPtr GiveLink(){
        struct Impl : active_ptr<Ty> {};
        ActLinkPtr link = std::make_shared<Impl>();
        link->isActive = true;
        link->pointer = target;
        linkList.push_back(link);
        return link;
    }
    
    void Destroy(){
        for(auto& link : linkList){
            link->isActive = false;
            link->pointer = nullptr;
        }
        SAFE_DELETE(target);
    }
    
    ~active_root_ptr(){
        Destroy();
    }
    
    Ty* operator=(Ty* arg){target = arg;}
private:
    std::vector<ActLinkPtr> linkList;
    Ty* target;
};

class Parent{
public:
    void Show(){
        std::cout << "Parent Show" << std::endl;
    }
};

using LinkPtr = std::shared_ptr<active_ptr<Parent>>;

class Child{
public:
    Child() = default;
    Child(LinkPtr link) : linkPtr(link){}
    void Show(){
        std::cout << "Child Show" << std::endl;
        if(!linkPtr){
            std::cout << "Link is not set" << std::endl;
            std::cout << std::endl;
            return;
        }
        if(linkPtr->IsActive()){
            linkPtr->Get()->Show();
        }
        else {
            std::cout << "Link is dead" << std::endl;
        }
        std::cout << std::endl;
    }
    LinkPtr linkPtr;
};

int main(){
    active_root_ptr<Parent> parent = new Parent();
    Child* child1 = new Child(parent.GiveLink());
    Child* child2 = new Child();
    Child* child3 = new Child(parent.GiveLink());
    
    child1->Show();
    delete child1;
    
    parent.Destroy();
    child2->Show();
    child3->Show();
    delete child2;
    delete child3;
    return 0 ;
}

