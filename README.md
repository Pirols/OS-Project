# OS-Project

1. Semaphores in DisastrOS (4 people)

   implement the following system calls in DisastrOS
   // creates a semaphore in the system, having num semnum
   // the semaphore is accessible throughuot the entire system
   // by its id.
   // on success, the function call returns semnum (>=0);
   // in failure the function returns an error code <0
   - int s=DisastrOS_semOpen(int semnum)

   //releases from an application the given
   // returns 0 on success
   // returns an error code if the semaphore is not owned by the application
   - int DisastrOS_semClose(int semnum)

   //decrements the given semaphore
   //if the semaphore is 0, the caller is put onto wait
   //returns an error code
   - int DisastrOS_semWait(int semnum);

   //increments the given semaphore
   //if the semaphore was at 0, and some other thread was waiting
   //the thread is resumed
   //returns 0 on success, an error code on failure 
   int DisastrOS_semPost(int semnum);

2. File System (2 people)
   implement a file system interface using binary files
   - The file system reserves the first part of the file
     to store:
     - a linked list of free blocks
     - linked lists of file blocks
     - a single global directory
     
   - Blocks are of two types
     - data blocks
     - directory blocks

     A data block are "random" information
     A directory block contains a sequence of
     structs of type "directory_entry",
     containing the blocks where the files in that folder start
     and if they are directory themselves
   
4. VideoGame (2 people)
   - implement a distributed videogame
     Server Side:
     - the server operates in both TCP and UDP
       TCP Part
       - registerning a new client when it comes online
       - deregistering a client when it goes offline
       - sending the map, when the client requests it
     - UDP part
       - the server receives preiodic upates from the client
         in the form of
	 <timestamp, translational acceleration, rotational acceleration>
	 Each "epoch" it integrates the messages from the clients,
	 and sends back a state update
       - the server sends to each connected client
         the position of the agents around him

     Client side
     - the client does the following
     - connects to the server (TCP)
     - requests the map, and gets an ID from the server (TCP)
     - receives udates on the state from the server

     - periodically
       - updates the viewer (provided)
       - reads either keyboard or joystick
       - sends the <UDP> packet of the control to the server

5. On Arduino (2 people)
   NO USING OF ARDUINO IDE, BARE METAL PROGRAMMING
     - implement a sytem to read one or more sensors at your choice,
     chosen among
     - encoder
     - Analog to digital converter
     - Digital Pins
     and to deliver binary  messages to the PC, following
     a binary protocol (not text)

    on the PC implement a program to interact with the arduino
     and to display the incoming messages


  Variants of the project on the microcontroller below:
  
  5a. Oscilloscope
     - implement a system using an aduino + a PC that realizes a simple
       oscilloscope
     - the oscilloscope is realized by measuring the analog value of
       a voltage on one of the pins of the microcontrollers
     - the oscilloscope is configured by the PC by setting
       - the sampling rate (hz) at which the samples should be acquired
       - the number of samples to acquire

     - upon a start command, the arduino samples the values from the
       ADC [implement a proper ADC interface, using interrupts]
       and assembles them in a message of the proper size, and
       sends them to the PC.

     - the oscilloscope supports also a "continuous" mode,
       when the samples are periodically sent to the PC.

     - the oscilloscope supports also sampling from multiple channels (pins)

     - on the PC implement a program to control the modalities of
       the oscilloscope

     - carefully design the API to be clean and compact

  5b. Sensor logger
     - if you have a bunch of sensors (e.g. temperature, humidity etc)
       implement a "sensor node".
     - a sensor node is a stand-alone device that is programmed once
       and records a series of data in an interval
     - from the PC you will configure (via serial),
       the period of acquisition of each data.
       The data will be stored on the eprom of the device for further reading
     - once connected again to the PC the device can be used to reading the
       recorded data and to stream them to the PC.

   5c. remote command copier (telecomando)
     - if you have an IR sensor, implement a program to
       "clone" a remote command
     - the cloning happens by
       a. telling the arduino that you are pressing a key on the remote command
          (from PC via serial line)
       b. on the arduino, IR sensor is sampled at high rate
          (from when the first pulse comes in), and the corresponding digital string is stored somewhere
       c. when the pulses stop (how to figure out the pulses stop?),
          send back to the PC the read string
       d. if you have an IR led, you might also be able to decode the IR
      - on the PC you will do a program that controls all this, via serial.

   5d. on a different microcontroller (not ATMEL AVR) port the orazio firmware
       Port only the arch folder!!!!
       Write sanity tests, and do severe debugging.
       The software will be tested with real sensors and actuators.

   5e. joystick+videogame
       Extension to the videogame project +1 person
       Implement a joystick using an arduino (old joystick non USB, if you have one)
       and integrate it in the videogame.
       
   
6. Raspberry PI Video Surveillance(3 people)
   Implement a controlled video surveillance system based on
   RPI, a webcam and a servo
   RPI side:
   - the RPI controls a servo in PWM (google for it), that
     moves a webcam, and reads the images of a webcam
   - a remote host connects to the RPI and sends a desired
     servo position, that moves servo
   - the RPI periodically posts the image grabbed from the webcam
   - implement full camera controls (resolution, exposure etc)
   PC side:
   - browser

