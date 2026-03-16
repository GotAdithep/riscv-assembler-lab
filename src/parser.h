#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

/* Parse a single assembly source line.
 * Parameters:
 *  - line: input C string (may include trailing newline)
 *  - label_out: buffer to receive optional label (without ':'). May be NULL.
 *  - label_sz: size of label_out buffer.
 *  - mnemonic_out: buffer to receive mnemonic. May be NULL.
 *  - mnemonic_sz: size of mnemonic_out buffer.
 *  - operands_out: buffer to receive the operands string (rest of the line after mnemonic).
 *  - operands_sz: size of operands_out buffer.
 *
 * Returns:
 *  - 0 when a mnemonic was parsed successfully (label_out may be empty if none)
 *  - 1 when the line is blank or a comment
 *  - -1 on error (null pointers or buffer too small)
 */
int parse_line(const char *line,
			   char *label_out, size_t label_sz,
			   char *mnemonic_out, size_t mnemonic_sz,
			   char *operands_out, size_t operands_sz);

#endif /* PARSER_H */

