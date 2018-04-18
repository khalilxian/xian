all:client server
client : cclient.c types.c types.h
	gcc cclient.c types.c -o client -l pthread
	
server: sserver.c types.c types.h
	gcc sserver.c types.c -o server  -l pthread

clean : 
	rm  client server