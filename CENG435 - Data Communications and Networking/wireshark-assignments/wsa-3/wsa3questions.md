**Objective
In this assignment, you will examine network traffic using Wireshark and excercise the basics of network layer protocols.


**Instructions
Start up your favorite browser and Wireshark and start a packet capture.
Use traceroute to find a route to www.twitch.tv [traceroute -I -q 1 <website/IP addr>]
Send a single ping request with a size of 5000 bytes to hbostann.com [ping -c 1 -s 5000 hbostann.com ]
Answer the questions below.
Tip: Before you start doing the exercises, it is highly recommended to close all programs
that communicate over network to make the task easier for yourself.

**Questions
How many ICMP echo requests did traceroute send? How does the TTL fields of these packets change?
What are the numbers and source IP addresses of TTL-exceeded responses (if any)? Compare them with the output of the traceroute command.
Can you explain how traceroute found out the route to destination? Is this route always the same assuming you run traceroute from same location/network? Explain briefly.
What is the IP header length and total packet length of the DNS query response with type A record for hbostann.com? (If no dns requests are made for hbostann.com please flush your dns cache and try again)
What is the value of Protocol field in IP header for UDP communication and for ICMP communication?
In the ping request you made to hbostann.com, has the IP datagram been fragmented? If so, how many fragments are used and why?