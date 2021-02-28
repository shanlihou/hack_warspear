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

function print_str(src, len)
         local ret = readBytes(src, len, true)  --- 下面要调用的读缓冲区的函数
         local str_ret = ""
         for k,v in ipairs(ret) do
             str_ret = str_ret .. string.format("%x ", v)
         end
         return str_ret
end

function print_tmp()
         local tmp = 0x9A5798;
         print(string.format("tmp1 is 0x%X", tmp))
         tmp = readInteger(tmp + 0xc)
         print(string.format("tmp2 is 0x%X", tmp))
         tmp = readInteger(tmp + 0x0)
         print(string.format("tmp3 is 0x%X", tmp))
         tmp = readInteger(tmp + 0x7c)
         print(string.format("tmp4 is 0x%X", tmp))
         tmp = readInteger(tmp + 0x40)
         print(string.format("tmp5 is 0x%X", tmp))
         tmp = tmp + 0x550c;
         tmp = readInteger(tmp + 0x1a98)
         print(string.format("tmp6 is 0x%X", tmp))
         tmp = readInteger(tmp + 0x18)
         print(string.format("tmp7 is 0x%X", tmp))
end

function debugger_onBreakpoint()
         if EIP == 0x0050e35c and EDX > 4 --这个是我测得时候发现,如果是心跳包,dex一定小于等于4,所以这里过滤掉
         then
            -- 因为上面,是从dl中读取值,所以我们这里打印EDX的值
             print(string.format("eax:0x%X", EDX))
             local stack = print_stack(EBP, 10)
             print(stack) -- 打印堆栈
         elseif EIP == 0x00883c7c -- send函数调用位置
         then
             local len = EAX -- send 的缓冲区大小
             local src = readInteger(ESI + 0x44) -- send 缓冲区指针
             local ret = print_str(src, len) -- 这个函数 从缓冲区地址src中读出len个字符到ret中
             if string.find(ret, "ad", 1) == 1
             then
                 print(ret)
             end
             --print(string.format("esi+0x44:0x%X", readInteger(ESI+0x44)))
         elseif EIP == 0x006f0e20 -- 拾取函数调用点
         then
             print(g_str)
             print(string.format("eip:0x%X, esi:0x%X, eax:0x%X", EIP, ESI, EAX))
         elseif EIP == 0x007BECA6 -- 拾取函数变量转移
         then
             local tmp = readInteger(EAX + 0x18)
             print(string.format("eip:0x%X, eax:0x%X, tmp:0x%X", EIP, EAX, tmp))
         elseif EIP == 0x007AFa0d -- 追踪ecx + esi * 4
         then
             local tmp = readInteger(EAX + 0x18)
             g_str = string.format("tmp:0x%X", tmp)
             if tmp == 0x7e8b20
             then
                 print(string.format("eip:0x%X", EIP))
             end
         end

    return 1
end

clear_debug()
print(1)
g_str = "";
--debug_setBreakpoint(0x0050e35c)
--debug_setBreakpoint(0x00883c7c)
debug_setBreakpoint(0x006f0e20)
debug_setBreakpoint(0x007BECA6)
--debug_setBreakpoint(0x007AFa0d)

--print_tmp()