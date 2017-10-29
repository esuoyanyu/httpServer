TARGET := httpServer 
CC     := gcc  
#-DDEBUG  
CFLAG  := -Wall -DRELEASE -DSYSLOG
LDFLAG := -pthread
INCLUDES   := -I.    

SRCS   =  misc.c sysConfig.c deal.c dealBase.c httpStringOper.c queueBase.c socketBase.c epollBase.c workQueue.c pthreadBase.c test.c  main.c

OBJS    = $(SRCS:.c=.o)    

.PHONY:all
all:$(OBJS)   

	$(CC) $(LDFLAG)  $^  -o $(TARGET)
	cp $(TARGET) ../bin

%.o : %.c   

	$(CC) -c $(INCLUDES) $(CFLAG) $(CPPFLAG) $< -o $@  

clean:
	rm -rf $(OBJS) $(TARGET) ../bin/$(TARGET)
