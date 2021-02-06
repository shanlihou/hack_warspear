def rem_hook(imm, addr):
    hooks = imm.listHooks()
    if addr in hooks:
        imm.log('rem_hook:{}'.format(addr))
        imm.removeHook(addr)
        imm.deleteBreakpoint(int(addr, 16))

def clear_hooks(imm):
    hooks = imm.listHooks()
    for addr in hooks:
        rem_hook(imm, addr)
