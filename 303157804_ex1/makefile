
CC = gcc

# put your server object files here
OBJS1 = mmap_writer.o

# put your client object files here
OBJS2 = mmap_reader.o 

# put your server object files here
OBJS3 = fifo_writer.o

# put your client object files here
OBJS4 = fifo_reader.o 

# The executabel filename of the server app
EXEC1 = mmap_writer

# The executabel filename of the client app
EXEC2 = mmap_reader

# The executabel filename of the server app
EXEC3 = fifo_writer

# The executabel filename of the client app
EXEC4 = fifo_reader

#C_COMP_FLAG = -Wall -g  -DNDEBUG 

#C_COMP_FLAG = -std=c99 -Wall -Wextra \
#-Werror -pedantic-errors -DNDEBUG

all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4)

mmap_writer.o:   
		$(CC) $(C_COMP_FLAG)  -c $*.c

mmap_reader.o:   
		$(CC) $(C_COMP_FLAG)  -c $*.c

fifo_writer.o:   
		$(CC) $(C_COMP_FLAG)  -c $*.c

fifo_reader.o:   
		$(CC) $(C_COMP_FLAG)  -c $*.c

$(EXEC1): $(OBJS1)
		$(CC) $(OBJS1) -o $@
		
$(EXEC2): $(OBJS2)
		$(CC) $(OBJS2) -o $@

$(EXEC3): $(OBJS3)
		$(CC) $(OBJS3) -o $@
		
$(EXEC4): $(OBJS4)
		$(CC) $(OBJS4) -o $@

# a rule for building a simple c souorce file


clean:
		rm -f $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4)

kill:
		pkill $(EXEC2)
		pkill $(EXEC4)
		sudo rm ./tmp/osfifo		

fifo: 
	    ./fifo_reader & ./fifo_writer 40000000 

