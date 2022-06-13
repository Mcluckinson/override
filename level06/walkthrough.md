so the thing again asks for login and passwd

info functions: noticable stuff
    - printf //maybe overflow?
    - some weird signals and alarms
    - syscall // no shellcode here?
    - ptrace //dafuq?
    - auth //lets check this


auth: after some time notice this 
   0x08048866 <+286>:    cmp    -0x10(%ebp),%eax
is this the same stuff as level00 kek?

oh wow look at this
.---------------------------.
| !! TAMPERING DETECTED !!  |
'---------------------------'
[Inferior 1 (process 1862) exited with code 01]
(gdb)

ok, thats what ptrace does here.
   0x080487ba <+114>:    cmp    $0xffffffff,%eax
and this is where this shit happens

so we get to this line in gdb and change eax val from -1 to 0, ((gdb) set $eax=0)
then we can get to the cmp line

set breakpoint at cmp
chack whats compared: eax vs -0x10(%ebp)
check value of -0x10(%ebp): 

(gdb) x/d $ebp-0x10
0xffffd5d8:    6232805
(login pepega, passwd may be whatevs during gdb stuff)

try the serial
it works


cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8

