--local hp = require("getHp")
function print_stack()
  local value=ESP
  local esp = readInteger(value)
  local esp_4 = readInteger(value + 4)
  local esp_8 = readInteger(value + 8)
  local esp_12 = readInteger(value + 12)
  print(string.format("%x,%x,%x,%x", esp, esp_4, esp_8, esp_12))
end

function print_4020e0()
  local value = EDX
  local src = readInteger(EBP + 0xc)
  local bytesRet = readBytes(src, 1, false)
  local ebp = readInteger(EBP)
  local ebp_4 = readInteger(EBP + 4)
  if (EBX == 30)
  then
  print(type(bytesRet))
  print(string.format("ebp:%x, ebp_4:%x", ebp, ebp_4))
print(string.format("edx:%x, eax:%x, %d, %d", value, EAX, EDX - EAX, EBX))
  end

end

function print_856223()
         local esi_48 = readInteger(EDI + 0x48)
         if (esi_48 == 26)
         then
             local stack = readInteger(EBP + 4)
             print(string.format("s:%x, esi:%d, ebx:%d, esi_48:%d", stack, ESI, EBX, esi_48))
         end
end

function print_82b6b5()
         local ebp_4 = readInteger(EBP + 4)
         local ebp_8 = readInteger(EBP + 8)
         local ebp_c = readInteger(EBP + 0xc)
         if not( ebp_c == 6)
         then
         print(string.format("eax:%x, %x, %x, %x", EAX, ebp_8, ebp_c, ebp_4))
         end
end

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

function print_007794F1()
         local esp_4 = readInteger(ESP+4)
         local esp = readInteger(ESP)
         local ebp_4 = readInteger(EBP + 4)
         if not(esp_4 == 6)
         then
         local ret = print_stack(EBP, 10)
         print(ret)
         print(string.format("%x, %x, %x", esp_4, esp, ebp_4))
         end
end

function print_006C76B3()
         print(string.format("eax:%x", EAX))
end

function print_006C88FD()
         local ret = print_stack(EBP, 15)
         print(ret)
         print(string.format("ecx:%x, eax:%x", ECX, EAX))
end

function print_00781DC9()
         local call_addr = readInteger(EAX+0x44)
         print(string.format("ecx:%x, eax:%x, esi:%x, addr:%x", ECX, EAX, ESI, call_addr))
end

function print_0077A1A0()
         local ebp_4 = readInteger(ESP+4)
         local ebp = readInteger(ESP)
         print(string.format("ecx:%x, eax:%x, esi:%x, ebp_4:%x, ebp:%x", ECX, EAX, ESI, ebp_4, ebp))
end

function get_every_win(in_addr, index)
        local a = readInteger(in_addr + 0x10)
        a = readInteger(a + index * 4)
        a = a + 8
        a = readInteger(a + 0x8)
        a = readInteger(a + 4 * 5)
        print(string.format("every:%d, %x", index, a))
end

function get_window()
--mov     ecx, offset dword_96FC10
        local a = 0x974C58
--mov     ecx, [eax+0Ch]
        a = readInteger(a+0xc)
        print(a)
--mov     eax, [ecx]
          a = readInteger(a)
        print(a)
--mov     edx, [eax+74h]
        a = readInteger(a + 0x74)
        print(a)
--mov     ecx, [eax+40h]
        a = readInteger(a + 0x40)
        print(a)
--add     ecx, 56ACh
        a = a + 0x5628
        print(string.format("%x %x", a, a+0x1884))
        for i = 0, 2 do
            get_every_win(a, i)
        end
--mov     edx, [ebx+1884h]
        a = readInteger(a + 0x1884)
        print(a)
--mov     ecx, edx
--mov     ebx, ecx
--mov     ecx, ebx
        return a
end

function print_0077A214()
         local eax_18 = readInteger(EAX+0x18)
         local win = get_window()
         print(string.format("ecx:%x, esi:%x, eax_18:%x, win:%x", ECX, ESI, eax_18, win))
end

function print_76C76C19()
         local esp = readInteger(ESP)
         local esp_4 = readInteger(ESP+4)
         local esp_8 = readInteger(ESP+8)
         local esp_12 = readInteger(ESP+12)
         if esp_12 == 6
         then
         return
         end
         print(string.format("esp:%x, esp_4:%x, esp_8:%x, esp_12:%x", esp, esp_4, esp_8, esp_12))
end

function debugger_0077E114()
         local eax_18 = readInteger(EAX+0x18)
         --local st = print_stack(EBP, 15)
         --print(st)
         --local win = get_window()
         --get_window()
         print(string.format("pc:%x", EIP))
         print(string.format("eax_18:%x, esi:%x", eax_18, ESI))
end

function debugger_795226()
         ret = print_stack(EBP, 15)
         print(ret)
         print(string.format("EAX:%x", EAX))
end

function debugger_784a21()
         --mov     ecx, [ecx+esi*4]
         local ret = readInteger(ECX + ESI * 4)
         if not(ret == 0xcb9bcf0)
         then
             return
         end
         print(string.format("ret:%x, ECX:%x, ESI:%x", ret, ECX, ESI))
end

function debugger_7c08d0()
         print(1)
end


function debugger_onBreakpoint()
         local ebp_4 = readInteger(EBP + 4)
         if EIP == 0x77E114
         then
             debugger_0077E114()
         else
             debugger_784a21()
         end
  return 1
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
clear_debug()
debug_setBreakpoint(0x77E114)
--debug_setBreakpoint(0x784a21)
--debug_setBreakpoint(0x7c08d0)  -- called if the above condition is true. And it will set breakpoint at 0x0040CEA6
ret = get_window()
