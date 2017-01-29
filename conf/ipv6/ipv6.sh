#!/bin/bash
ip -6 addr add 2607:fcd0:114:3d1::/64 dev eth0
ip -6 route add 2607:fcd0:114::1 dev eth0  metric 1024
ip -6 route add default via 2607:fcd0:114::1 dev eth0  metric 1024

