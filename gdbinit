set disassemble-next-line on
b _start
b start_kernel
b uart_init
target remote : 1235
