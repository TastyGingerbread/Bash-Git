// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close() 


struct termios tty;

int main()
{
	char num[15];
	
	printf("Enter the port address:\n");
	scanf("%s", num);
	while(1)
	{
		
		if(num[0] == 'q')
		{
			return 0;
		}
		printf("%s\n", num);
		int serial_port = open(num, O_RDWR);

		//Check for errors
		if (serial_port < 0)
		{
			printf("Error %i from open: %s\n", errno, strerror(errno));
			printf("The programm was terminated");
			return 0;
		}
	
		// Create new termios struc, we call it 'tty' for convention
		memset(&tty, 0, sizeof tty);

		// Read in existing settings, and handle any error
		if(tcgetattr(serial_port, &tty) != 0) 
		{
			printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
			printf("The programm was terminated");
			return 0;
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
		tty.c_cc[VTIME] = 1;
	
		cfsetispeed(&tty, B9600);
		cfsetospeed(&tty, B9600);
		
		while(1)
		{
			printf("Enter your message or enter '!' to exit:\n");
		
			unsigned char msg[100];
			scanf("%s", msg);
		
			if(msg[0] == '!')
			{
				return 0;
			}
			write(serial_port, msg, sizeof(msg));
			read(serial_port, msg, sizeof(msg));
			printf("Response:\t%s\n\n", msg);
		}
	}
}
