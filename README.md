## Firmware Engineer Interview

## Requirements

- Microcontroller with internet access, the following are ok:
    - MCU: ESP8266 / ESP32 / STM32 / RP2040 / ARM Cortex M based boards
    - Development Environment: Bare metal / Arduino / Free rtos / Mbed OS / Zephyr Os

### Simulation

In case you do not have a board, you can use an emulated environment.

- [QEMU](https://github.com/Ebiroll/qemu_esp32) 
- [Wokwi](https://wokwi.com/) -  online simulator, here is an example on esp32 that connects to WiFi [https://wokwi.com/projects/346593345301119571]( https://wokwi.com/projects/346593345301119571)


## Challenge description

In this challenge you are presented with a TCP server (host: 20.110.238.2 port 5050) that you can connect and communicate with, for example using [netcat](https://en.wikipedia.org/wiki/Netcat)

```console
$ nc 20.110.238.2 5050         
Hello Padawan!

One of our engineers has been trying to decompress
a gzip string for a month now, can you help him? 
Below is the hex encoded compressed string

1f8b080032f7d85f02ff55cac10980400c04c056b602abb18153721a249b2309dabe873fbfc3aca760932c0c89ee618dbb406d84df62c282266a9e8ac6d452277ab87d46e7e371290f64b59ac93fffe1f202fcefa41e62000000
Secret:

```

Write a firmware that:

- Opens a [TCP Socket](http://beej.us/guide/bgnet/html/) with the server and reads the challenge description. Server IP is `20.110.238.2` Port `5050`. 
- Hex decodes the compressed string and gzip decompresses it
- Sends the decompressed string back to the server for validation.

The firmware should solve the challenge with the above workflow in realtime, for example its ok to write a test function that tries to decode the above hex code but the final solutuion should fetch it from the server in realtime, not decoding a hard coded hex string.

on success you be given another secret in the form `NOVEK{--your scret---}`, use this string as the last [commit message](https://guides.github.com/introduction/git-handbook/) when you submit your work

```
Secret:valid decompressed string

Nice work padawan, here is you flag: NOVEK{--your scret---}
```

- on failure, the server replies with the below and terminates the tcp connection.

```
Secret:invalid string       

Ummh i dont think that's it

```

>> **Note: You need to terminate your response with a new line `\n` character for the server to process the response**

## Submission 

>> You will be required to implement the solution during the live coding interview. Please do not solve it before. The reason we send the challenge before the interview is to assess your code design & review process.

Before solving the challenge create a new branch from master e.g `challenge1` then mark it a draft PR, once done mark the PR as ready for review.

The challenge solution including build instructions should be placed in the challenge folder.

Its important to note you will be assessed based on the puzzle pieces you solved and not the entire solution, for example a piece of code that can atleast open & close a socket is better than the code with the entire solution but is not valid or does not work.

Below are the areas to be assessed: 
- code review & design
- use of version control
- establishing connection with challenge server
- get & log challenge string from server
- send server decompressed string & get appropriate response from server

Don't [DOS](https://en.wikipedia.org/wiki/Denial-of-service_attack) the server, use atleast 100ms delay in your super loop.

### Would be great to have
- Your code follows a [C Style](https://developer.gnome.org/programming-guidelines/stable/c-coding-style.html.en) standard
- Add a section to the readme with build instructions