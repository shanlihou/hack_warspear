#!/usr/bin/python
# coding:utf-8
import immlib


class Item(object):
    def __init__(self, hp, ptr):
        self.hp = hp
        self.ptr = ptr

    def read_long(self, imm, offset):
        return imm.readLong(self.ptr + offset)

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

    def add_obj(self, hp, ptr):
        self.items.setdefault(hp, [])
        self.items[hp].append(Item(hp, ptr))
        self.hp_set.add(hp)

    def parse_one_diff(self, imm, hp):
        item_list = self.items[hp]
        iter_list = [Item.print_tag()]
        iter_list.extend(
            map(lambda x: x.print_iter(imm), item_list))

        for i in range(400):
            vals = map(next, iter_list)
            vals_str = ''.join(vals)

            val_dic = {}
            for val in vals[1:]:
                val_dic[val] = val_dic.get(val, 0) + 1

            if len(val_dic) == 2 and 1 in val_dic.values():
                imm.log(vals_str)

    def cmp(self, imm):
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

        for i in range(400):
            vals = map(next, iter_list)
            vals_str = ''.join(vals)
            # if vals[index_203] == vals[index_203 + 1] and vals[index_203] !=
            # '{:>10d}'.format(0):
            if 1:
                imm.log(vals_str)


def read_tree(imm, node, func=None):
    if node == 0:
        return

    left = imm.readLong(node + 4)
    right = imm.readLong(node + 8)
    value = imm.readLong(node + 16)
    ptr = imm.readLong(node + 20)
    hp = imm.readLong(ptr + 244)
    read_tree(imm, left, func)
    read_tree(imm, right, func)
    imm.log('{:#x},{}'.format(ptr, hp))
    if func:
        func(hp, ptr)


def read_mons_info():
    imm = immlib.Debugger()
    a1_addr = 0x96EA30
    a2_addr = imm.readLong(a1_addr)
    a3_addr = imm.readLong(a2_addr + 16)
    a4_addr = imm.readLong(a3_addr + 48)
    root = imm.readLong(a4_addr)

    ii = ItemsInfo()

    read_tree(imm, root, ii.add_obj)

    ii.parse_one_diff(imm, 203)


def add_hook():
    imm = immlib.Debugger()
    hook_list = imm.listHooks()
    imm.log(str(hook_list))


def attach():
    imm = immlib.Debugger()
    imm.Attach(20624)


def main(args):
    add_hook()
    return "[*] PyCommand Executed!"
