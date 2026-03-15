ESP-NOW is a fast and low power way for devices to talk to each other without needing a Wi-Fi router. This is because ESP-NOW is a connectionless communication protocol that was made by Espressif Systems for ESP32 and ESP8266 boards. ESP-NOW allows devices to send data to each other quickly which is great for things like smart home sensors and remote controls.

So what kind of range can ESP-NOW handle?

Well it depends on the situation.

In a field with the antennas that come on the board ESP-NOW can usually send data between 220 and 400 meters.

If everything is perfect like in an open space with no obstacles ESP-NOW can send data up to 650 meters or more.

There is a long range mode that lets ESP-NOW send data over 900 meters which is almost a kilometer but this does make the data rate slower.

If you are inside ESP-NOW can usually send data between 40 and 100 meters depending on the walls and things that are in the way.


Some of the things about ESP-NOW are that it does not need Wi-Fi to work, which means devices can connect directly to each other.

This makes ESP-NOW really fast so data gets sent instantly.

ESP-NOW is also really good at saving power, which's great for devices that run on batteries.

ESP-NOW can send messages that are encrypted or not encrypted.

The normal speed of ESP-NOW is 1 megabit per second. In open spaces it can actually send data at around 214 kilobits, per second.

If you use antennas and point them in the right direction you can make ESP-NOW send data even farther.



Working Mechanism of ESP-

The working mechanism of ESP-NOW on devices like the ESP32 is based on direct communication at the Wi-Fi MAC layer. This means that ESP devices can send wireless packets directly to each other. They do not need to connect to a Wi-Fi router to do this.

Here is how ESP-NOW works.

1. Device Initialization

First the ESP device turns on its Wi-Fi hardware.. It does not connect to any Wi-Fi network or router. The Wi-Fi radio is just activated so the device can send and receive wireless data frames. This is the step in the ESP-NOW process.

2. Peer Registration

Every ESP device has a MAC address. This MAC address serves as the devices hardware identification. Before communication starts the sender device must store the MAC address of the receiver. This is called adding a peer.

For example the sender ESP32 knows the MAC address: 30:C6:F7:91:44:20. Once the receiver is registered as a peer communication can begin. The ESP- process relies on the MAC address of the ESP devices.

3. Data Preparation

When the sender wants to transmit data it creates a data packet. This packet includes the MAC address the sender MAC address and the payload data. The payload data can be sensor readings, commands or messages. The maximum payload size is 250 bytes. The ESP- packet is small and simple.

4. Packet Encapsulation

The ESP32 places this data inside a Wi-Fi frame called a vendor-specific action frame. This frame contains the MAC header the Espressif vendor identifier, the ESP-NOW payload and an error-checking code. This structure enables ESP devices to recognize that the packet is part of ESP- communication. The ESP- frame is unique and simple.

5. Wireless Transmission

The packet is sent using the 2.4 GHz Wi-Fi radio. Before sending the Wi-Fi hardware checks if the channel is free. If the channel is free the packet is sent. If the channel is busy the device waits for a random time and then tries again. The ESP-NOW transmission is fast and reliable.

6. Packet Reception

All nearby ESP devices can receive the frame. However only the device whose MAC address matches the destination address will process the packet. Other devices simply ignore it. The ESP- reception is secure and simple.

7. Data Processing

When the correct ESP device receives the packet the MAC layer extracts the ESP- payload. The ESP- software passes the data to the application program. A receive callback function in the program is triggered. The ESP device can then take actions, such as displaying a sensor value or controlling a motor or actuator. The ESP- process is fast and efficient.

8. Optional Encryption

ESP-NOW also supports AES-128 encryption for communication. When encryption is enabled the sender encrypts the data before transmission. The receiver decrypts the data after receiving it. This helps safeguard the communication from devices. The ESP- encryption is secure and reliable.

9. Continuous Communication

Once everything is set up ESP devices can continuously send packets to each other. ESP-NOW supports communication types, such as one-to-one one-to-many and many-to-one. All of this occurs without needing a Wi-Fi router. The ESP- communication is fast low-power and ideal for IoT, robotics and embedded systems.

Simple Summary

In terms ESP-NOW enables ESP devices to communicate directly using their MAC addresses by sending small Wi-Fi packets between them. The ESP- process bypasses the usual Wi-Fi networking layers making the communication very fast low-power and ideal, for IoT, robotics and embedded systems. The ESP- technology is simple, reliable and secure.
