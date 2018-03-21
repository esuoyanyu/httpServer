TARGET := httpServer  
#-DDEBUG  
CFLAG  := -Wall -DRELEASE -DSYSLOG
LDFLAG := -pthread
INCLUDES   := -I.    

SRCS   =  misc.c sysConfig.c deal.c dealBase.c httpStringOper.c queueBase.c socketBase.c epollBase.c workQueue.c pthreadBase.c test.c  main.c

OBJS    = $(SRCS:.c=.o)    

ta:$(OBJS)   
	$(CC) $(LDFLAG)  $(OBJS)  -o $(TARGET)
	cp $(TARGET) ../bin

%.o : %.c   
	$(CC) -c $(INCLUDES) $(CFLAG) $< -o $@  

clean:
	rm -rf $(OBJS) $(TARGET) ../bin/$(TARGET)
