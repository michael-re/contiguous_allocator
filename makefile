all:
	gcc -Wall -Wextra -Wno-sign-compare src/*.c -o allocator.out

clean:
	rm -rf allocator.out
