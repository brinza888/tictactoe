#!/bin/sh

key_dir="/etc/ssh/keys"
key_priv="${key_dir}/ssh_host_rsa_key"
key_pub="${key_priv}.pub"

mkdir -p "$key_dir"

if [[ ! -f $key_priv ]]; then
    ssh-keygen -q -t rsa -b 4096 -f "$key_priv" -N ""
fi

chmod 600 "$key_priv"
chmod 644 "$key_pub"

exec /usr/sbin/sshd -D -e
