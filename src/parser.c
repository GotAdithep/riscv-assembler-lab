#include "parser.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

static void str_trim(char *s)
{
    if (!s)
        return;
    /* trim leading */
    char *p = s;
    while (isspace((unsigned char)*p))
        p++;
    if (p != s)
        memmove(s, p, strlen(p) + 1);
    /* trim trailing */
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1]))
        s[--len] = '\0';
}

int parse_line(const char *line,
               char *label_out, size_t label_sz,
               char *mnemonic_out, size_t mnemonic_sz,
               char *operands_out, size_t operands_sz)
{
    if (!line)
        return -1;

    char buf[1024];
    buf[0] = '\0';
    strncpy(buf, line, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    /* strip comments: //, #, ; */
    char *cpos = strstr(buf, "//");
    if (!cpos)
    {
        char *p1 = strchr(buf, '#');
        char *p2 = strchr(buf, ';');
        if (p1 && (!p2 || p1 < p2))
            cpos = p1;
        else if (p2)
            cpos = p2;
    }
    if (cpos)
        *cpos = '\0';

    str_trim(buf);
    if (buf[0] == '\0')
        return 1; /* blank/comment */

    char *p = buf;

    /* detect label at start: token immediately followed by ':' */
    char *tok_end = p;
    while (*tok_end && !isspace((unsigned char)*tok_end) && *tok_end != ':')
        tok_end++;
    if (*tok_end == ':')
    {
        size_t lablen = (size_t)(tok_end - p);
        if (label_out)
        {
            if (lablen >= label_sz)
                return -1;
            memcpy(label_out, p, lablen);
            label_out[lablen] = '\0';
        }
        p = tok_end + 1;
    }
    else
    {
        if (label_out && label_sz > 0)
            label_out[0] = '\0';
    }

    /* skip whitespace before mnemonic */
    while (*p && isspace((unsigned char)*p))
        p++;
    if (*p == '\0')
    {
        /* only a label on the line */
        if (mnemonic_out && mnemonic_sz > 0)
            mnemonic_out[0] = '\0';
        if (operands_out && operands_sz > 0)
            operands_out[0] = '\0';
        return 0;
    }

    /* read mnemonic */
    char *mstart = p;
    while (*p && !isspace((unsigned char)*p))
        p++;
    size_t mlen = (size_t)(p - mstart);
    if (mnemonic_out)
    {
        if (mlen >= mnemonic_sz)
            return -1;
        memcpy(mnemonic_out, mstart, mlen);
        mnemonic_out[mlen] = '\0';
    }

    /* rest is operands */
    while (*p && isspace((unsigned char)*p))
        p++;
    if (operands_out)
    {
        char tmp[1024];
        strncpy(tmp, p, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';
        str_trim(tmp);
        size_t oplen = strlen(tmp);
        if (oplen >= operands_sz)
            return -1;
        memcpy(operands_out, tmp, oplen + 1);
    }

    return 0;
}
