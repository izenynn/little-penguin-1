# Working with linux-next

The linux-next tree is the holding area for patches aimed at the next kernel merge window.

## Initial set up

First clone a copy of the mainline Linux Git repository:
```bash
git clone https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
```

Then add a remote tracking branck for *linux-nest*:
```bash
cd linux
git remote add linux-next https://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git
```

Fetch *linux-next* plus tags:
```bash
git fetch linux-next
git fetch --tags linux-next
```

## Regular tracking

Update *linux-next*:
```bash
# to be safe
git checkout master
git remote update
```

List (recent) *linux-next* tags:
```bash
git tag -l "next-*" | tail
# ...
```

Choose the *linux-next* version that you will work from, and create a local branch based on that version:
```bash
git checkout -b my_local_branch next-20230512
```
