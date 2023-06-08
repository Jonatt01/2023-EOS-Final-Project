# FINAL PROJECT

## variable explaination

### server
- **clientfd**
    - The fd for the client
- **serverfd**
    - The fd for the server
- **device_status**
    - An array to save the status of all devices
    - The size is 1*12 float
    - Create by calling create_status_table defined in create_table.c
- **authenticate**
    - The flag to indicate whether the user is legal
    - "1" : legal, "0" : illegal