sudo ip route add 10.10.1.0/24 via 10.10.4.1
sudo ip route add 10.10.2.0/24 via 10.10.5.1

r3=$(getent ahosts "r3" | cut -d " " -f 1 | uniq)

r3_adapter=$(ip route get $r3 | grep -Po '(?<=(dev )).*(?= src| proto)')

sudo tc qdisc change dev $r3_adapter root netem loss 38% delay 3ms