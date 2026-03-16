#ifndef REGISTERS_H
#define REGISTERS_H

/* Parse a register name like "x0".."x31".
 * Returns the register number 0..31 on success, or -1 on invalid input.
 */
int parse_register(const char *name);

#endif /* REGISTERS_H */

