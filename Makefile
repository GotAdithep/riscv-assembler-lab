CC=gcc
CFLAGS=-Wall -Wextra -Isrc

SRC=src/main.c src/assembler.c src/parser.c \
    src/registers.c src/instr_table.c \
    src/symbols.c src/encode.c

assembler:
	gcc $(CFLAGS) $(SRC) -o assembler

clean:
	-del assembler
test_registers:
	gcc $(CFLAGS) tests/test_registers.c src/registers.c -o test_registers

test_parser:
	gcc $(CFLAGS) tests/test_parser.c src/parser.c -o test_parser

test_instr_table:
	gcc $(CFLAGS) tests/test_instr_table.c src/instr_table.c -o test_instr_table

test_symbols:
	gcc $(CFLAGS) tests/test_symbols.c src/symbols.c -o test_symbols

test_encode:
	gcc $(CFLAGS) tests/test_encode.c src/encode.c -o test_encode

test_assembler:
	gcc $(CFLAGS) tests/test_assembler.c src/parser.c src/instr_table.c src/symbols.c src/encode.c src/registers.c -o test_assembler

.PHONY: test
test: test_registers test_parser test_instr_table test_symbols test_encode test_assembler
	@echo "All tests built. Run ./test_registers, ./test_parser, ./test_instr_table, ./test_symbols, ./test_encode and ./test_assembler"
