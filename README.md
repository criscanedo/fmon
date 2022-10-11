fmon
====

fmon (file monitor) is a small C program intended to make easy running your own
custom C code whenever file events are detected.

Requirements
------------

A computer running Linux.

Installation
------------

	git clone https://github.com/criscanedo/fmon
	cd fmon
	sudo make install

Running fmon
------------

Simply invoke `fmon` with the files you want to monitor.

	fmon ~/irc/20221010.log

Configuration
-------------

By default `fmon` prints new lines from a file after it's been modified much
like `tail -f`, with the exception that nothing is done when a file has been
truncated.

`config.h` is provided to allow you to change this behavior for any `inotify`
file event.

```c
struct action {
    uint32_t imask;
    void (*handle)(struct file_info *):
} actions[] = {
    { IN_MODIFY, printlines } /* call printlines when a file is modified */
};
```

A list of inotify events can be found in the `inotify(7)` man page or its header
file `/usr/include/sys/inotify.h`:

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

If, for example, you wanted `fmon` to print to standard output whenever it
detects a file has closed, this is all you'd need to do:

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

 Recompile and reinstall with `sudo make install`.
