fmon
====

fmon (file monitor) is a small C program intended to make easy calling your
own custom code whenever file events are detected.

Requirements
------------

A computer running Linux.

Installation
------------

	git clone https://github.com/ccanedo/fmon
	cd ./fmon
	sudo make install

Running fmon
------------

Simply invoke `fmon` with the files you want to monitor.

	fmon ~/irc/20221010.log

Configuration
-------------

You may add additional inotify events to run code for when monitoring files
by simply adding another `struct action` entry in the `actions` array
defined in `config.h`.

By default `fmon` simply prints new data from a file after it's been
modified. It mimics `tail -f` with the exception that nothing is done when
a file has been truncated.

```c
struct action {
	uint32_t imask;
	void (*handle)(struct file_info *):
} actions[] = {
	{ IN_MODIFY, printlines }
};
```

A list of inotify events can be found in the inotify man page or the header
file `/usr/include/sys/inotify.h` which contains the following code:

```c
/* Supported events suitable for MASK parameter of INOTIFY_ADD_WATCH.  */
#define IN_ACCESS	 0x00000001	/* File was accessed.  */
#define IN_MODIFY	 0x00000002	/* File was modified.  */
#define IN_ATTRIB	 0x00000004	/* Metadata changed.  */
#define IN_CLOSE_WRITE	 0x00000008	/* Writtable file was closed.  */
#define IN_CLOSE_NOWRITE 0x00000010	/* Unwrittable file closed.  */
#define IN_CLOSE	 (IN_CLOSE_WRITE | IN_CLOSE_NOWRITE) /* Close.  */
#define IN_OPEN		 0x00000020	/* File was opened.  */
#define IN_MOVED_FROM	 0x00000040	/* File was moved from X.  */
#define IN_MOVED_TO      0x00000080	/* File was moved to Y.  */
#define IN_MOVE		 (IN_MOVED_FROM | IN_MOVED_TO) /* Moves.  */
#define IN_CREATE	 0x00000100	/* Subfile was created.  */
#define IN_DELETE	 0x00000200	/* Subfile was deleted.  */
#define IN_DELETE_SELF	 0x00000400	/* Self was deleted.  */
#define IN_MOVE_SELF	 0x00000800	/* Self was moved.  */
```

#### Configuration Example

If you wanted to do something after a file was closed you would define a
function with whatever it is you want to do, for instance:

```c
/* handles.h */
void itclosed(struct file_info *info);
```

```c
/* handles.c */
void itclosed(struct file_info *info)
{
	printf("%s was closed.\n", info->pathname);
}
```

Then just add a new entry in `actions` to call `itclosed` whenever the
`IN_CLOSE` inotify event is detected:

```c
/* config.h */
struct action {
	uint32_t imask;
	void (*handle)(struct file_info *):
} actions[] = {
	{ IN_MODIFY, printline }
	{ IN_CLOSE, itclosed }
};
```

Simply re-compile and re-install with `sudo make install`.
