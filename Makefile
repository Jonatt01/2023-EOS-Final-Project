all:
	gcc -o server server.c authentication.c 
	gcc user_client.c -o user_client
	gcc relay.c -o relay -lpthread
	gcc msgQsnd.c -o msgQsnd
	gcc device_client.c -o device_client
	gcc deviceStatus_client.c -o deviceStatus_client
clean:
	rm user_client
	rm server
	rm relay
	rm msgQsnd
	rm device_client
	rm deviceStatus_client

