<?xml version="1.0" encoding="utf-8"?>
<CheatTable CheatEngineTableVersion="31">
  <CheatEntries>
    <CheatEntry>
      <ID>0</ID>
      <Description>"306"</Description>
      <LastState Value="9395672" RealAddress="0D9447A8"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0D9447A8</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>1</ID>
      <Description>"306"</Description>
      <LastState Value="9395672" RealAddress="0D9467A0"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0D9467A0</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>2</ID>
      <Description>"4294967293"</Description>
      <LastState Value="9382204" RealAddress="0DB45998"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB45998</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>3</ID>
      <Description>"1"</Description>
      <LastState Value="9395672" RealAddress="0DB45C80"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB45C80</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>4</ID>
      <Description>"4294967288"</Description>
      <LastState Value="9382204" RealAddress="0DB45F68"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB45F68</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>5</ID>
      <Description>"1"</Description>
      <LastState Value="9395672" RealAddress="0DB46B08"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB46B08</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>6</ID>
      <Description>"4294967293"</Description>
      <LastState Value="9382204" RealAddress="0DB46DF0"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB46DF0</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>7</ID>
      <Description>"306"</Description>
      <LastState Value="9395672" RealAddress="0DB473C0"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB473C0</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>8</ID>
      <Description>"306"</Description>
      <LastState Value="9395672" RealAddress="0DB47C78"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB47C78</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>9</ID>
      <Description>"1"</Description>
      <LastState Value="9395672" RealAddress="0DB47F60"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB47F60</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>10</ID>
      <Description>"306"</Description>
      <LastState Value="9395672" RealAddress="0DB48530"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB48530</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>11</ID>
      <Description>"4294967294"</Description>
      <LastState Value="9382204" RealAddress="0DB48818"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB48818</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>12</ID>
      <Description>"306"</Description>
      <LastState Value="9395672" RealAddress="0DB493B8"/>
      <VariableType>4 Bytes</VariableType>
      <Address>0DB493B8</Address>
    </CheatEntry>
  </CheatEntries>
  <UserdefinedSymbols/>
  <LuaScript>function get_hp(memRecord, tbs)
    local addr = memRecord.getCurrentAddress()
    local hpAddr = addr + 244
    local ihp = readInteger(hpAddr)
    memRecord.setDescription(ihp)
    if (ihp == 306)
    then
      _mr = tbs.createMemoryRecord()
      _mr.setAddress(string.format("%x", hpAddr))
      _mr.setDescription(memRecord.getAddress())
    end
end

local tbs = getAddressList()
local cnt = tbs.getCount()
for i = 0, cnt - 1 do
    get_hp(tbs.getMemoryRecord(i), tbs)
end
</LuaScript>
</CheatTable>
