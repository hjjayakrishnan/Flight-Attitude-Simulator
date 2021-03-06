/**********************************************************************************************************
* Copyright (C) 2017 by JayaKrishnan HJ
*
*Redistribution,modification or use of this software in source or binary fors is permitted as long 
*as the files maintain this copyright. JayaKrishnan HJ is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file communication.c
* @brief all the functions to communicate with Tiva through UART
*
*This file has the functions to read and write data via UART
*
* @author Mounika Reddy Edula
* @       JayaKrishnan HJ
* @date December 11 2017
* @version 1.0
*
*/

#include "communication.h"

//mutex lock so that the i2c functions are nor pre- empted
static pthread_mutex_t i2c_mutex = PTHREAD_MUTEX_INITIALIZER;

//Configure UART
int8_t tty_config(struct termios *con, int descriptor){
    
    pthread_mutex_lock(&i2c_mutex);
    tcgetattr(descriptor, con);
	con->c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
	con->c_oflag = 0;
	con->c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
	con->c_cc[VMIN] = 1;
	con->c_cc[VTIME] = 0;
	
	if(cfsetispeed(con, B115200)||cfsetospeed(con, B115200)){
	    
		perror("ERROR in baud set\n");
		return -1;
	}
	
	
	if(tcsetattr(descriptor, TCSAFLUSH, con) < 0){
	    
		perror("ERROR in set attr\n");
		return -1;
	}
	
	pthread_mutex_unlock(&i2c_mutex);
    pthread_mutex_destroy(&i2c_mutex);
 	
	return 0;
    
}


//read one byte 
char read_one_byte(int fd){
    
    int i = 0;
    char c;
    while(1)
	{
		if(read(fd, &c, 1) > 0)
		{
			
			fflush(stdout);
			++i;
			
		}
		if(i>0)
			break;
		
	}
	return c;
}

// read multiple bytes till newline
int8_t read_bytes(int fd, char *str){
	
	pthread_mutex_lock(&i2c_mutex);
    
    int i = 0;
    while(1){
    
        *(str+i) = read_one_byte(fd);
        //printf("%c\n", *(str+i));
        if(*(str+i) == '\n'){
        	*(str+i) = '\0';
        	break;
        }
        	
        ++i;
    }
    pthread_mutex_unlock(&i2c_mutex);
    pthread_mutex_destroy(&i2c_mutex);
    
    return 0;
    
}


//send multiple bytes 
int8_t transfer_bytes(int fd, char *str){
	
	pthread_mutex_lock(&i2c_mutex);
	
	write(fd, str, strlen(str)+1);
	
	pthread_mutex_unlock(&i2c_mutex);
    pthread_mutex_destroy(&i2c_mutex);
    
	return 0;
}

