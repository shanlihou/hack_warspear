#include "BaseGame.h"
#include "StructGame.h"
#include "debug.h"

void EntityBase::attack(DWORD monsterPtr)
{
    try
    {
        DWORD selfPtr = this->basePtr;
        log_debug("will attack %p, %p\n", selfPtr, monsterPtr);
        __asm
        {
            mov eax, monsterPtr
            push eax //怪物的地址
            mov ecx, selfPtr //自己的地址
            mov eax, 0x0073F1E0
            CALL eax
        }
    }
    catch (...)
    {
    }
}


void EntityMgr::readTree(DWORD node)
{
    if (node == NULL)
    {
        return;
    }

    DWORD left = *(DWORD*)(node + 0x4);
    DWORD right = *(DWORD*)(node + 0x8);
    DWORD value = *(DWORD*)(node + 0x10);
    DWORD ptr = *(DWORD*)(node + 0x14);
    DWORD hp = *(DWORD*)(ptr + 0xf4);

    log_debug("hp:%d, ptr:%p\n", hp, (void*)ptr);
    readTree(left);
    readTree(right);

    EntityBase::EntityBasePtr entity(new EntityBase(ptr, hp));
    this->entities.insert(std::make_pair(value, entity));
}

EntityBase::EntityBasePtr EntityMgr::getEntityByHp(DWORD hp)
{
    for (auto ptr : this->entities)
    {
        if (ptr.second->HP == hp)
        {
            return EntityBase::EntityBasePtr(ptr.second);
        }
    }

    return EntityBase::EntityBasePtr();
}

void EntityMgr::getData()
{
    try
    {
        DWORD tmpVal = *(DWORD *)BASE_MONSTER_LIST_ROOT;
        tmpVal = *(DWORD*)(tmpVal + 0x10);
        tmpVal = *(DWORD*)(tmpVal + 0x30);
        tmpVal = *(DWORD*)(tmpVal);
        this->entities.clear();
        readTree(tmpVal);
    }
    catch (...) 
    {
        log_debug("error happened!\n");
    }
}

void EntityMgr::test()
{
    try
    {
        auto avatar = this->getEntityByHp(1214);
        auto mon = this->getEntityByHp(1);
        log_debug("will attack\n");
        avatar->attack(mon->basePtr);
    }
    catch (...)
    {
        log_debug("error happened!\n");
    }
}