# hack_warspear

## 0x01:必要工具

### 1.immunity debugger

  之所以选择 immunity 而不用od是因为这个调试器支持python，实现用python来简化一些重复性工作，比如在断点时候打印一些寄存器信息，内存信息等等，虽然od应该也可以做，不过对我这种不太习惯的人来说学习成本有点高。

### 2.cheat engine

  郁金香老师的外挂视频教程中也经常使用的一个查基址，甚至调试的软件。而且我发现ce也支持脚本语言，你是可以通过编写lua脚本来实现很多功能，比如查看堆栈等等

## 0x02:目录结构

GameData  
InjectDll  
UpdateBase  
hackWarspear  
immunity：该目录存放为immunity debugger写的一些python脚本  
info：该目录存放一些lua脚本及其他一些调试中间的信息记录  
上面前面4个目录是当时写的外挂软件目录,不过本文不涉及

## 0x03:ce + lua

先给大家分享下ce的lua api网址,里面有ce提供的各种lua接口  
<https://wiki.cheatengine.org/index.php?title=Lua>  

ce通常的使用方法相信应该很多朋友是知道的，比如我想知道怪物的血量信息：  
1.找到怪物血量信息在内存中的地址  
2.再attach到游戏进程中查看是哪里改写了这个地址  
3.然后根据堆栈找到基址  
但是我不知道ce当前代码对应的堆栈，所以就需要用到调试器附加到进程加上断点之后再重复上述操作。  

不过有了ce+lua后，我们就能通过编写lua脚本来直接从ce里获取堆栈，然后就可以直接根据堆栈用ida或其他查看静态汇编的软件来分析了。

```lua
function print_stack(ebp, deep)
    --deep:堆栈深度
    --ebp:栈低寄存器
    if deep == 0 
    then
        return ""
    end
    local ebp_4 = readInteger(ebp + 4) --ebp + 4 就是调用过来时候的eip寄存器存的地址,根据这个就能倒推堆栈
    local str_ret = string.format("%x->", ebp_4)
    local next_ebp = readInteger(ebp) -- 然后当前ebp里面存的就是上一个栈低的地址,也就是上一个ebp里面存的地址
    str_ret = str_ret .. print_stack(next_ebp, deep - 1) --剩下的就是递归调用了,最终会把堆栈保存到str_ret里面
    return str_ret
end
```

上面代码中用到了几个ce提供的lua接口,比如readInteger(addr)就是从传入的addr对应的地址中读出内存中四字节数,其他都是lua语法了.

当然少不了加断点,以及断点自动执行lua脚本的方法了,毕竟ce可是具备了调试器的.

```lua
function debugger_onBreakpoint() --通用断点回调,每个断点都会进到这里
         local ebp_4 = readInteger(EBP + 4)
         if EIP == 0x77E114 -- 根据当前eip地址来做函数分发
         then
             debugger_0077E114()
         else
             debugger_784a21()
         end
  return 1
end

function clear_debug() 
  local tbl = debug_getBreakpointList() --获取当前加上的所有断点列表
  if tbl == nil
  then
      return
  end
  for i,v in ipairs(tbl) do
      print(string.format("%4d 0x%X",i,v))
      debug_removeBreakpoint(v) --根据地址删除断点
  end
end
clear_debug()
debug_setBreakpoint(0x77E114) --设置断点到地址0x77E114
```

如上代码展示了ce的断点相关操作,其实debug_setBreakpoint,我看官方文档最近版本应该是可以指定函数做入参了,之前我写这个脚本时候貌似还没有.

这里只是展示了ce+lua脚本的一小部分功能,还有很多很方便的用法,有兴趣的同学可以试一试.毕竟人生苦短,我用(python or lua or ...),能提高一点效率是一点.

## 0x04: immunity debugger

相信immunity 的python调试功能很多同学都用过,我在分析这款游戏时候,首先也是用的immunity,不过后来发现ce能支持lua后大部分工作就放在ce上了.两个各有优缺点吧.  
immunity:  
优点: 调试功能还是比较全的,毕竟是名字里带着debugger的调试器  
缺点: 断点之后+python着实优点肿了,很多时候他的断点函数执行会导致进程变的极度缓慢.但是如果你用ce+lua就完全不会,一秒钟进入几十次断点游戏运行速度几乎不受影响  
ce+lua:  
优点: 如上所说,断点调试非常顺滑,甚至调用频率比较高的接口都敢放心加断点.  
缺点: lua跟python比提供的功能还是太简单了.而且这个lua的import貌似有问题,import之后的函数还是找不到,所以基本上代码只能写到一个文件中,一些自己封装的接口也没法通用.  

