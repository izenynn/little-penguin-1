obj-m += main.o
#obj-m += other.o

PWD := $(CURDIR)

SILENT := all
PHONY := all
all: modules

SILENT += dev
PHONY += dev
dev: modules compile_commands.json

PHONY += modules
modules:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

PHONY += modules_install
modules_install:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules_install

PHONY += compile_commands.json
compile_commands.json:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) compile_commands.json
	sed -i -e 's/-fconserve-stack //' -e 's/-fno-allow-store-data-races //' -e 's/-mindirect-branch-register //' -e 's/-mindirect-branch=thunk-extern //' -e 's/-mpreferred-stack-boundary=3 //' compile_commands.json

PHONY += clean
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

PHONY += check
check:
	@if [ "$(shell id -u)" -ne 0 ]; then \
		echo "Error: This rule must be run as root."; \
		exit 1; \
	fi
	insmod main.ko
	dmesg -T | tail -1
	rmmod main.ko
	dmesg -T | tail -1

.SILENT: $(SILENT)
.PHONY: $(PHONY)
