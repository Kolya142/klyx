#include <thirdparty/printf.h>
#include <klyx/tty.h>
#define TTYS_CAP 4

tty_t ttys[TTYS_CAP];

idx_t current_tty_displ;
idx_t current_tty;

size_t tty_write(idx_t td, const char *buf, size_t count) {
    if (td >= TTYS_CAP) {
        errno = EFAULT;
        return 0;
    }
    if (ttys[td].write == NULL) {
        errno = ENOSYS;
        return 0;
    }
    errno = ENO;
    return ttys[td].write(td, buf, count);
}
size_t tty_read(idx_t td, char *buf, size_t count) {
    if (td >= TTYS_CAP) {
        errno = EFAULT;
        return 0;
    }
    if (ttys[td].read == NULL) {
        errno = ENOSYS;
        return 0;
    }
    size_t text_cur = 0;
    int end_of_loop = 0;
    for(;;) {
        errno = ENO;
        tty_char res = ttys[td].read(td);
        if (errno == EAGAIN) continue;
        switch (res) {
        case '\n':
            ttys[td].write(td, (char[]) {res}, 1);
            /* fall-through */
        case TTY_CHAR_EOF:
            end_of_loop = 1;
            break;
        case 0:
            break;
        case '\b':
            if (text_cur!=0) {
                --text_cur;
                ttys[td].write(td, "\b \b", 3);
            }
            break;
        default:
            if (text_cur<count) {
                buf[text_cur++] = res;
                ttys[td].write(td, (char[]) {res}, 1);
            }
            break;
        }
        if (end_of_loop) break;
    }
    return text_cur;
}
uint32_t tty_getcur(idx_t td) {
    if (td >= TTYS_CAP) {
        errno = EFAULT;
        return 0;
    }
    if (ttys[td].read == NULL) {
        errno = ENOSYS;
        return 0;
    }
    return ttys[td].getcur(td);
}
void tty_setcur(idx_t td, uint32_t cur) {
    if (td >= TTYS_CAP) {
        errno = EFAULT;
        return;
    }
    if (ttys[td].read == NULL) {
        errno = ENOSYS;
        return;
    }
    ttys[td].setcur(td, cur);
}
