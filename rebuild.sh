#! /bin/sh
sudo mn -c
sudo python ./kill_ovs.py
sudo ovs-dpctl del-dp system@ovs-system
sudo rmmod openvswitch
sudo rm -rf /var/log/openvswitch/*
sudo rm -rf /usr/local/var/log/openvswitch/*
sudo rm -rf /usr/local/etc/openvswitch
sudo rm -rf /usr/local/var/run/openvswitch
#sudo rm /var/log/syslog
#sudo touch /var/log/syslog
#sudo chmod 777 /var/log/syslog
sudo make clean
sudo ./configure CC=gcc-4.8 CFLAGS="-g -O2 -march=native" --with-linux=/lib/modules/$(uname -r)/build --enable-Werror
sudo make -j6
sudo make -j6 install
sudo make -j6 modules_install
sudo rm /etc/depmod.d/openvswitch.conf
sudo touch /etc/depmod.d/openvswitch.conf
sudo ./kmod_config.sh
sudo depmod -a
sudo /sbin/modprobe openvswitch
sudo mkdir -p /usr/local/etc/openvswitch
sudo ovsdb-tool create /usr/local/etc/openvswitch/conf.db vswitchd/vswitch.ovsschema
sudo mkdir -p /usr/local/var/run/openvswitch
sudo ovsdb-server --remote=punix:/usr/local/var/run/openvswitch/db.sock --remote=db:Open_vSwitch,Open_vSwitch,manager_options --private-key=db:Open_vSwitch,SSL,private_key --certificate=db:Open_vSwitch,SSL,certificate --bootstrap-ca-cert=db:Open_vSwitch,SSL,ca_cert --pidfile --detach --log-file
sudo ovs-vsctl --no-wait init
sudo ovs-vswitchd --pidfile --detach --log-file
sudo /sbin/lsmod | grep openvswitch
sudo modinfo openvswitch | grep version:
sudo ovs-appctl vlog/set warn
