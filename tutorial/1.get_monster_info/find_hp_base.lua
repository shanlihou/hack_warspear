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
    if EIP == 0x570FFB
    then
        local ret = print_stack(EBP, 10)
        print(ret)
    else
        print("not found")
    end
    return 1
end

clear_debug()
debug_setBreakpoint(0x570FFB)