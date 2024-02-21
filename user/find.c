#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void clear_buf(char buf[], int len)
{
    for (int i = 0; i < len; i++)
    {
        buf[i] = '\0';
    }
}

const char*
getname(const char *path)
{
    static char buf[DIRSIZ + 1];
    const char *p;

    clear_buf(buf, DIRSIZ + 1);

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Copy to buf
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    return buf;
}

const char*
fmtname(const char *path)
{
    static char buf[DIRSIZ + 1];
    const char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find_rec(const char path[], const char to_find[])
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        // printf("is it %s?\n", getname(path)); // GOTCHA!
        if (!strcmp(getname(path), to_find))
            printf("%s\n", path); // GOTCHA!
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            
            if (!strcmp(getname(buf), ".") || !strcmp(getname(buf), "..")) { continue; }
            // printf("lets go find_rec(%s, %s);\n", buf, to_find);
            find_rec(buf, to_find);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3) // TODO call find_rec with arguments
    {
        printf("Usage: find <dir> <name>\n");
        exit(0);
    }
    find_rec(argv[1], argv[2]);

    exit(0);
}