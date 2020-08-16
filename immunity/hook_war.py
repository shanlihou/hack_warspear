from immlib import *
import re
import rec_down

ADD_HOOK = 1
UN_HOOK = 2
HOOK_WITH_ARG = 'a'
HOOK_TEST = 4
HOOK_SELF = 'self'
HOOK_CLEAR = 'c'
HOOK_DEL_ONE = 'o'
HOOK_SEARCH = 's'

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
        if val == 0:
            return 0

        ret = self.imm.readLong(val)
        return ret

def parse_expr(imm, regs, expr):
    rh = RecHandler(imm, regs)
    ee = rec_down.ExpressionEvaluator(rh)
    return ee.parse(expr)


class HookWar(LogBpHook):
    def __init__(self, desp, exprs, is_common=True, print_str='no print'):
        LogBpHook.__init__(self)
        self.description = desp
        self.exprs = exprs
        self.is_common = is_common
        self.print_str = print_str

    #########################################################################
    def run(self, regs):
        '''

        '''
        imm = Debugger()
        if not self.is_common:
            ret = parse_expr(imm, regs, '[esi+6]')
            high = (ret & 0xffff0000) >> 16
            low = (ret & 0xffff)
            imm.log("high:{}, low:{}".format(high, low))
            return

        if self.exprs:
            exprs = self.exprs.split(',')
            rh = RecHandler(imm, regs)
            ee = rec_down.ExpressionEvaluator(rh)
            rets = map(lambda x: '{}:{:x}'.format(x, ee.parse(x)), exprs)
            imm.log('desp:{} :{}'.format(self.print_str, ','.join(rets)))
        else:
            imm.log('{}:{}'.format('regs:', str(regs)))

        # high_p = self.exprs.upper()
        # if high_p in regs:
        #     imm.log('{}:{}'.format(high_p, regs[high_p]))
        # else:
        #     imm.log("{}:{}".format(self.exprs, str(regs)))

#esi,[esi+0x48],[[esi+0x48]+0x3c],[[[esi+0x48]+0x3c]],[[[[esi+0x48]+0x3c]]+0x14]
class Hook_3c(HookWar):
    def run(self, regs):
        imm = Debugger()
        esi = parse_expr(imm, regs, '[[[[[0x96ea30]+0x10]+0x34]+0x38c]+0xe14]')
        imm.log('esi:{:x}'.format(esi))
        if not esi:
            return

        addr = parse_expr(imm, regs, '[[[%d+0x48]+0x3c]]' % esi)
        imm.log('my:{:x}'.format(addr))
        if addr:
            addr = parse_expr(imm, regs, '[[[[esi+0x48]+0x3c]]+0x14]')
            imm.log('my2:{:x}'.format(addr))

class HookEsi78(HookWar):
    def run(self, regs):
        imm = Debugger()
        addr = parse_expr(imm, regs, '[[esi+0x78]]+0xc')
        imm.log("will hook:{:x}".format(addr))
        imm.setMemBreakpoint(addr, 'W')


class HookBag(HookWar):
    def run(self, regs):
        imm = Debugger()
        calc_eax = parse_expr(imm, regs, '[[[0x96ea30]+0x10]+0x10c8+0x30]')
        calc_edi = parse_expr(imm, regs, '[ebp-4]')
        calc_val = calc_eax + calc_edi * 4
        calc_ecx = parse_expr(imm, regs, '[%d]' % calc_val)
        ecx = parse_expr(imm, regs, 'ecx')
        imm.log('calc_ecx:{:x}, calc_edi:{}, ecx:{:x}'.format(calc_ecx, calc_edi, ecx))


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


def rem_hook(imm, addr):
    hooks = imm.listHooks()
    if addr in hooks:
        imm.log('rem_hook:{}'.format(addr))
        imm.removeHook(addr)
        imm.deleteBreakpoint(int(addr, 16))

def main(args):
    imm = Debugger()
    imm.log(str(args))
    if len(args) < 1:
        return 'hook war failed'

    arg = args[0]
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
        if len(args) < 3:
            return 'hook war args wrong'

        addr = args[1]
        rem_hook(imm, addr)

        expr_str = args[2]
        if len(args) == 4:
            print_str = args[3]
        else:
            print_str = 'no print'
        h = HookWar(addr, expr_str, print_str=print_str)
        ret = h.add(h.description,  int(addr, 16))
        if ret == -1:
            imm.log("hook with arg failed!")
    elif arg == HOOK_TEST:
        if len(args) != 2:
            return 'hook war test args wrong'

        addr = args[1]
        h = HookWar(addr, '', False)
        ret = h.add(h.description,  int(addr, 16))
        if ret == -1:
            imm.log("hook with arg failed!")
    elif arg == HOOK_SELF:
        #addr = '57fc5f'
        addr = '5410c0'
        rem_hook(imm, addr)

        h = HookBag(addr, '')
        ret = h.add(h.description,  int(addr, 16))
        if ret == -1:
            imm.log("hook with arg failed!")
    elif arg == HOOK_CLEAR:
        imm.log(str(imm.listHooks()))
        for hook_desp in imm.listHooks():
            imm.log("will rem:{}".format(hook_desp))
            imm.removeHook(hook_desp)
            imm.deleteBreakpoint(int(hook_desp, 16))
    elif arg == HOOK_DEL_ONE:
        addr = args[1]
        rem_hook(imm, addr)
    elif arg == HOOK_SEARCH:
        addr = args[1]
        ret = imm.searchLong(int(addr, 16))
        imm.log(str(ret))



    return 'hello'