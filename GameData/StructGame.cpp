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
        __asm
        {
            mov eax, monsterPtr
            push eax //怪物的地址
            mov ecx, selfPtr //自己的地址
            mov eax, 0x00769c50
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
        /*
        a = imm.readLong(0x96ea30)
        a = imm.readLong(a + 0x10)
        a = imm.readLong(a + 0x34)
        a = imm.readLong(a + 0x38c)
        a = imm.readLong(a + 0xe14)
        a = imm.readLong(a + 0x78)
        a = imm.readLong(a)
        a = imm.readLong(a + 0x8)*/
        /*
        !hook_war a 57fc5f esi, [esi + 0x48], [[esi + 0x48]+ 0x3c], [[[esi + 0x48]+ 0x3c] ], [[[[esi + 0x48]+ 0x3c] ] + 0x14] eax + 14
            !hook_war a 73fae7[0x96ea30], [[0x96ea30]+ 0x10], [[[0x96ea30]+ 0x10] + 0x34], [[[[0x96ea30]+ 0x10] + 0x34] + 0x38c], [[[[[0x96ea30]+ 0x10] + 0x34] + 0x38c] + 0xe14]*/
        DWORD tmp_ptr = *(DWORD*)(BASE_MONSTER_LIST_ROOT);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x10);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x34);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x38c);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0xe14);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x48);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x3c);
        tmp_ptr = *(DWORD*)(tmp_ptr + 0x0);
        if (tmp_ptr == 0)
        {
            log_debug("search but tmp_ptr:0\n");
            return;
        }

        tmp_ptr = *(DWORD*)(tmp_ptr + 0x14);
        
        DWORD selfPtr = this->basePtr;
        log_debug("will searchDead %p\n", selfPtr);
        __asm
        {
            mov eax, tmp_ptr
            push eax //怪物的地址
            mov ecx, selfPtr //自己的地址
            mov eax, 0x0073F0C0
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
        DWORD a = 0x96FC10;
        log_debug("will pick start:%x\n", a);
        a = *(DWORD*)(a + 0xc);
        a = *(DWORD*)(a + 0x0);
        a = *(DWORD*)(a + 0x74);
        a = *(DWORD*)(a + 0x40);
        a = a + 0x56ac;
        a = *(DWORD*)(a + 0x1884);
        log_debug("will pick:%x\n", a);
        __asm
        {
            mov ecx, 0x64 
            push ecx
            mov ecx, a
            push ecx
            mov eax, 0x782430
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