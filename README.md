# FINAL PROJECT
## System Restriction
- At most 10 user register to the system.
    - user id length < 20 characters
    - password length < 20 characters

## How to use for RPi 3B+
- Download this project on client, server, and device sides.
    ```
    $ git clone https://github.com/Jonatt01/2023-EOS-Final-Project.git
    ```
- Check IP on client side and device side
**terminal on server side**
    ```
    $ ifconfig
    ```
And fill the IP address to all client side codes and device side codes.
If user side is the same PC with server side, then set the IP address in client side code `127.0.0.1`

- client side (must register to server first if not registered before)
    ```
    $ make
    $ ./register
    $ ./user_client_XXX
    ```

- server side

    **terminal 1 (run system server)**
    ```
    $ make
    $ ./server
    ```
    **terminal 2 (Run relay function)**
    ```
    $ ./relay
    ```

- device side

    **terminal 1**
    ```
    $ make
    $ sudo insmod seg_driver.ko
    $ ./device_client
    ```

    **terminal 2**
    ```
    $ sudo ./dht11
    ```
    **terminal 3**
    ```
    $ sudo ./dht11_2
    ```
     **terminal 4**
    ```
    $ sudo ./client_dht11
    ```
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
- set the expect using time of devices
    - `expect time | user Jonathan`
- set the expect WATT of devices
    - `expect watt | user Jonathan`
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
- checkout the using time
    - `check using time | user Jonathan | bedroom light`
- checkout the temperature
    - `check temperature | user Jonathan | bedroom | livingroom | bathroom`
- check out device status
    - `check device status | user Jonathan | bedroom light | livingroom fan`
- calculate
    - `calculate bill`

## parser
- output list of structures

## scheduler 
- insert the list of structures into the original task list

## recommend condition
- temperature is higher than his comfort
    - server will check the temperature in shared memory to decide whether recommand the user to open the aircondition (2 temperature)
- device using time more than user wish
    - tell the user about this situation
    - recommend the user to turn off the device (not yet)
- simultaneously opening device more than user wish (not yet)
    - tell the user about this situation

## Some macros you can set
- In `additional.c`
    ```c=
    # define AIRCONDITION_WATT 5 // The watt of airconditioners
    # define LIGHT_WATT 1 // The watt of lights
    # define FAN_WATT 2 // The watt of fans
    # define MONEY_PER_WATT 5 // the cost per WATT that government charges
    ```

## mapping
- number of devices
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