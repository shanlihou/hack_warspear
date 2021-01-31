<?xml version="1.0" encoding="utf-8"?>
<CheatTable CheatEngineTableVersion="31">
  <CheatEntries>
    <CheatEntry>
      <ID>0</ID>
      <Description>"无描述"</Description>
      <LastState Value="12" RealAddress="0E72E678"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0E72E678</Address>
    </CheatEntry>
  </CheatEntries>
  <UserdefinedSymbols/>
  <LuaScript>mm = {}
g_k = 203
g_v = nil

function handleNode(value, ptr, hp)
    print(string.format('%x,%x,%d', value, ptr, hp))
    if (g_k == hp and g_v == nil)
    then
        g_v = ptr
    end
    mm[hp] = ptr
end

function read_tree(node, func)
    if (node == 0 or node == nil)
    then
        return
    end

    local left = readInteger(node + 4)
    local right = readInteger(node + 8)
    local value = readInteger(node + 16)
    local ptr = readInteger(node + 20)
    local hp = readInteger(ptr + 244)
    read_tree(left, func)
    read_tree(right, func)
    func(value, ptr, hp)
end

function process()
    for k in pairs(mm)
    do
        print(k)
    end

    for i = 0, 66
    do
      local str = ''
      local cmp_val = nil
      for k, v in pairs(mm)
      do
          local val = readInteger(v + i * 4)
          str = string.format('%s,%x', str, val)
          if (k == g_k)
          then
              cmp_val = val
          end
      end
      local same_v = readInteger(g_v + i * 4)
      if (same_v == cmp_val)
      then
          print(i, str)
      end
    end
end

local g_base = readInteger(0x9a45b0)
local tmp_ptr = readInteger(g_base + 0x10)
tmp_ptr = readInteger(tmp_ptr + 0x38)
local root = readInteger(tmp_ptr)
print('start')
read_tree(root, handleNode)
process()
</LuaScript>
</CheatTable>
