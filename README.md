# FINAL PROJECT
## system
- 最多接受10個人註冊
- user id length < 20, password length < 20 
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
## tables

## commands
- register id
    - `register`
- log in
    - `login`
- delete user id
    - `delete`
- set the mode
    - `setmode | user Jonathan | afternoon`
- set preference
    - `preference | user Jonathan`
- change the user specific mode
    - `mode afternoon | user Jonathan`
- emergency
    - `emergency earthquake`
- open device
    - `control device | user Jonathan | bedroom light 3 | livingroom light 2`
- set place to **comfort**
    - `room | user Jonathan | bedroom comfort | bathroom comfort`
- open device with comfort
    - `control device | user Jonathan | bedroom light comfort | kitchen light comfort`
- reservation
    - `reservation 30 | user Jonathan | bedroom light 3 | livingroom light 2`
    - `reservation 30 | user Jonathan | bathroom light comfort | livingroom light 2`


## parser
- output list of structures

## scheduler 
- insert the list of structures into the original task list

## mapping
- number to devices
    - bedroom
        - 1: air conditioner
        - 2: light
        - 3: fan
        - 4: curtain

    - living room
        - 5: air conditioner
        - 6: light
        - 7: fan
        - 8: curtain

    - kitchen
        - 9: light

    - bathroom
        - 10: air conditioner
        - 11: light

    - special
        - 12: doors
