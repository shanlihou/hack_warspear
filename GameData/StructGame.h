#pragma once
#include <Windows.h>
#include <map>
#include <memory>

class EntityBase
{
public:
    EntityBase()
    {}

    EntityBase(DWORD ptr, DWORD hp)
        :basePtr(ptr),
        HP(hp)
    {
    };

    virtual void attack(DWORD ptr);
public:
    DWORD basePtr;
    DWORD HP;

    typedef std::shared_ptr<EntityBase> EntityBasePtr;
};

class MonsterInfo: EntityBase
{
public:
    MonsterInfo(DWORD ptr, DWORD hp) :EntityBase(ptr, hp)
    {};
};


class EntityMgr
{
private:
    std::map<DWORD, EntityBase::EntityBasePtr> entities;
    void readTree(DWORD);
    EntityBase::EntityBasePtr getEntityByHp(DWORD hp);

public:
    void getData();
    void test();
};