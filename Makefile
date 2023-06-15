all: server.c authentication.c create_table.c translate.c costimizer.c parser.c scheduler.c list_operation.c relay.c reservation_signal.c dispatcher.c user_client_2.c user_client_3.c
	gcc -o server server.c authentication.c create_table.c translate.c costimizer.c parser.c scheduler.c list_operation.c dispatcher.c reservation_signal.c additional.c -lpthread
	gcc user_client.c -o user_client
	gcc user_client_2.c -o user_client_2
	gcc user_client_3.c -o user_client_3
	gcc relay.c -o relay -lpthread
	gcc device_client.c -o device_client
	gcc deviceStatus_client.c -o deviceStatus_client
	gcc user_client_register.c -o user_client_register
	gcc user_client_mode.c -o user_client_mode
	gcc user_client_emergency.c -o user_client_emergency
	gcc user_client_control.c -o user_client_control
	gcc user_client_preference.c -o user_client_preference
	gcc user_client_set_watt_n_time.c -o user_client_set_watt_n_time
	gcc user_client_reservation.c -o user_client_reservation
	gcc user_client_duration.c -o user_client_duration
	gcc user_client_check.c -o user_client_check
	gcc user_client_delete.c -o user_client_delete
	gcc user_register_Brain.c -o user_register_Brain


clean:
	rm user_client
	rm user_client_2
	rm user_client_3
	rm server
	rm relay
	rm device_client
	rm deviceStatus_client
	rm user_client_register
	rm user_client_mode
	rm user_client_emergency
	rm user_client_control
	rm user_client_preference
	rm user_client_set_watt_n_time
	rm user_client_reservation
	rm user_client_duration
	rm user_client_check
	rm user_client_delete
	rm user_register_Brain

