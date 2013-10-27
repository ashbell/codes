opend : main.o cli_args.o request.o buf_args.o send_err.o send_fd.o
	cc -o opend main.o cli_args.o request.o buf_args.o send_err.o send_fd.o
send_err.o : send_err.c
	cc -c send_err.c
send_fd.o : send_fd.c
	cc -c send_fd.c
main.o : main.c opend.h 
	cc -c  -o main.o main.c  
cli_args.o : cli_args.c cli_args.h
	cc -c -o cli_args.o cli_args.c
request.o :request.c request.h 
	cc -c -o request.o request.c 
buf_args.o : buf_args.c buf_args.h
	cc -c -o buf_args.o buf_args.c 
clean:
	-rm *.o opend








