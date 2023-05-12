# How to

## Extra version

The linus git tree has the EXTRAVERSION `-rc1`, to change it we just need to open the Makefile inside the `linux/` directory and change the line to:
```diff
-EXTRAVERSION = -rc1
#EXTRAVERSION = -thor_kernel
```

Recompile the kernel
```bash
# Clean the tree
make mrproper

# Get the .config we saved on `/boot/config-6.4.0` in assignment00
cp -v /boot/config-6.4.0 ./.config

# Configure the kernel options (just load the .config)
make menuconfig
# save the config and make sure 'CONFIG_LOCALVERSION_AUTO' is set to 'y'

# Build! :D
make
make modules_install

# Move the new kernel files
cp -iv arch/x86/boot/bzImage /boot/vmlinuz-6.4.0-thor_kernel
# This will overwrite the ones generated in assignment00
# But they should be the same so you can skip instead
cp -iv System.map /boot/System.map-6.4.0
cp -iv .config /boot/config-6.4.0

# Optional: remove the previus kernel of assignment00
rm -v /boot/vmlinuz-6.4.0-rc1
```

## Grub

```bash
# Add/edit the `/boot/grub/grub.cfg` entry to match the new kernel EXTRAVERSION string
nvim /boot/grub/grub.cfg
# Reboot into your brand new kernel! :D
```

# Files to turn in

## Boot log

```bash
cp -v /var/log/boot.log .
```

## Generate patch files

Create a new branch for the changes
```bash
git checkout -t -b feat-extraversion-string
```

Generate the patch file
```bash
git format-patch --base=auto --cover-letter -o outgoing/ master
```

Add the necessary lines to the patch files (in my case, the `Signed-off-by: ... <...>` and description line)

## Check the patches

To check the patches like a reviewer will do run the following commands
```bash
# Create a new branch for the review
git checkout -b patch-review [base commit-id]

# Apply the patches from mailbox
git am patches.mbox

# Apply the patches locally
# Skip the empty `cover-letter` patch
git am /path/to/patches/0000-[...].patch
git am /path/to/patches/0001-[...].patch
# Or do it all at once
# This will apply in the order defined by your shell
# Bash expand wildcards in alphabetical order
# Which is fine becasuse the patches have the [NUMBER]-[NAME] format :D
git am /path/to/patches/*.patch
# If the patch fails becasuse the `cover-letter` is empty just skip it
git am --skip

# You can review changes by running
git log -p

# Once finished go back to master
git checkout master
```
