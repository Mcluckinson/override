
level08@OverRide:~$ ./level08
Usage: ./level08 filename
ERROR: Failed to open (null)

Нас встречает програ, которая на вход жрет файл
и судя по info functions читает, и пишет в него что-то

0x00000000004006f0  strcpy
0x0000000000400700  write
0x0000000000400710  fclose
0x0000000000400730  printf
0x0000000000400740  snprintf
0x0000000000400750  strncat
0x0000000000400760  fgetc
0x0000000000400770  close
0x0000000000400780  strcspn
0x00000000004007a0  fprintf
0x00000000004007b0  open
0x00000000004007c0  fopen
0x00000000004007d0  exit
0x00000000004008a0  frame_dummy
0x00000000004008c4  log_wrapper
0x00000000004009f0  main

Судя по всему он так же логгирует куда-то (log_wrapper)

ls -la показывает, что есть папочка backups, которой владеет level09:
drwxrwx---+ 1 level09 users      60 Oct 19  2016 backups

В которой есть файл 
-rwxrwx---+ 1 level09 users    24 Jun 13 14:43 .log

КАРОЧ

эта шляпа копирует файл в папку бэкапс. Если попробовать скопипастить файл .pass ничего и не выйдет
А вот если создать на него ссылку (предварительно бахнув chmod себе в папку) - то его скопирует кек. И он будет уже от нашего юзера
и откроется. Пароль 

fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
