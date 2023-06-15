all: server.c authentication.c create_table.c translate.c costimizer.c parser.c scheduler.c list_operation.c relay.c reservation_signal.c dispatcher.c user_client_2.c user_client_3.c
	gcc -o server server.c authentication.c create_table.c translate.c costimizer.c parser.c scheduler.c list_operation.c dispatcher.c reservation_signal.c additional.c -lpthread
	gcc user_client.c -o user_client -lpthread
	gcc user_client_2.c -o user_client_2 -lpthread
	gcc user_client_3.c -o user_client_3 -lpthread
	gcc relay.c -o relay -lpthread -lpthread
	gcc device_client.c -o device_client -lpthread
	gcc deviceStatus_client.c -o deviceStatus_client -lpthread
	gcc user_client_register.c -o user_client_register -lpthread
	gcc user_client_mode.c -o user_client_mode -lpthread
	gcc user_client_emergency.c -o user_client_emergency -lpthread
	gcc user_client_closealll.c -o user_client_closealll -lpthread
	gcc user_client_preference.c -o user_client_preference -lpthread
	gcc user_client_set_watt_n_time.c -o user_client_set_watt_n_time -lpthread
	gcc user_client_reservation.c -o user_client_reservation -lpthread
	gcc user_client_duration.c -o user_client_duration -lpthread
	gcc user_client_check.c -o user_client_check -lpthread
	gcc user_client_delete.c -o user_client_delete -lpthread
	gcc user_register_Brain.c -o user_register_Brain -lpthread
	gcc user_client_duration_Jonathan.c -o user_client_duration_Jonathan -lpthread
	gcc user_client_duration_Brain.c -o user_client_duration_Brain -lpthread
	gcc user_client_mode_Brain.c -o user_client_mode_Brain -lpthread


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
	rm user_client_closealll
	rm user_client_preference
	rm user_client_set_watt_n_time
	rm user_client_reservation
	rm user_client_duration
	rm user_client_check
	rm user_client_delete
	rm user_register_Brain
	rm user_client_duration_Jonathan
	rm user_client_duration_Brain
	rm user_client_mode_Brain
