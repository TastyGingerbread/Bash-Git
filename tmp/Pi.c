// C library headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close() 


int main()
{
	int serial_port;
	char num[15] = {'/', 'd', 'e', 'v', '/', 't', 't', 'y'};
	while(1)
	{
		printf("Enter a port (w/o /dev/tty):\t");
		scanf("%s", &(num[0]) + 8);
		printf("\n%s\n", num);
		serial_port = open(num, O_RDWR);

		//Check for errors
		if (serial_port < 0)
		{
			printf("Error %i from open: %s\n", errno, strerror(errno));
			printf("The programm was terminated\n");
			break;
		}
		
		struct termios tty;
		
		// Create new termios struct, we call it 'tty' for convention
		memset(&tty, 0, sizeof (tty));

		// Read in existing settings, and handle any error
		if(tcgetattr(serial_port, &tty) != 0) 
		{
			printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
			printf("The programm was terminated\n");
			break;
		}
	
		tty.c_cflag &= ~PARENB;
		tty.c_cflag &= ~CSTOPB;
		tty.c_cflag |= CS8;
		tty.c_cflag &= ~CRTSCTS;
		tty.c_cflag |= CREAD | CLOCAL;
		tty.c_lflag &= ~ICANON;
		tty.c_lflag &= ~ECHO; // Disable echo
		tty.c_lflag &= ~ECHOE; // Disable erasure
		tty.c_lflag &= ~ECHONL; // Disable new-line echo
		tty.c_lflag &= ~ISIG; 
		tty.c_iflag &= ~(IXON | IXOFF | IXANY);
		tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
		tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
		tty.c_oflag &= ~ONLCR;
		tty.c_cc[VMIN] = 0;
		tty.c_cc[VTIME] = 30;
	
		cfsetispeed(&tty, B9600);
		cfsetospeed(&tty, B9600);
		
		// Save tty settings, also checking for error
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
	{
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}	
		
		char* resp = malloc(8);
		for(char i = 'a'; i < 'z'; i++)
		{
			printf("Request:\t%c", i);
			write(serial_port, &i,  8);
			read(serial_port, resp, 8);
			printf("\nResponse:\t%c\n\n", *resp);
		}
		
		while(*resp != 'y')
		{
			read(serial_port, resp, 8);
			printf("\nResponse:\t%c\n\n", *resp);
		}
		
		scanf("%c", &resp);
		/*while(1)
		{
			printf("Enter your message or enter '!' to exit:\t");
		
			char* request;
			request = (char*)malloc(8);
			
			char* response;
			response = (char*)malloc(8);
			
			scanf("%c", request);
		
			if(request[0] == '!')
			{
				close(serial_port);
				return 0;
			}
			write(serial_port, request, sizeof(request) * 8);
			read(serial_port, response, sizeof(response) * 8);
			printf("Response:\t%s\n\n", response);
			free(request);
			free(response);
		}*/
	}
	
	close(serial_port);
}
