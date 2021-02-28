#include "BaseGame.h"
#include "StructGame.h"
#include "debug.h"
#include "const.h"

void EntityBase::attack(DWORD monsterPtr)
{
    log_debug("enter attack\n");
    try
    {
        DWORD selfPtr = this->basePtr;
        log_debug("will attack %p, %p\n", selfPtr, monsterPtr);
        DWORD attack_call = ATTACK_CALL_ADDR;
        __asm
        {
            mov eax, monsterPtr
            push eax //怪物的地址
            mov ecx, selfPtr //自己的地址
            mov eax, attack_call
            CALL eax
        }
    }
    catch (...)
    {
    }
}

void EntityBase::searchDead()
{
    try
    {

        DWORD tmp_ptr = *(DWORD*)(BASE_MONSTER_LIST_ROOT);
        log_debug("before get ptr :%p\n", tmp_ptr);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x10);
        log_debug("before get ptr :%p\n", tmp_ptr);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x3c);
        log_debug("before get ptr :%p\n", tmp_ptr);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x38c);
        log_debug("before get ptr :%p\n", tmp_ptr);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0xecc);
        DWORD tmp2 = tmp_ptr;

        //log_debug("before get ptr :%p\n", tmp_ptr);
        //tmp_ptr = *(DWORD*)(tmp_ptr + 0x78);
        //log_debug("before get ptr :%p\n", tmp_ptr);
        //tmp_ptr = *(DWORD*)(tmp_ptr + 0x0);
        //log_debug("before get ptr :%p\n", tmp_ptr);
        //tmp_ptr = *(DWORD*)(tmp_ptr + 0x8);
        //log_debug("after get tmp %p\n", tmp_ptr);


        tmp2 = *(DWORD*)(tmp2 + 0x48);
        log_debug("get ptr tmp2.01 :%p\n", tmp2);
        tmp2 = *(DWORD*)(tmp2 + 0x44);
        log_debug("get ptr tmp2.02 :%p\n", tmp2);
        tmp2 = *(DWORD*)(tmp2 + 0x0);
        log_debug("get ptr tmp2.03 :%p\n", tmp2);
        DWORD node = tmp2;
        DWORD next_node = *(DWORD*)(node + 0x4);
        log_debug("get ptr node :%p\n", node);
        while (next_node) {
            node = next_node;
            log_debug("get ptr node :%p\n", node);
            DWORD next_node = *(DWORD*)(node + 0x4);
        }
        tmp2 = *(DWORD*)(node + 0x14);
        if (tmp2 == 0)
        {
            log_debug("search but tmp_ptr:0\n");
            return;
        }
        
        DWORD selfPtr = this->basePtr;
        log_debug("will searchDead tmp2 %p %p\n", selfPtr, tmp2);
        DWORD searchDropCall = SEARCH_DEAD_DROP;
        __asm
        {
            mov eax, tmp2
            push eax //怪物的地址
            mov ecx, selfPtr //自己的地址
            mov eax, searchDropCall
            CALL eax
        }
    }
    catch (...)
    {
    }
}

void EntityBase::pick()
{}


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
    DWORD type = *(DWORD*)(ptr + 0x4);

    log_debug("hp:%d, ptr:%p\n", hp, (void*)ptr);
    readTree(left);
    readTree(right);

    EntityBase::EntityBasePtr entity(new EntityBase(ptr, hp, type));
    this->entities.insert(std::make_pair(value, entity));
}

EntityBase::EntityBasePtr EntityMgr::getEntityByHp(DWORD hp)
{
    for (auto ptr : this->entities)
    {
        if (ptr.second->HP == hp)
        {
            log_debug("find hp\n");
            return EntityBase::EntityBasePtr(ptr.second);
        }
    }

    return EntityBase::EntityBasePtr();
}

EntityBase::EntityBasePtr EntityMgr::getEntityByType(DWORD type)
{
    for (auto ptr : this->entities)
    {
        if (ptr.second->type == type)
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
        tmpVal = *(DWORD*)(tmpVal + 0x38);
        tmpVal = *(DWORD*)(tmpVal);
        this->entities.clear();
        readTree(tmpVal);
    }
    catch (...) 
    {
        log_debug("error happened!\n");
    }
}

void EntityMgr::searchDead()
{
    try
    {
        auto avatar = this->getEntityByType(EntityType::SELF);
        log_debug("avatar :%d\n", avatar->HP);
        avatar->searchDead();
    }
    catch (...)
    {
        log_debug("error happened!\n");
    }
}

void EntityMgr::attack()
{
    try
    {
        log_debug("avatar attack\n");
        auto avatar = this->getEntityByType(EntityType::SELF);
        auto mon = this->getEntityByHp(470);
        if (mon) {
            log_debug("avatar :%d\n", avatar->HP, mon->basePtr);
            avatar->attack(mon->basePtr);
        }
    }
    catch (...)
    {
        log_debug("error happened!\n");
    }
}

void EntityMgr::pick()
{
    try
    {
        /*
            --mov     ecx, [eax + 0Ch]
            a = readInteger(a + 0xc)
            --mov     eax, [ecx]
            a = readInteger(a)
            --mov     edx, [eax + 74h]
            a = readInteger(a + 0x74)
            --mov     ecx, [eax + 40h]
            a = readInteger(a + 0x40)
            --add     ecx, 56ACh
            a = a + 0x56ac
            --mov     edx, [ebx + 1884h]
            a = readInteger(a + 0x1884)*/
        DWORD a = 0x9A5798;
        log_debug("will pick start1:%x\n", a);
        a = *(DWORD*)(a + 0xc);
        log_debug("will pick start2:%x\n", a);
        a = *(DWORD*)(a + 0x0);
        log_debug("will pick start3:%x\n", a);
        a = *(DWORD*)(a + 0x7c);
        log_debug("will pick start4:%x\n", a);
        a = *(DWORD*)(a + 0x40);
        log_debug("will pick start5:%x\n", a);
        a += 0x550c;
        log_debug("will pick start6:%x\n", a);
        a = *(DWORD*)(a + 0x1A98);
        log_debug("will pick:%x\n", a);
        a = *(DWORD*)(a + 0x18);

        __asm
        {
            mov ecx, 0x0 
            push ecx
            mov ecx, a
            mov eax, 0x6f00b0
            CALL eax
        }

    }
    catch (...)
    {
        log_debug("error happened!\n");
    }
}

HWND getGameWndHandle()
{
    try
    {
        return *(HWND*)BASE_HANDLE_ADDR;
    }
    catch (...)
    {
        return NULL;
    }
}