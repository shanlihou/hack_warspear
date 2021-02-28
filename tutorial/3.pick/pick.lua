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
    if EIP == 0x76a7a5
    then
        local tmp = 0x9a45b0
        tmp = readInteger(tmp + 0x0)
        print(string.format("tmp1 val:0x%X", tmp))
        tmp = readInteger(tmp + 0x10)
        print(string.format("tmp2 val:0x%X", tmp))
        tmp = readInteger(tmp + 0x3c)
        print(string.format("tmp3 val:0x%X", tmp))
        tmp = readInteger(tmp + 0x38c)
        print(string.format("tmp4 val:0x%X", tmp))
        tmp = readInteger(tmp + 0xecc)
        local tmp2 = tmp
        print(string.format("tmp5 val:0x%X", tmp))
        tmp = readInteger(tmp + 0x78)
        print(string.format("tmp6 val:0x%X", tmp))
        tmp = readInteger(tmp + 0x0)
        print(string.format("tmp7 val:0x%X", tmp))
        tmp = readInteger(tmp + 0x8)
        print(string.format("enter in 0x76a7a5, esi:0x%X, edi:0x%X, tmp:0x%X", ESI, EDI, tmp))

        tmp2 = readInteger(tmp2 + 0x48)
        tmp2 = readInteger(tmp2 + 0x44)
        tmp2 = readInteger(tmp2 + 0x0)
        local node = tmp2
        local next_node = readInteger(node + 4)
        while (next_node ~= 0)
        do
          node = next_node
          next_node = readInteger(node + 4)
        end
        tmp2 = readInteger(node + 0x14)
        print(string.format("the tmp 2 is:0x%X", tmp2))

    elseif EIP == 0x5c71ba
    then
        print(string.format("eax:0x%X %X", EAX, readInteger(0x9a45b0)))
    elseif EIP == 0x76a573
    then
        print(string.format("eip:0x%X, esi:0x%X", EIP, ESI))
    elseif EIP == 0x5c72ab
    then
        print(string.format("eip:0x%X, ebx:0x%X", EIP, EBX))
    elseif EIP == 0x4246d8
    then
        local ebp_8 = readInteger(EBP + 0x8)
        print(string.format("eip:0x%X, eax:0x%X, ebp_8:0x%X", EIP, EAX, ebp_8))
        local stack = print_stack(EBP, 10)
        print(stack)
    elseif EIP == 0x59748a
    then
        print(string.format("eip:0x%X, edi:0x%X, esi:0x%X", EIP, EDI, ESI))
    elseif EIP == 0x595d4c
    then
        print(string.format("eip:0x%X, edi:0x%X, esi:0x%X, ebx:0x%X, [ebx]:0x%X", EIP, EDI, ESI, EBX, readInteger(EBX)))
    elseif EIP == 0x596545
    then
        print(string.format("eip:0x%X, eax:0x%X", EIP, EAX))
    elseif EIP == 0x595c60
    then
        print(string.format("eip:0x%X, eax+0x14:0x%X", EIP, EAX + 0x14))
    elseif EIP == 0x596267
    then
        print(string.format("eip:0x%X, ecx:0x%X", EIP, ECX))
    else
        print(2)
    end
    return 1
end

clear_debug()
print(12)
debug_setBreakpoint(0x76a7a5)
--debug_setBreakpoint(0x4246d8)
debug_setBreakpoint(0x59748a)
debug_setBreakpoint(0x595d4c)
--debug_setBreakpoint(0x596545)
--debug_setBreakpoint(0x595c60)
--debug_setBreakpoint(0x596267)