#!/usr/bin/python
# coding:utf-8
import immlib


class Item(object):
    def __init__(self, hp, ptr):
        self.hp = hp
        self.ptr = ptr


class ItemsInfo(object):
    def __init__(self):
        self.items = {}
        
    def add_obj(self, hp, ptr):
        self.items.setdefault(hp, [])
        self.items[hp].append(Item(hp, ptr))
        

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
    read_tree(imm, root)
    
def attach():
    imm = immlib.Debugger()
    imm.Attach(41312)

def main(args):
    attach()
    return "[*] PyCommand Executed!"