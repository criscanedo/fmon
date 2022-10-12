#include <stdio.h>
#include <sys/inotify.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>
#include "util.h"
#include "handles.h"
#include "config.h"

int nfiles = 0;                 /* Number of files to monitor */
int nready = 0;                 /* Number of poll fds ready */
int ifd = -1;                   /* inotify instance file descriptor */
int *wd = NULL;                 /* Buffer containing inotify watch descriptors */
struct pollfd fds[1];           /* Only polling the inotify instance fd */
struct file_info *files = NULL; /* Buffer containing file information */
char *name = NULL;              /* Program name */

static void run_actions(const struct inotify_event *event)
{
    int wdi;
    for (size_t i = 0; i < LENGTH(actions); i++) {
        if (event->mask & actions[i].imask) {
            if ((wdi = indexof(&event->wd, wd, nfiles)) == -1) {
                die("%s: could not get index of wd %d", name, event->wd);
            }
            actions[i].handle(&files[wdi]);
        }
    }
}

static void handle_inotify(void)
{
    char buf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t nbytes;

    for (;;) {
        nbytes = read(ifd, buf, sizeof(buf));
        if (nbytes == -1 && errno != EAGAIN) {
            die("read");
        }

        if (nbytes <= 0) {
            break;
        }

        for (char *ptr = buf; ptr < buf + nbytes;
                ptr += sizeof(struct inotify_event) + event->len) {
            event = (const struct inotify_event *)ptr;
            run_actions(event);
        }
    }
}

static void run(void)
{
    while (1) {
        if ((nready = poll(fds, 1, -1)) == -1) {
            if (errno == EINTR) {
                continue;
            }
            die("poll");
        }
        if (nready > 0) {
            if (fds[0].revents & POLLIN) {
                handle_inotify();
            }
        }
    }
}

static void init(int argc, char *argv[])
{
    uint32_t mask = 0;
    nfiles = argc - 1;
    name = argv[0];

    if ((ifd = inotify_init1(IN_NONBLOCK)) == -1) {
        die("%s: could not create inotify instance", name);
    }
    if (!(wd = calloc(nfiles, sizeof(int)))) {
        die("calloc");
    }
    if (!(files = malloc(sizeof(struct file_info) * nfiles))) {
        die("malloc");
    }

    /* Aggregate masks from actions array */
    for (int i = 0; i < LENGTH(actions); i++) {
        mask |= actions[i].imask;
    }
    for (int i = 0; i < nfiles; i++) {
        if ((wd[i] = inotify_add_watch(ifd, argv[i + 1], mask)) == -1) {
            die("%s: could not add watch for %s", name, argv[i + 1]);
        }
        setinfo(&files[i], argv[i + 1], strlen(argv[i + 1]));
    }
    fds[0].fd = ifd;
    fds[0].events = POLLIN;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        die("Usage: %s FILE ...", argv[0]);
    }
    init(argc, argv);
    run();
    return 0;
}
