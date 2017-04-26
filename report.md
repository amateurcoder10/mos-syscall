A Report on System Calls

Assignment 3,Modern Operating systems

Submitted by:Prashanthi S.K(MT2016520)

Approach and Observations:

I started off by reading about bare metal programming and cross compilers.When I had done a fair bit of reading,I proceeded to go through namaste.c as well as startup.s and namaste.ld.I also went through the makefile and understood how to run them.My first observation was that UART was declared volatile.So I looked and found out why and where volatile types are used.What struck me was that even though no built-in functions were used,this program could easily achieve the desired objective.Next,I understood and ran the assembly code hello.s.

Before starting to work on gossip.c,I began reading up about termios and line discipline.I came across stty,a command line utility which could be used to modify the line discipline settings for the terminal.I experimented with toggling commands like echo,icanon etc.Keystrokes for special signals like erase and kill could be reconfigured as well.During one of these experiments,I turned off canonical mode and typed a stream of characters followed by Ctrl-U(kill).I expected that the kill command would not be processed.To my surprise,the line got erased.I puzzled over this for quite some time and couldn't find any explanation as to why this was happening.I finally decided that since there was no program reading this character,the terminal would inevitably process the character before displaying it,with the result that it was being interpreted as a kill line signal.I'm not entirely sure this is the right explanation,but when I tried to do the same with a program in non-canonical mode, it worked as expected i.e kill and erase were not interpreted and were passed to the program as is.

Now,I looked up how to implement the same using termios and writing gossip.c was very easy.I created a function called secret,which when called would turn off echo and enter non-canonical mode.After which it would echo * for every character typed and terminate on encountering a newline.Before exiting the function,I restored the line discipline to its original state.I wondered what would happen if the program got terminated while echo was off and contemplated adding a signal handler which would handle SIGINT and gracefully exit after restoring line discipline.When I tried giving a SIGINT while echo was off,the program was terminated.Echo however,unexpectedly worked.Therefore,I did not add a signal handler to the program.I ran the program with strace and observed the various system calls that were used by the program.Using gdb and an appropriate breakpoint,I looked at the local flags before and after echo toggle.All of this is recorded in the log file.

Before answering the questions,I also ran a small program which iterated through and printed all the environment variables using *envp.

Answers to questions:

1)In strace output, what does the execve system call do? what is its third argument?

Execve is used to execute the program.It is the first system call when the ./a.out command is run.It takes 3 arguments:the first is the filename,the second is *argv and the third is *envp.The third argument is the pointer to the environment variables which are stored as key value pairs.The argument vector and environment variable can be accessed by the program's main function.

2)In C programming, we normally use only two arguments in main(). There is also a third  argument. What is this argument and what library functions deal with this  third argument?

The third argument is the pointer to environment variables.The program receives information about the context in which it was invoked through these environment variables.They are stored as key-value pairs.Standard environment variables include information about the user's home directory,path etc.Several system calls use these environment variables.For isntance,the exec system call resolves the file to be executed using environment variables like PATH.

3)What is canonical processing mode in TTY line discipline? Why is it useful? When is it not useful?

In the Canonical processing mode,input is buffered.Only when a newline or EOF is encountered,it is fed to the program.This is used to implement functions like ERASE and KILL--basically to process the line before it is given to the program.It is useful in most cases because it allows the user to edit the input.However,in cases like serial communication, the data entered should not be interpreted by the terminal and also needs to be made available to the program for transmission immediately.Also,some programs prefer to do their own editing.In such use cases,canonical mode isn't useful.

4)Did you use canonicalmode or switch it off for your program? Why?

I switched off canonical mode.The reasons being:A continuous echo of the mask character is required as the secret is typed and this is not possible in the canonical mode as the input is buffered and read till newline is encountered.Also,kill and erase processing aren't required here since the program deals with one character at a time.

5)Did you have to use reset(1) command to restore sane settings during your experiments? What settings does this command restore?

Yes.Reset switches on echo,sets the mode to cooked,turns off cbreak and raw modes,enables newline translation and restores the values of special characters to their previous state.

6)What is the command used to control TTY line discipline settings from a shell script? How will you use it to turn off echo while running a program and then restore the original settings after the program ends?

stty -echo //to turn off the echo.
......
//program
......
stty echo //restores echoing.

If any other settings are changed,stty sane could be used at the end.

Conclusion:

This assignment served to demostrate the importance of line discipline which acts as the policy followed by the driver.Also,it provided insight on how to modify inbuilt functions so as to get the desired behavior out of them.

References:

1)Lecture Slides:Part 12,Part 13

2)Man pages:termios(3),stty(1),ioctl(2),tty_ioctl(4),execve(2),reset(1),ascii(7),exec(3)

3)Using ubuntu ARM cross sompiler for bare metal programming
https://balau82.wordpress.com/2010/12/05/using-ubuntu-arm-cross-compiler-for-bare-metal-programming/

4)The TTY demystified
http://www.linusakesson.net/programming/tty/

5)Sample code for non-canonical mode
http://poincare.matf.bg.ac.rs/~ivana/courses/tos/sistemi_knjige/pomocno/apue/APUE/0201433079/ch18lev1sec11.html

6)A brief introduction to Termios
https://blog.nelhage.com/2009/12/a-brief-introduction-to-termios-termios3-and-stty/

7)Serial Programing-termios,Wikibooks
https://en.wikibooks.org/wiki/Serial_Programming/termios

8)Line Discipline,Wikipedia
https://en.wikipedia.org/wiki/Line_discipline

9)Environment Variables Code Sample
http://crasseux.com/books/ctutorial/Environment-variables.html

10)http://stackoverflow.com/questions/358342/canonical-vs-non-canonical-terminal-input

11)https://en.wikipedia.org/wiki/Cooked_mode

12)Environment Variables,GNU Programming Library
https://www.gnu.org/software/libc/manual/html_node/Environment-Variables.html#Environment-Variables

