# **************************************************************************** #
#                                     VARS                                     #
# **************************************************************************** #

MAKE = make

NAME = main

# **************************************************************************** #
#                                   SOURCES                                    #
# **************************************************************************** #

obj-m += $(NAME).o
main-y := init.o

PWD := $(CURDIR)

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

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

SILENT += check
PHONY += check
check:
	if [ "$(shell id -u)" -ne 0 ]; then \
		echo "Error: this rule must be run as root."; \
		exit 1; \
	fi
	echo "Loading module..."; \
	$(MAKE) -s ins; \
	echo
	echo "Running 'cat /proc/mymounts'..."; \
	cat /proc/mymounts; \
	echo
	echo "Unloading module..."; \
	rm $(NAME).test || true; \
	$(MAKE) -s rm; \
	echo

PHONY += ins
ins:
	insmod $(NAME).ko

PHONY += rm
rm:
	rmmod $(NAME).ko

PHONY += rmf
rmf:
	rmmod -f $(NAME).ko

.SILENT: $(SILENT)
.PHONY: $(PHONY)
