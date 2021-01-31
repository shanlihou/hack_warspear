function print_stack(ebp, deep)
    if deep == 0
    then
        return ""
    end
    local ebp_4 = readInteger(ebp + 4)
    local str_ret = string.format("%x->", ebp_4)
    local next_ebp = readInteger(ebp)
    str_ret = str_ret .. print_stack(next_ebp, deep - 1)
    return str_ret
end

function clear_debug()
    local tbl = debug_getBreakpointList()
    if tbl == nil
    then
        return
    end
    for i,v in ipairs(tbl) do
        print(string.format("%4d 0x%X",i,v))
        debug_removeBreakpoint(v)
    end
end

function debugger_onBreakpoint()
    if EIP == 0x5c7ac6
    then
        local arg2 = readInteger(0x19FCEC)
        print(string.format("enter in 0x5c7ac6, arg2:0x%X", arg2))
    else
        local arg1 = readInteger(EAX)
        local arg2 = readInteger(EAX + 0x4)
        print(string.format("arg1:0x%X arg2:0x%X, eax:0x%X", arg1, arg2, EAX))
    end
    return 1
end

clear_debug()
print(1)
--debug_setBreakpoint(0x5c7ac6)
--debug_setBreakpoint(0x8602c4)