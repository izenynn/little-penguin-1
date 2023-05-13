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

PHONY += fclean
fclean: clean

PHONY += re
re: fclean all

PHONY += check
check: modules
	@if [ "$(shell id -u)" -ne 0 ]; then \
		echo "Error: this rule must be run as root."; \
		exit 1; \
	fi
	@ echo "Loading module..."; \
	insmod main.ko; \
	dmesg -T | tail -1; \
	echo
	@ echo "Running tests..."; \
	gcc main_test.c -o main.test; \
	./main.test || true; \
	rm main.test || true; \
	echo
	@echo "Unloading module..."; \
	rmmod main.ko; \
	dmesg -T | tail -1; \
	echo

.SILENT: $(SILENT)
.PHONY: $(PHONY)
