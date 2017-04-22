# Sample code to illustrate writing a greeting to console
# using only syscalls on 64-bit Linux.
#
#     gcc -nostdlib -o hello64 helloi64.s
# ----------------------------------------------------------------------------------------
        .global _start
.text
_start:
        # write(1, message, len)
        mov     $1, %rax                # system call 1 is write
        mov     $1, %rdi                # file handle 1 is stdout
        mov     $message, %rsi          # address of string to output
        mov     $len, %rdx              # number of bytes
        syscall                         # invoke operating system to do the write

        # exit(64)
        mov     $60, %rax               # system call 60 is exit
        mov	$64, %rdi               # we want return code 3
        syscall                         # invoke operating system to exit
.data
message:
        .ascii  "Namaste!\n"
	len = . - message		# store length here