其实大部分功能还是相似的,比如常用的断点执行某个函数等功能,主要区别是因为python的库比较多,可以实现一些自己的小工具等.  

比如根目录下immunity/rec_down.py这个文件是我实现的一个方便查看内存的脚本.不过我也是在网上找了一个现成的别人对递归下降算法的实现,然后改了一下,让他能支持对简单的语法解析,里面内容有点长,这里就不展示了,原理是利用递归下降算法来做语法解析,有兴趣的同学可以了解下.  
用法就是在immunity的命令行窗口下  
!rec_down [eax]: 这个命令可以用来查看eax寄存器对应地址的内容  
!rec_down [ebx + [eax + 4]] 这个命令是先读取eax+4地址对应内容再 加上ebx的值,得到的地址,再从中获取值  
!rec_down [ecx+[ebx + [eax+4]]]  哈哈,这个主要就是可以无限嵌套.  
immunity 的脚本执行需要通过在命令窗口执行 "!脚本名 脚本参数1 脚本参数2" 以这种方式执行(不带双引号)

在有了这个小工具后,你就可以把他当做一个库来用在其他脚本里了,比如

```python
from immlib import *
import rec_down # 引入刚才咱们介绍的小工具
import utils


class HookStack(LogBpHook):
    def __init__(self, desp):
        LogBpHook.__init__(self)
        self.desp = desp

    #########################################################################
    def run(self, regs):
        imm = Debugger()
        #ret = rec_down.parse_exprs(imm, regs, '[esp],[esp+8],[esp+12],[esp+16]') 这是之前注释的一个,
        data_len = rec_down.parse_expr(imm, regs, '[esp+12]') # 这个就是获取 esp+12 再取地址内容存入data_len
        if data_len != 6:
            data_addr = rec_down.parse_expr(imm, regs, '[esp+8]') 
            ret = imm.readMemory(data_addr, data_len) #从地址data_addr 处读取 data_len长度的数据到ret中
            ret = list(map(lambda x:ord(x), ret))
            imm.log("data_addr:{:x}, len:{}, ret:{}".format(data_addr, data_len, ret))

        if data_len == 30:
            stacks = imm.callStack() # 如果数据长度等于30的话,就获取堆栈并把堆栈输出到imm的log窗口中
            for stack in stacks:
                imm.log(str(stack))
        # stack_set = imm.getKnowledge('stack')
        # if not stack_set:
        #     stack_set = set()

        # stack_set.add(rec_down.parse_expr(imm, regs, '[esp+4]'))
        # imm.addKnowledge('stack', stack_set)

    @staticmethod
    def cat_stack_set():
        imm = Debugger()
        ret = imm.getKnowledge('stack') # getKnowledge是获取用户自定义的之前存入stack中的值,相当于一个全局变量
        imm.log(str(ret))

def main(args): # immunity的可执行脚本都要有main函数,然后参数从这里传入
    imm = Debugger()

    utils.clear_hooks(imm)
    opr = args[0]
    if opr == 'hook': 
        # ---------------------------- send start ------------------------------------
        addr = '76C76C19' # bp send
        #addr = '008563D1' # 8418a6
        #addr = '841871'
        # ---------------------------- send end --------------------------------------
        #addr = '5a926a' # HookClick
        #addr = '5a9210' # click header
        h = HookStack(addr) # 这个钩子是我们刚刚定义的,现在把它实例化
        ret = h.add(h.desp,  int(addr, 16)) # 这个就是加断点了,当断下来的时候就会回调到我们的钩子函数里面
        if ret == -1:
            imm.log("hook send failed!")
    elif opr == 'cat':
        HookStack.cat_stack_set()
    elif opr == 'clear':
        utils.clear_hooks(imm)

    return "ok"
```

上面展示的就是加断点并且附带一个钩子函数,当断点断下来时候会执行成员函数run里面的内容.
