obj-m += main.o

PWD := $(CURDIR)

SILENT := all
PHONY := all
all: module

PHONY += module
module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

PHONY += clean
clean:
	mv ./compile_commands.json ./compile_commands.json.bak
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	mv ./compile_commands.json.bak ./compile_commands.json

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
