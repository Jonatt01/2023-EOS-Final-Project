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
- **set the expect using time of devices**
    - `expect time | user Jonathan`
- **set the expect WATT of devices**
    - `expect WATT | user Jonathan`
- set preference
    - `preference | user Jonathan`
- change the user specific mode
    - `mode afternoon | user Jonathan`
- emergency
    - `emergency earthquake`
- open device (`duration 0` means do not set with duration)
    - `control device | user Jonathan | duration 20 | bedroom light 3 | livingroom light 2`
    - `control device | user Jonathan | duration 0 | bedroom light comfort | kitchen light comfort`
- set place to comfort (`duration 0` means do not set with duration)
    - `room | user Jonathan | duration 20 | bedroom comfort | bathroom comfort`
- reservation
    - `reservation 30 | user Jonathan | bedroom light 3 | livingroom light 2`
    - `reservation 30 | user Jonathan | bathroom light comfort | livingroom light 2`
- **checkout the using time**
    - `using time | bedroom light`
- **checkout the temperature**
    - `temperature | bedroom`
- **check out device status**
    - `device status | bedroom light`
- **calculate**
    - `calculate bill`

## parser
- output list of structures

## scheduler 
- insert the list of structures into the original task list

## recommand condition
- temperature is higher than his comfort
    - server will check the temperature in shared memory to decide whether recommand the user to open the aircondition (2 temperature)
- device using more than user wish
    - tell the user about this situation
    - recommand the user to turn off the device
- simultaneously opening device more than user wish
    - tell the user about this situation

## bill calculate
- WATT * sec * money a degree

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
