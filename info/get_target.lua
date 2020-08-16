<?xml version="1.0" encoding="utf-8"?>
<CheatTable CheatEngineTableVersion="31">
  <CheatEntries/>
  <UserdefinedSymbols/>
  <Comments>关于表格信息:
</Comments>
  <LuaScript>local g_base = readInteger(9890352)--96EA30
local tmp_ptr = readInteger(g_base + 0x10)
tmp_ptr = readInteger(tmp_ptr + 0x34)
tmp_ptr = readInteger(tmp_ptr + 0x38c)
tmp_ptr = readInteger(tmp_ptr + 0xe14)
tmp_ptr = readInteger(tmp_ptr + 0x78)
tmp_ptr = readInteger(tmp_ptr + 0x0)
print(string.format("tmp:%x", tmp_ptr+0x8))
--!hook_war a 73fafd [esi+0x38c],[[esi+0x38c]+0xe14],[[[esi+0x38c]+0xe14]+0x78],[[[[esi+0x38c]+0xe14]+0x78]],[[[[[esi+0x38c]+0xe14]+0x78]]+0x8] print_edi

</LuaScript>
</CheatTable>
