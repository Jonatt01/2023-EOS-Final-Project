all: server.c authentication.c create_table.c translate.c costimizer.c parser.c scheduler.c list_operation.c relay.c reservation_signal.c dispatcher.c
	gcc -o server server.c authentication.c create_table.c translate.c costimizer.c parser.c scheduler.c list_operation.c dispatcher.c reservation_signal.c
	gcc user_client.c -o user_client
	gcc user_client_2.c -o user_client_2
	gcc user_client_3.c -o user_client_3
	gcc relay.c -o relay -lpthread
	gcc device_client.c -o device_client
	gcc deviceStatus_client.c -o deviceStatus_client
clean:
	rm user_client
	rm user_client_2
	rm user_client_3
	rm server
	rm relay
	rm device_client
	rm deviceStatus_client

