# Compile

```bash
# clean the tree
make mrproper

# configure the kernel options
make menuconfig
# save the config and make sure 'CONFIG_LOCALVERSION_AUTO' is set to 'y'

# build! :D
make
make modules_install

# move files
cp -iv arch/x86/boot/bzImage /boot/vmlinuz-6.4.0-rc1
cp -iv System.map /boot/System.map-6.4.0
cp -iv .config /boot/config-6.4.0

# install the documentation
install -d /usr/share/doc/linux-6.4.0
cp -r Documentation/* /usr/share/doc/linux-6.4.0
```

# Config

```bash
cp -v /boot/config-6.4.0 ./.config
```

# Boot log

```bash
cp -v /var/log/boot.log .
```
