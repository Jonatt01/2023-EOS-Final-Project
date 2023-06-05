all:
	gcc server.c -o server
	gcc user_client.c -o user_client
clean:
	rm user_client
	rm server

