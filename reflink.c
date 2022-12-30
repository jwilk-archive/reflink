/* Copyright © 2022 Jakub Wilk <jwilk@jwilk.net>
 * SPDX-License-Identifier: MIT
 */

#define _GNU_SOURCE

#include <err.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <linux/fs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

static void usage(FILE *fp)
{
    fprintf(fp, "Usage: reflink SRC DST\n");
}

int main(int argc, char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        usage(stdout);
        return EXIT_SUCCESS;
    }
    if (argc != 3) {
        usage(stderr);
        return EXIT_FAILURE;
    }
    const char *src = argv[1];
    int src_fd = open(src, O_RDONLY | O_LARGEFILE);
    if (src_fd < 0)
        err(EXIT_FAILURE, "open(\"%s\", ...)", argv[1]);
    const char *dst = argv[2];
    char *dst2 = strdup(dst);
    if (dst2 == NULL)
        err(EXIT_FAILURE, "strdup()");
    const char *dst_dir = dirname(dst2);
    int dst_fd = open(dst_dir, O_TMPFILE | O_RDWR | O_LARGEFILE, S_IRUSR | S_IWUSR);
    if (dst_fd < 0)
        err(EXIT_FAILURE, "open(\"%s\", O_TMPFILE...)", dst_dir);
    char dst_fd_path[PATH_MAX];
    snprintf(dst_fd_path, sizeof dst_fd_path, "/proc/self/fd/%d", dst_fd);
    int rc = ioctl(dst_fd, FICLONE, src_fd);
    if (rc < 0)
        err(EXIT_FAILURE, "ioctl(..., FICLONE, ...)");
    rc = linkat(AT_FDCWD, dst_fd_path, AT_FDCWD, dst, AT_SYMLINK_FOLLOW);
    if (rc < 0)
        err(EXIT_FAILURE, "linkat(..., \"%s\", ...)", dst);
    return EXIT_SUCCESS;
}

/* vim:set ts=4 sts=4 sw=4 et: */
