from immlib import *
import re
import rec_down

ADD_HOOK = 1
UN_HOOK = 2
HOOK_WITH_ARG = 3
HOOK_TEST = 4

#############################################################################
class RecHandler(object):
    def __init__(self, imm, regs):
        self.regs = regs
        self.imm = imm

    def handle_reg(self, val):
        val_up = val.upper()
        if val_up in self.regs:
            return int(self.regs[val_up])

        return 0

    def handle_addr(self, val):
        ret = self.imm.readLong(val)
        return ret


class HookWar(LogBpHook):
    def __init__(self, desp, print_str):
        LogBpHook.__init__(self)
        self.description = desp
        self.print_str = print_str

    #########################################################################
    def run(self,regs):
        '''

        '''
        imm = Debugger()

        # high_p = self.print_str.upper()
        # if high_p in regs:
        #     imm.log('{}:{}'.format(high_p, regs[high_p]))
        # else:
        #     imm.log("{}:{}".format(self.print_str, str(regs)))


class InstallHook(object):
    def __init__(self, imm):
        self.imm = imm
        self.addr_list = []
        self.addr_pre = [
            # '0084E1B0',
            # '0083A600',
            # '0071D2A0',
            # '005A9890',
            # '007BC2A0',
            # '00781C60',
            '005A9210',
            '0073FA90',
        ]

    def add_hook(self, addr):
        self.imm.log(addr)
        self.addr_list.append(addr)
        print_str = '->'.join(self.addr_list)
        int_addr = int(addr, 16)

        hooker = HookWar(addr, print_str)
        ret = hooker.add(hooker.description,  int_addr)
        if ret == -1:
            self.imm.log("Hooking add failed:{}!".format(addr))

    def add_pre_set(self):
        for index, addr in enumerate(self.addr_pre):
            int_addr = int(addr, 16)
            self.imm.log(addr)
            print_str = '->'.join(self.addr_pre[:index + 1])

            hooker = HookWar(addr, print_str)
            ret = hooker.add(hooker.description, int_addr)
            if ret == -1:
                self.imm.log("Hooking add failed:{}!".format(addr))

    def get_hook_info(self):
        pat = re.compile(r'warspear\.(\w+)\s+warspear\.(\w+)')
        with open(r'E:\shgithub\cpp\hackWarspear\hackWarspear\info\dead.txt') as fr:
            for line in fr:
                find = pat.search(line)
                if find:
                    self.add_hook(find.groups()[0])


def main(args):
    imm = Debugger()
    imm.log(str(args))
    if len(args) < 1:
        return 'hook war failed'

    arg = int(args[0])
    if arg == ADD_HOOK:
        ih = InstallHook(imm)
        ih.add_pre_set()

        # hooker = HookWar()
        # hooker.description = 'hello'
        # ret = hooker.add(hooker.description,  0x4B2576)
        # if ret == -1:
        #     imm.log("Hooking add failed!")
        # else:
        #     imm.addKnowledge("ckz_hook", hooker.description)
    elif arg == UN_HOOK:
        desp = imm.getKnowledge("ckz_hook")
        if desp:
            imm.log(desp)

            imm.removeHook(desp)
                #now forget about that hook
            imm.forgetKnowledge("ckz_hook")
    elif arg == HOOK_WITH_ARG:
        if len(args) != 3:
            return 'hook war args wrong'

        addr = args[1]
        print_str = args[2]
        h = HookWar(addr, print_str)
        ret = h.add(h.description,  int(addr, 16))
        if ret == -1:
            imm.log("hook with arg failed!")
    elif arg == HOOK_TEST:
        if len(args) != 3:
            return 'hook war test args wrong'

        print_str = args[2]
        imm.log(str(imm.getRegs()))

        rh = RecHandler(imm, imm.getRegs())
        e = rec_down.ExpressionEvaluator(rh)
        ret = e.parse(print_str)
        imm.log('ret:{}'.format(ret))

    return 'hello'