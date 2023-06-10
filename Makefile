all: server.c authentication.c create_table.c translate.c costimizer.c parser.c scheduler.c list_operation.c relay.c reservation_signal.c dispatcher.c
	gcc -o server server.c authentication.c create_table.c translate.c costimizer.c parser.c scheduler.c list_operation.c dispatcher.c reservation_signal.c
	gcc user_client.c -o user_client
	gcc relay.c -o relay -lpthread
	gcc device_client.c -o device_client
	gcc deviceStatus_client.c -o deviceStatus_client
clean:
	rm user_client
	rm server
	rm relay
	rm device_client
	rm deviceStatus_client

