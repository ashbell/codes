client : main.o csopen.o recv_fd.o
	cc -o client main.o csopen.o recv_fd.o
main.o : main.c open.h
	cc -c -o main.o main.c 
csopen.o : csopen.h csopen.c  
	cc -c -o  csopen.o csopen.c 
recv_fd.o : recv_fd.c apue.h
	cc -c -o recv_fd.o recv_fd.c 
clean :
	-rm *.o client
	
