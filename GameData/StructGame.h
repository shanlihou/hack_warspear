#pragma once
#include <Windows.h>
#include <map>
#include <memory>
HWND getGameWndHandle();

class EntityBase
{
public:
    EntityBase()
    {}

    EntityBase(DWORD ptr, DWORD hp, DWORD type)
        :basePtr(ptr),
        HP(hp),
        type(type)
    {
    };

    virtual void attack(DWORD ptr);
    virtual void searchDead();

public:
    DWORD basePtr;
    DWORD type;
    DWORD HP;

    typedef std::shared_ptr<EntityBase> EntityBasePtr;
};

class MonsterInfo: EntityBase
{
public:
    MonsterInfo(DWORD ptr, DWORD hp) :EntityBase(ptr, hp, 1)
    {};
};


class EntityMgr
{
private:
    std::map<DWORD, EntityBase::EntityBasePtr> entities;
    void readTree(DWORD);
    EntityBase::EntityBasePtr getEntityByHp(DWORD hp);
    EntityBase::EntityBasePtr getEntityByType(DWORD hp);

public:
    void getData();
    void test();
};