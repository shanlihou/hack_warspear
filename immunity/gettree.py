#!/usr/bin/python
# coding:utf-8
import immlib


class Item(object):
    def __init__(self, hp, ptr, entity_type):
        self.hp = hp
        self.ptr = ptr
        self.entity_type = entity_type

    def read_long(self, imm, offset):
        return imm.readLong(self.ptr + offset)

    def get_target(self, imm):
        return imm.readLong(self.ptr + 0x268)

    def print_iter(self, imm):
        yield '{:>10d}'.format(self.hp)
        i = 0
        while 1:
            val = imm.readMemory(self.ptr + i, 1)

            yield '{:>10x}'.format(ord(val))
            i += 1

    @staticmethod
    def print_tag():
        yield ' ' * 8 + 'hp'
        i = 0
        while 1:
            yield '{:>10d}'.format(i)
            i += 1


class ItemsInfo(object):
    def __init__(self):
        self.items = {}
        self.hp_set = set()

    def get_avatar(self):
        for item_list in self.items.values():
            for item in item_list:
                if item.entity_type == 3:
                    return item
        
        return None

    def add_obj(self, hp, ptr, entity_type):
        self.items.setdefault(hp, [])
        self.items[hp].append(Item(hp, ptr, entity_type))
        self.hp_set.add(hp)

    def parse_one_diff(self, imm, hp):
        item_list = self.items[hp]
        iter_list = [Item.print_tag()]
        iter_list.extend(
            map(lambda x: x.print_iter(imm), item_list))

        for i in range(400):
            vals = list(map(next, iter_list))
            vals_str = ''.join(vals)

            val_dic = {}
            for val in vals[1:]:
                val_dic[val] = val_dic.get(val, 0) + 1

            if len(val_dic) == 2 and 1 in val_dic.values():
                imm.log(vals_str)

    def cmp_mon(self, imm):
        iter_list = []
        iter_list.append(Item.print_tag())
        index = 1
        index_203 = -1
        for hp in self.hp_set:
            item_list = self.items[hp]
            iter_list.append(item_list[0].print_iter(imm))

            if hp == 203:
                iter_list.append(item_list[1].print_iter(imm))
                index_203 = index

            index += 1

        def log_gen():
            while 1:
                vals = list(map(next, iter_list))
                vals_str = ''.join(vals)
                ret = (vals[index_203] == vals[index_203 + 1] and vals[index_203] != '{:>10d}'.format(0))
                yield ret, vals_str
        
        lg = log_gen()
        ret, str_print = next(lg)
        imm.log(str_print)
        for i in range(100):
            rets = []
            strs = []
            for i in range(4):
                ret, str_print = next(lg)
                rets.append(ret)
                strs.append(str_print)

            imm.log(str(rets))
            if any(rets):
                for str_print in strs:
                    imm.log(str_print)


def read_tree(imm, node, func=None):
    if node == 0:
        return

    left = imm.readLong(node + 4)
    right = imm.readLong(node + 8)
    value = imm.readLong(node + 16)
    ptr = imm.readLong(node + 20)
    hp = imm.readLong(ptr + 244)
    entity_type = imm.readLong(ptr + 0x4)
    
    read_tree(imm, left, func)
    read_tree(imm, right, func)
    imm.log('{:#x},{}, {}'.format(ptr, hp, entity_type))
    if func:
        func(hp, ptr, entity_type)


def read_mons_info():
    imm = immlib.Debugger()
    a1_addr = 0x96EA30
    a2_addr = imm.readLong(a1_addr)
    a3_addr = imm.readLong(a2_addr + 16)
    a4_addr = imm.readLong(a3_addr + 48)
    root = imm.readLong(a4_addr)

    ii = ItemsInfo()

    read_tree(imm, root, ii.add_obj)
    avatar = ii.get_avatar()
    imm.log('avatar target:{:x}'.format(avatar.get_target(imm)))
    ii.parse_one_diff(imm, 203)


def attach():
    imm = immlib.Debugger()
    imm.Attach(20624)

def get_dead_body():
    imm = immlib.Debugger()
    # MOV EAX,DWORD PTR DS:[96EA30]
    # MOV ECX,DWORD PTR DS:[EAX+10]
    # MOV ECX,DWORD PTR DS:[ECX+34]
    a = imm.readLong(0x96ea30)
    a = imm.readLong(a + 0x10)
    a = imm.readLong(a + 0x34)
    a = imm.readLong(a + 0x38c)
    a = imm.readLong(a + 0xe14)
    a = imm.readLong(a + 0x78)
    a = imm.readLong(a)
    
    a = imm.readLong(a + 0x8)
    imm.log('ecx is:{:x}'.format(a))

def get_switch_choice():
    imm = immlib.Debugger()
    #ecx = imm.getRegs()['ESI']
    #imm.log('ecx:{:x}'.format(ecx))
    EAX = imm.readLong(0x96EA30)
    imm.log('EAX:{:x}'.format(EAX))
    ECX = imm.readLong(EAX + 0x10)
    imm.log('ECX:{:x}'.format(ECX))
    ECX = imm.readLong(ECX + 0x34)
    imm.log('ECX:{:x}'.format(ECX))
    # 0073FAFD  |. 8B86 8C030000  MOV EAX,DWORD PTR DS:[ESI+38C]
    # 0073FB03  |. 8B80 140E0000  MOV EAX,DWORD PTR DS:[EAX+E14]
    # 0073FB09  |. 8B40 78        MOV EAX,DWORD PTR DS:[EAX+78]
    # 0073FB0C  |. 8B08           MOV ECX,DWORD PTR DS:[EAX]
    EAX = imm.readLong(ECX + 0x38c)
    imm.log('EAX:{:x}'.format(EAX))
    EAX = imm.readLong(EAX + 0xe14)
    imm.log('EAX:{:x}'.format(EAX))
    EAX = imm.readLong(EAX + 0x78)
    imm.log('EAX:{:x}'.format(EAX))
    ECX = imm.readLong(EAX + 0x0)
    imm.log('ECX:{:x}'.format(ECX))

    # 0073FB26  |> 8B51 0C        MOV EDX,DWORD PTR DS:[ECX+C]
    EDX = imm.readLong(ECX + 0xc)
    imm.log('EDX:{:x}'.format(EDX))
    a = imm.readLong(EDX + 0xc)
    imm.log('choice:{:x}'.format(a))

def main(args):
    get_switch_choice()
    return "[*] PyCommand Executed!"
