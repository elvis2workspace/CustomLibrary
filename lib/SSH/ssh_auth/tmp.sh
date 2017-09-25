#!/usr/bin/expect
spawn ssh-keygen -b 1024 -t rsa
expect *key*
send "\r"
expect *passphrase*
send "\r"
expect *again:
send "\r"
interact