from immlib import *
import rec_down
import utils


class HookStack(LogBpHook):
    def __init__(self, desp):
        LogBpHook.__init__(self)
        self.desp = desp

    #########################################################################
    def run(self, regs):
        imm = Debugger()
        #ret = rec_down.parse_exprs(imm, regs, '[esp],[esp+8],[esp+12],[esp+16]')
        data_len = rec_down.parse_expr(imm, regs, '[esp+12]')
        if data_len != 6:
            data_addr = rec_down.parse_expr(imm, regs, '[esp+8]')
            ret = imm.readMemory(data_addr, data_len)
            ret = list(map(lambda x:ord(x), ret))
            imm.log("data_addr:{:x}, len:{}, ret:{}".format(data_addr, data_len, ret))

        if data_len == 30:
            stacks = imm.callStack()
            for stack in stacks:
                imm.log(str(stack))
        # stack_set = imm.getKnowledge('stack')
        # if not stack_set:
        #     stack_set = set()

        # stack_set.add(rec_down.parse_expr(imm, regs, '[esp+4]'))
        # imm.addKnowledge('stack', stack_set)

    @staticmethod
    def cat_stack_set():
        imm = Debugger()
        ret = imm.getKnowledge('stack')
        imm.log(str(ret))

def main(args):
    imm = Debugger()

    utils.clear_hooks(imm)
    opr = args[0]
    if opr == 'hook':
        # ---------------------------- send start ------------------------------------
        addr = '76C76C19' # bp send
        #addr = '008563D1' # 8418a6
        #addr = '841871'
        # ---------------------------- send end --------------------------------------
        #addr = '5a926a' # HookClick
        #addr = '5a9210' # click header
        h = HookStack(addr)
        ret = h.add(h.desp,  int(addr, 16))
        if ret == -1:
            imm.log("hook send failed!")
    elif opr == 'cat':
        HookStack.cat_stack_set()
    elif opr == 'clear':
        utils.clear_hooks(imm)

    return "ok"
