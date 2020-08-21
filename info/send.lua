local hp = require("getHp")
function print_stack()
  local value=ESP
  local esp = readInteger(value)
  local esp_4 = readInteger(value + 4)
  local esp_8 = readInteger(value + 8)
  local esp_12 = readInteger(value + 12)
  print(string.format("%x,%x,%x,%x", esp, esp_4, esp_8, esp_12))
end

function print_edx()
  local value = EDX
  print(string.format("edx:%x", value))
end

function debugger_onBreakpoint()
  return 1
end

function clear_debug()
  local tbl = debug_getBreakpointList()
  for i,v in ipairs(tbl) do
      print(string.format("%4d 0x%X",i,v))
      debug_removeBreakpoint(v)
  end
end
--debug_setBreakpoint(0x4020e0)  -- called if the above condition is true. And it will set breakpoint at 0x0040CEA6
clear_debug()
--print(EAX)
