									Bash Shell Script Prog 

1. cat file | sed '/^$/d' >> outfile  : delete the blank lines.

2. echo {1..20}.c | tr '0-9' 'a-z' | xargs touch  : touch files with 1-20 and use tr translante digit to alpha.

3. echo -e {1..100}'\n' > num  : create a file contain numbers 1-100, remember use -e option. and end by '\n'.

4. cat num | echo $[ $(tr '\n' '+') 0 ] : calculate the sum of numbers. ps: the lenght of exp not too long. also `bc`.

5. echo $[ $(cat file) ] : calculate the expression from file, exp like : 1+2+3..., not too long. 

6. echo aaaaaHHHdfsdfssd | sed 's/[^.\n]/&\n/g'| sed '/^$/d' | sort | uniq -c | tr -d ' \n'  :\
   show the count of alpha appearances and sort it.

7. echo -e {1..10}'+' | tr -d ' '| sed 's/+$//g' | bc  : nice job.

8. seq -s '/' 10 | sed 's/^/scale=20;/' | bc : Funny!

9. use nc and script or scriptreplay to show .
   > a. Create a pipe to use:   mkfifo fifo . On server.
   > b. Listen local IP and PORT :  nc -ln server_ip -p server_port.  On client.
   > c. Bind our PIPE to Player_server:  cat fifo | nc server_ip port.
   > d. Now, show your ideas in console. script -f fifo. The client will see what u r doing.
   -----------------
   > e. Use scriptreplay commands.  script -t 2> time.log -a output.session
   > f. Play it.  scriptreplay  time.log output.session.
   -----------------

10. find  . -maxdepth 1 -name "*.pdf" | grep "^./[a-z].*.pdf"
   > -maxdepth (a non-negative integer)



