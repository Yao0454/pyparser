# pyparser

一个用 C++ 从零实现的玩具级 Python 子集解释器。程序读取一个 `.py` 源文件，依次经过缩进处理、词法分析、语法分析与树遍历求值，直接执行其中的代码。

本项目用于学习编译器/解释器前端的基本原理，并非完整的 Python 实现，仅支持下文列出的语言子集。

## 处理流程

```
源文件
  → 缩进处理 (mark_indents)   将缩进层级转换为显式的 INDENT/DEDENT 标记
  → 词法分析 (tokenize)       将字符流切分为 Token 序列
  → 语法分析 (parse_*)        递归下降构建抽象语法树 (AST)
  → 求值/执行 (eval / exec)   遍历 AST 直接解释执行
```

## 已实现功能

### 词法分析

- 关键字：`def`、`if`、`else`
- 运算符与符号：`+`、`-`、`<`、`>`、`=`、`:`、`(`、`)`
- 字面量：整数（NUMBER）、双引号字符串（STRING）
- 标识符（IDENT），其中 `True`、`False` 在求值阶段被识别为布尔值
- 缩进处理：以空格宽度计 1、制表符计 8 计算缩进，转换为 INDENT/DEDENT，并在内部以 `{`、`}` 表示代码块
- 每个 Token 记录所在行号，用于错误定位
- 在 Token 序列末尾追加 END 哨兵，避免越界

### 语法分析

递归下降，支持的文法（近似 EBNF，`{ }` 表示重复，`[ ]` 表示可选）：

```
program    = { statement } ;
statement  = funcdef | if_stmt | assignment | expr_stmt ;
funcdef    = "def" IDENT "(" ")" ":" block ;
if_stmt    = "if" expression ":" block [ "else" ":" block ] ;
assignment = IDENT "=" expression ;
expr_stmt  = expression ;
block      = INDENT { statement } DEDENT ;
expression = additive { ( "<" | ">" ) additive } ;
additive   = primary  { ( "+" | "-" ) primary } ;
primary    = NUMBER
           | STRING
           | IDENT
           | IDENT "(" { expression } ")"
           | "(" expression ")" ;
```

运算符优先级由文法分层保证：比较运算符（`<`、`>`）低于加减运算符（`+`、`-`），加减低于括号与原子。所有二元运算符为左结合。

AST 节点类型：`NUMBER`、`STRING`、`IDENT`、`BINOP`、`ASSIGN`、`CALL`、`BLOCK`、`IF`、`FUNCDEF`。

### 求值与执行

树遍历解释器。运行时值（Value）有三种类型：整数、字符串、布尔。

- 表达式求值（eval）：整数、字符串、变量读取、`True`/`False`、二元运算（`+`、`-` 返回整数；`<`、`>` 返回布尔）
- 语句执行（exec）：
  - 函数定义：按名字保存，定义时不执行函数体
  - 代码块：顺序执行其中语句
  - 赋值：将右值表达式求值后写入变量表
  - `if` / `else`：对条件求值，按布尔结果执行对应代码块
  - 函数调用：内置 `print`；用户自定义函数按名字查找并执行其函数体
- 变量保存在全局变量表中
- 内置函数：`print`，对各实参求值后输出，实参之间以空格分隔，末尾换行

## 构建

### 依赖

- 支持 C++23 的编译器，且提供 `<print>` 头文件（例如 Clang 17 及以上配合 libc++，或 GCC 14 及以上）
- CMake 3.16 或更高版本

### 步骤

```sh
cmake -S . -B build
cmake --build build
```

可执行文件生成于 `build/pyparser`。

## 运行

```sh
./build/pyparser <源文件路径>
```

例如：

```sh
./build/pyparser ./pyfile.py
```

若未提供文件路径，或文件无法打开，程序会输出错误信息并以非零状态码退出。

## 示例

输入 `pyfile.py`：

```python
def main():
    a = 10
    b = 20
    if a + b > 20:
        print("Hello Python!")
        if True:
            print("Aboba!")
        print("qwq")


main()
```

输出：

```
Hello Python!
Aboba!
qwq
```

## 项目结构

```
.
├── CMakeLists.txt
├── main.cpp                 入口：读取文件并驱动各阶段
├── include/
│   ├── lexer.hpp            Line / Token / TokenType 定义，词法接口
│   ├── parser.hpp           Node / NodeType 定义，语法分析接口
│   └── interpreter.hpp      Value / ValueType 定义，求值接口
├── source/
│   ├── lexer.cpp            缩进处理与词法分析
│   ├── parser.cpp           递归下降语法分析与 AST 打印
│   └── interpreter.cpp      树遍历求值与执行
└── pyfile.py               示例输入
```

## 已知限制

- 数值仅支持整数，不支持浮点数与负数字面量
- 二元运算符仅有 `+`、`-`、`<`、`>`；不支持 `*`、`/`、`==`、`!=`、`>=`、`<=` 及 `and`、`or`、`not`
- 函数不支持参数与返回值；条件分支仅有 `if` / `else`，不支持 `elif`
- 不支持 `while`、`for` 等循环结构
- 变量为全局作用域，函数体与顶层共享同一变量表，没有局部作用域
- 唯一内置函数为 `print`；函数调用的实参之间没有逗号分隔符
- 赋值目标必须是单个标识符
- 不支持同一行书写多条语句（词法阶段没有语句分隔标记）
- 没有错误恢复机制，遇到第一个词法、语法或运行时错误即终止

## License

本项目采用 MIT 协议，详见 [LICENSE](LICENSE)。
