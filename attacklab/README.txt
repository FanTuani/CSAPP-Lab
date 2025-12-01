本文件包含一次 Attack Lab 实验实例所需的材料。

文件说明：

ctarget
具有代码注入漏洞的 Linux 可执行文件。用于完成实验的第 1–3 阶段。

rtarget
具有返回导向编程（Return-Oriented Programming, ROP）漏洞的 Linux 可执行文件。用于完成实验的第 4–5 阶段。

cookie.txt
文本文件，包含本次实验实例所需的 4 字节签名（cookie）。

farm.c
源代码文件，包含本实例的 rtarget 中的 gadget farm。可以自行编译（使用 -Og 编译选项）并对其反汇编以查找所需的 gadgets。

hex2raw
用于生成字节序列的工具程序。具体使用方法参见实验文档。


This file contains materials for one instance of the attacklab.

Files:

    ctarget

Linux binary with code-injection vulnerability.  To be used for phases
1-3 of the assignment.

    rtarget

Linux binary with return-oriented programming vulnerability.  To be
used for phases 4-5 of the assignment.

     cookie.txt

Text file containing 4-byte signature required for this lab instance.

     farm.c

Source code for gadget farm present in this instance of rtarget.  You
can compile (use flag -Og) and disassemble it to look for gadgets.

     hex2raw

Utility program to generate byte sequences.  See documentation in lab
handout.

