#include <stdio.h>
#include <unistd.h>
#include<linux/input.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#define MOUSEFILE "dev/input/event4"

void IOSetup(void)
{
    int FdExport, Fd10, Fd26, Fd74;  //Defining the various variables for accessing the gpio directory //
    int Fd4,Fd22,Fd70;
    int Fd5,Fd24,Fd44;
    int Fd11,Fd32;
    int Fd12,Fd28,Fd45;
    int Fd14,Fd16,Fd76;
    int Fd13,Fd34,Fd77;
    int Fd6,Fd36;
    int Fd0,Fd18,Fd66;
    int Fd1,Fd20,Fd68;
    int Fd38;
    int Fd40;
    int Fd15,Fd42;
    int Fd7,Fd30,Fd46;

        
// * Here I have manually exported the respective gpio directory as well as the set_direction and set_value of these gpios. I haven't made use of the predefined functions and have effectively exported and unexported the gpio * //

FdExport = open("/sys/class/gpio/export", O_WRONLY);
		
  //For IO0 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"11",3))
			printf("error FdExport 10");
		if(0> write(FdExport,"32",2))
			printf("error FdExport 32");
		

  //For IO1 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"12",3))
			printf("error FdExport 12");
		if(0> write(FdExport,"28",2))
			printf("error FdExport 28");
		if(0> write(FdExport,"45",2))
			printf("error FdExport 45");
//For IO2 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"13",3))
			printf("error FdExport 13");
		if(0> write(FdExport,"34",2))
			printf("error FdExport 34");
		if(0> write(FdExport,"77",2))
			printf("error FdExport 77");
//For IO3 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"14",3))
			printf("error FdExport 14");
		if(0> write(FdExport,"16",2))
			printf("error FdExport 16");
		if(0> write(FdExport,"76",2))
			printf("error FdExport 76");
//For IO4 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"6",3))
			printf("error FdExport 6");
		if(0> write(FdExport,"36",2))
			printf("error FdExport 36");
		
//For IO5 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"0",3))
			printf("error FdExport 0");
		if(0> write(FdExport,"18",2))
			printf("error FdExport 18");
		if(0> write(FdExport,"66",2))
			printf("error FdExport 66");
//For IO6 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"1",3))
			printf("error FdExport 1");
		if(0> write(FdExport,"20",2))
			printf("error FdExport 20");
		if(0> write(FdExport,"68",2))
			printf("error FdExport 68");
//For IO7 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"38",3))
			printf("error FdExport 38");
//For IO8 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"40",2))
			printf("error FdExport 40");
		
		


//For IO9 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"4",2))
			printf("error FdExport 4");
		if(0> write(FdExport,"22",2))
			printf("error FdExport 22");
		if(0> write(FdExport,"70",2))
			printf("error FdExport 70");
//For IO10
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"10",3))
			printf("error FdExport 10");
		if(0> write(FdExport,"26",2))
			printf("error FdExport 26");
		if(0> write(FdExport,"74",2))
			printf("error FdExport 74");


		close(FdExport);
//For IO11 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"5",2))
			printf("error FdExport 5");
		if(0> write(FdExport,"24",2))
			printf("error FdExport 24");
		if(0> write(FdExport,"44",2))
			printf("error FdExport 44");
//For IO12 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"15",2))
			printf("error FdExport 15");
		if(0> write(FdExport,"42",2))
			printf("error FdExport 42");
	

//For IO13 //
if (FdExport < 0)
		{
			printf("\n gpio export open failed");
		}

		if(0> write(FdExport,"7",2))
			printf("error FdExport 7");
		if(0> write(FdExport,"30",2))
			printf("error FdExport 30");
		if(0> write(FdExport,"46",2))
			printf("error FdExport 46");



 /* Initialize all GPIOs */
	//For Io0 //
        Fd11 = open("/sys/class/gpio/gpio11/direction", O_WRONLY);
		if (Fd11 < 0)
		{
			printf("\n gpio11 direction open failed");
		}
		
		Fd32 = open("/sys/class/gpio/gpio32/direction", O_WRONLY);
		if (Fd32 < 0)
		{
			printf("\n gpio32 direction open failed");
		}

		

		if(0> write(Fd11,"out",3))
			printf("error Fd11");

		if(0> write(Fd32,"out",3))
			printf("error Fd32");

		

	     Fd32 = open("/sys/class/gpio/gpio32/value", O_WRONLY); 
                 if (Fd32 < 0) {
		printf("\n gpio32 value open failed"); } 
	


		if(0> write(Fd32,"0",1))
		printf("error Fd32 value");

 
	//For Io1 //
        Fd12 = open("/sys/class/gpio/gpio12/direction", O_WRONLY);
		if (Fd12 < 0)
		{
			printf("\n gpio12 direction open failed");
		}
		
		Fd28 = open("/sys/class/gpio/gpio28/direction", O_WRONLY);
		if (Fd28 < 0)
		{
			printf("\n gpio28 direction open failed");
		}

		Fd45 = open("/sys/class/gpio/gpio45/direction", O_WRONLY);
		if (Fd45< 0)
		{
			printf("\n gpio45 direction open failed");
		}


		if(0> write(Fd12,"out",3))
			printf("error Fd12");

		if(0> write(Fd28,"out",3))
			printf("error Fd28");

		if(0> write(Fd45,"out",3))
			printf("error Fd45");
        Fd28 = open("/sys/class/gpio/gpio28/value", O_WRONLY); 
                 if (Fd28 < 0) {
		printf("\n gpio28 value open failed"); } 
		Fd45 =open("/sys/class/gpio/gpio45/value", O_WRONLY); 
                if (Fd45 < 0) {
		printf("\n gpio45 value open failed"); 
	}

		if(0> write(Fd28,"0",1))
		printf("error Fd28 value");

		if(0> write(Fd45,"0",1))
		printf("error Fd45 value");

	//For Io2 //
        Fd13 = open("/sys/class/gpio/gpio13/direction", O_WRONLY);
		if (Fd13 < 0)
		{
			printf("\n gpio13 direction open failed");
		}

		Fd34 = open("/sys/class/gpio/gpio34/direction", O_WRONLY);
		if (Fd34 < 0)
		{
			printf("\n gpio34 direction open failed");
		}

		Fd77= open("/sys/class/gpio/gpio77/direction", O_WRONLY);
		if (Fd77 < 0)
		{
			printf("\n gpio77 direction open failed");
		}


		if(0> write(Fd13,"out",3))
			printf("error Fd13");

		if(0> write(Fd34,"out",3))
			printf("error Fd34");

		if(0> write(Fd77,"out",3))
			printf("error Fd77");

		Fd34 = open("/sys/class/gpio/gpio34/value", O_WRONLY); 
                 if (Fd34 < 0) {
		printf("\n gpio34 value open failed"); } 
		Fd77 =open("/sys/class/gpio/gpio77/value", O_WRONLY); 
                if (Fd77 < 0) {
		printf("\n gpio77 value open failed"); 
	}

		if(0> write(Fd34,"0",1))
		printf("error Fd34 value");

		if(0> write(Fd77,"0",1))
		printf("error Fd77 value");

	//For Io3 //
        Fd14 = open("/sys/class/gpio/gpio14/direction", O_WRONLY);
		if (Fd14 < 0)
		{
			printf("\n gpio14 direction open failed");
		}
		
		Fd16 = open("/sys/class/gpio/gpio16/direction", O_WRONLY);
		if (Fd16 < 0)
		{
			printf("\n gpio16 direction open failed");
		}

		Fd76 = open("/sys/class/gpio/gpio76/direction", O_WRONLY);
		if (Fd76 < 0)
		{
			printf("\n gpio76 direction open failed");
		}


		if(0> write(Fd14,"out",3))
			printf("error Fd14");

		if(0> write(Fd16,"out",3))
			printf("error Fd16");

		if(0> write(Fd76,"out",3))
			printf("error Fd76");

		Fd16 = open("/sys/class/gpio/gpio16/value", O_WRONLY); 
                 if (Fd16 < 0) {
		printf("\n gpio16 value open failed"); }

		Fd76 =open("/sys/class/gpio/gpio76/value", O_WRONLY); 
                if (Fd76 < 0) {
		printf("\n gpio76 value open failed"); 
	}

		if(0> write(Fd16,"0",1))
		printf("error Fd16 value");

		if(0> write(Fd76,"0",1))
		printf("error Fd76 value");

	  //For Io4 //
        Fd6 = open("/sys/class/gpio/gpio6/direction", O_WRONLY);
		if (Fd6 < 0)
		{
			printf("\n gpio6 direction open failed");
		}
	
		Fd36 = open("/sys/class/gpio/gpio36/direction", O_WRONLY);
		if (Fd36 < 0)
		{
			printf("\n gpio36 direction open failed");
		}


		if(0> write(Fd6,"out",3))
			printf("error Fd6");

		if(0> write(Fd36,"out",3))
			printf("error Fd36");

	     Fd36 = open("/sys/class/gpio/gpio36/value", O_WRONLY); 
        if (Fd36 < 0) {
		printf("\n gpio36 value open failed"); } 
		if(0> write(Fd36,"0",1))
		printf("error Fd36 value");


	//For Io5 //
        	Fd0 = open("/sys/class/gpio/gpio0/direction", O_WRONLY);
		if (Fd0 < 0)
		{
			printf("\n gpio0 direction open failed");
		}
		
		Fd18 = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
		if (Fd18 < 0)
		{
			printf("\n gpio18 direction open failed");
		}

		Fd66 = open("/sys/class/gpio/gpio66/direction", O_WRONLY);
		if (Fd66 < 0)
		{
			printf("\n gpio66 direction open failed");
		}


		if(0> write(Fd0,"out",3))
			printf("error Fd0");

		if(0> write(Fd18,"out",3))
			printf("error Fd18");

		if(0> write(Fd66,"out",3))
			printf("error Fd66");

		Fd18 = open("/sys/class/gpio/gpio18/value", O_WRONLY); 
                 if (Fd18 < 0) {
		printf("\n gpio18 value open failed"); } 
		Fd66 =open("/sys/class/gpio/gpio66/value", O_WRONLY); 
                if (Fd66 < 0) {
		printf("\n gpio66 value open failed"); 
	}

		if(0> write(Fd18,"0",1))
		printf("error Fd18 value");

		if(0> write(Fd66,"0",1))
		printf("error Fd66 value");

	//For Io6 //
        	Fd1 = open("/sys/class/gpio/gpio1/direction", O_WRONLY);
		if (Fd1 < 0)
		{
			printf("\n gpio1 direction open failed");
		}
		Fd20 = open("/sys/class/gpio/gpio20/direction", O_WRONLY);
		if (Fd20 < 0)
		{
			printf("\n gpio20 direction open failed");
		}

		Fd68 = open("/sys/class/gpio/gpio68/direction", O_WRONLY);
		if (Fd68 < 0)
		{
			printf("\n gpio68 direction open failed");
		}


		if(0> write(Fd1,"out",3))
			printf("error Fd1");

		if(0> write(Fd20,"out",3))
			printf("error Fd20");

		if(0> write(Fd68,"out",3))
			printf("error Fd68");

		Fd20 = open("/sys/class/gpio/gpio20/value", O_WRONLY); 
                 if (Fd20 < 0) {
		printf("\n gpio20 value open failed"); } 
		Fd68 =open("/sys/class/gpio/gpio68/value", O_WRONLY); 
                if (Fd68 < 0) {
		printf("\n gpio68 value open failed"); 
	}

		if(0> write(Fd20,"0",1))
		printf("error Fd20 value");

		if(0> write(Fd68,"0",1))
		printf("error Fd68 value");

	//For Io7 //
        	Fd38 = open("/sys/class/gpio/gpio38/direction", O_WRONLY);
		if (Fd38 < 0)
		{
			printf("\n gpio38 direction open failed");
		}

		

		if(0> write(Fd38,"out",3))
			printf("error Fd38");

	

	
 
	//For Io8 //
        	Fd40 = open("/sys/class/gpio/gpio40/direction", O_WRONLY);
		if (Fd40 < 0)
		{
			printf("\n gpio40 direction open failed");
		}
		


		if(0> write(Fd40,"out",3))
			printf("error Fd40");

		
//For Io11 //
		Fd5 = open("/sys/class/gpio/gpio5/direction", O_WRONLY);
		if (Fd5 < 0)
		{
			printf("\n gpio5 direction open failed");
		}
		
		Fd24 = open("/sys/class/gpio/gpio24/direction", O_WRONLY);
		if (Fd24 < 0)
		{
			printf("\n gpio24 direction open failed");
		}

		Fd44 = open("/sys/class/gpio/gpio44/direction", O_WRONLY);
		if (Fd44 < 0)
		{
			printf("\n gpio44 direction open failed");
		}


		if(0> write(Fd5,"out",3))
			printf("error Fd5");

		if(0> write(Fd24,"out",3))
			printf("error Fd24");

		if(0> write(Fd44,"out",3))
			printf("error Fd44");

		Fd24 = open("/sys/class/gpio/gpio24/value", O_WRONLY);
		if (Fd24 < 0)
				{
					printf("\n gpio24 value open failed");
				}
		Fd44 = open("/sys/class/gpio/gpio44/value", O_WRONLY);
		if (Fd74 < 0)
				{
					printf("\n gpio44 value open failed");
				}

		if(0> write(Fd24,"0",1))
					printf("error Fd24 value");

		if(0> write(Fd44,"0",1))
					printf("error Fd44 value");


	//For Io10 //
        	Fd10 = open("/sys/class/gpio/gpio10/direction", O_WRONLY);
		if (Fd10 < 0)
		{
			printf("\n gpio10 direction open failed");
		}
		
		Fd26 = open("/sys/class/gpio/gpio26/direction", O_WRONLY);
		if (Fd26 < 0)
		{
			printf("\n gpio26 direction open failed");
		}

		Fd74 = open("/sys/class/gpio/gpio74/direction", O_WRONLY);
		if (Fd74 < 0)
		{
			printf("\n gpio74 direction open failed");
		}


		if(0> write(Fd10,"out",3))
			printf("error Fd10");

		if(0> write(Fd26,"out",3))
			printf("error Fd26");

		if(0> write(Fd74,"out",3))
			printf("error Fd74");

		Fd26 = open("/sys/class/gpio/gpio26/value", O_WRONLY); 
                 if (Fd26 < 0) {
		printf("\n gpio26 value open failed"); } 
		Fd74 =open("/sys/class/gpio/gpio74/value", O_WRONLY); 
                if (Fd74 < 0) {
		printf("\n gpio74 value open failed"); 
	}

		if(0> write(Fd26,"0",1))
		printf("error Fd26 value");

		if(0> write(Fd74,"0",1))
		printf("error Fd74 value");


			
//For Io9//
		Fd4 = open("/sys/class/gpio/gpio4/direction", O_WRONLY);
		if (Fd4 < 0)
		{
			printf("\n gpio4 direction open failed");
		}
		
		Fd22 = open("/sys/class/gpio/gpio22/direction", O_WRONLY);
		if (Fd22 < 0)
		{
			printf("\n gpio22 direction open failed");
		}

		Fd70 = open("/sys/class/gpio/gpio70/direction", O_WRONLY);
		if (Fd70 < 0)
		{
			printf("\n gpio70 direction open failed");
		}


		if(0> write(Fd4,"out",3))
			printf("error Fd38");

		if(0> write(Fd22,"out",3))
			printf("error Fd22");

		if(0> write(Fd70,"out",3))
			printf("error Fd70");

		Fd22 = open("/sys/class/gpio/gpio22/value", O_WRONLY);
		if (Fd22 < 0)
				{
					printf("\n gpio22 value open failed");
				}
		Fd70 = open("/sys/class/gpio/gpio70/value", O_WRONLY);
		if (Fd70 < 0)
				{
					printf("\n gpio70 value open failed");
				}

		if(0> write(Fd22,"0",1))
					printf("error Fd22 value");

		if(0> write(Fd70,"0",1))
					printf("error Fd70 value");



	//For Io12 //
        	Fd15 = open("/sys/class/gpio/gpio15/direction", O_WRONLY);
		if (Fd15 < 0)
		{
			printf("\n gpio15 direction open failed");
		}
		Fd42 = open("/sys/class/gpio/gpio42/direction", O_WRONLY);
		if (Fd42< 0)
		{
			printf("\n gpio42 direction open failed");
		}

		
	

		if(0> write(Fd15,"out",3))
			printf("error Fd7");

		if(0> write(Fd42,"out",3))
			printf("error Fd42");


		Fd42 = open("/sys/class/gpio/gpio42/value", O_WRONLY); 
                 if (Fd42 < 0) {
		printf("\n gpio42 value open failed"); } 
		

		if(0> write(Fd42,"0",1))
		{
		printf("error Fd42 value");
	}



	//For Io13 //
        Fd7 = open("/sys/class/gpio/gpio7/direction", O_WRONLY);
		if (Fd7 < 0)
		{
			printf("\n gpio7 direction open failed");
		}

		Fd30 = open("/sys/class/gpio/gpio30/direction", O_WRONLY);
		if (Fd30< 0)
		{
			printf("\n gpio30 direction open failed");
		}

		Fd46 = open("/sys/class/gpio/gpio46/direction", O_WRONLY);
		if (Fd46 < 0)
		{
			printf("\n gpio46 direction open failed");
		}


		if(0> write(Fd7,"out",3))
			printf("error Fd7");

		if(0> write(Fd30,"out",3))
			printf("error Fd30");

		if(0> write(Fd46,"out",3))
			printf("error Fd46");

		Fd30 = open("/sys/class/gpio/gpio30/value", O_WRONLY); 
                 if (Fd30 < 0) {
                 	printf("\n gpio30 value open failed");
                 	 } 
		Fd46 =open("/sys/class/gpio/gpio46/value", O_WRONLY); 
                if (Fd46 < 0) {
		printf("\n gpio46 value open failed"); 
	}

		if(0> write(Fd30,"0",1)) {
		printf("error Fd30 value");}

		if(0> write(Fd46,"0",1)){
		printf("error Fd46 value");
}

		
}


int main()
{

int filed;     //this is the filedscriptor to open the mouse event //
int flag=0;        //this variable is assigned to check the termination of the program upon detecting the mouse click //
int  sq;            // variable to run the loop inside the LED open //
int dc,p0,p1,p2;        //dc is the percentage of the PWM duty cycle //
int total_cycle_duration=2000;    
int no_of_loops=25;
struct input_event ieve; //this is defined to indicate the mouse event //
int on_time;          //this is esssentially the time in milliseconds //
printf("Please enter the duty cycle of PWM (percent) and the pin numbers where the LEDs are connected \n");
scanf("%d %d %d %d",&dc,&p0,&p1,&p2);  
int LedR,LedG,LedB;   

IOSetup(); //calling the IOsetup function //

//After getting the user input about which pins the R, G and B should be assigned to, we open the value for that particular gpio //
      if(p0=0)    {LedR = open("/sys/class/gpio/gpio11/value", O_WRONLY);  
}else if(p0=1)   {LedR = open("/sys/class/gpio/gpio12/value", O_WRONLY);
}else if(p0=2)   {LedR = open("/sys/class/gpio/gpio13/value", O_WRONLY);
}else if(p0=3)   {LedR = open("/sys/class/gpio/gpio14/value", O_WRONLY);
}else if(p0=4)   {LedR = open("/sys/class/gpio/gpio6/value", O_WRONLY);
}else if(p0=5)   {LedR = open("/sys/class/gpio/gpio0/value", O_WRONLY);
}else if(p0=6)   {LedR = open("/sys/class/gpio/gpio1/value", O_WRONLY);
}else if(p0=7)   {LedR = open("/sys/class/gpio/gpio38/value", O_WRONLY);
}else if(p0=8)   {LedR = open("/sys/class/gpio/gpio40/value", O_WRONLY);
}else if(p0=9)   {LedR = open("/sys/class/gpio/gpio4/value", O_WRONLY);
}else if(p0=10)  {LedR = open("/sys/class/gpio/gpio10/value", O_WRONLY);
}else if(p0=11)  {LedR = open("/sys/class/gpio/gpio5/value", O_WRONLY);
}else if(p0=12)  {LedR = open("/sys/class/gpio/gpio15/value", O_WRONLY);
}else if(p0=13)  {LedR = open("/sys/class/gpio/gpio7/value", O_WRONLY);
}
if(p1=0)        {LedG = open("/sys/class/gpio/gpio11/value", O_WRONLY);
}else if(p1=1)  {LedG = open("/sys/class/gpio/gpio12/value", O_WRONLY);
}else if(p1=2)  {LedG = open("/sys/class/gpio/gpio13/value", O_WRONLY);
}else if(p1=3)  {LedG = open("/sys/class/gpio/gpio14/value", O_WRONLY);
}else if(p1=4)  {LedG = open("/sys/class/gpio/gpio6/value", O_WRONLY);
}else if(p1=5)  {LedG = open("/sys/class/gpio/gpio0/value", O_WRONLY);
}else if(p1=6)  {LedG = open("/sys/class/gpio/gpio1/value", O_WRONLY);
}else if(p1=7)  {LedG = open("/sys/class/gpio/gpio38/value", O_WRONLY);
}else if(p1=8)  {LedG = open("/sys/class/gpio/gpio40/value", O_WRONLY);
}else if(p1=9)  {LedG = open("/sys/class/gpio/gpio4/value", O_WRONLY);
}else if(p1=10) {LedG = open("/sys/class/gpio/gpio10/value", O_WRONLY);
}else if(p1=11) {LedG = open("/sys/class/gpio/gpio5/value", O_WRONLY);
 }else if(p1=12){LedG = open("/sys/class/gpio/gpio15/value", O_WRONLY);
 }else if(p1=13){LedG = open("/sys/class/gpio/gpio7/value", O_WRONLY);
}
if(p2=0)         {LedB = open("/sys/class/gpio/gpio11/value", O_WRONLY);
}else if(p2=1)   {LedB = open("/sys/class/gpio/gpio12/value", O_WRONLY);
}else if(p2=2)   {LedB = open("/sys/class/gpio/gpio13/value", O_WRONLY);
}else if(p2=3)   {LedB = open("/sys/class/gpio/gpio14/value", O_WRONLY);
}else if(p2=4)   {LedB = open("/sys/class/gpio/gpio6/value", O_WRONLY);
}else if(p2=5)   {LedB = open("/sys/class/gpio/gpio0/value", O_WRONLY);
}else if(p2=6)   {LedB = open("/sys/class/gpio/gpio1/value", O_WRONLY);
}else if(p2=7)   {LedB = open("/sys/class/gpio/gpio38/value", O_WRONLY);
}else if(p2=8)   {LedB = open("/sys/class/gpio/gpio40/value", O_WRONLY);
}else if(p2=9)   {LedB = open("/sys/class/gpio/gpio4/value", O_WRONLY);
}else if(p2=10)  {LedB = open("/sys/class/gpio/gpio10/value", O_WRONLY);
}else if(p2=11)  {LedB = open("/sys/class/gpio/gpio5/value", O_WRONLY);
 }else if(p2=12) {LedB = open("/sys/class/gpio/gpio15/value", O_WRONLY);
 }else if(p2=13) {LedB = open("/sys/class/gpio/gpio7/value", O_WRONLY);
}
on_time=20*dc;    //Since dc will be entered by the user as a percent of PWM, multiplying this by 20 will give us the on time in milliseconds
	// loop forever toggling the LED every second

	for (sq=0;sq<no_of_loops;sq++) {
		if(flag!=1)
 {

		write(LedR,"1",1);
		usleep(on_time);
		write(LedR, "0", 1);
		usleep(total_cycle_duration-on_time);
	}
}

	for (sq=0;sq<no_of_loops;sq++) {
		if(flag!=1)
 {

		write(LedG,"1",1);
		usleep(on_time );
		write(LedG, "0", 1);
		usleep(total_cycle_duration-on_time );
	}
}

	for (sq=0;sq<no_of_loops;sq++) {
		if(flag!=1)
 {

		write(LedB,"1",1);
		usleep(on_time);
		write(LedB, "0", 1);
		usleep(total_cycle_duration-on_time);
	}
}

for (sq=0;sq<no_of_loops;sq++) {
		if(flag!=1)
 {
                write(LedR,"1",1);
		write(LedG,"1",1);
		usleep(on_time);
                write(LedR, "0", 1);
		write(LedG, "0", 1);
		usleep(total_cycle_duration-on_time);
	}
}

for (sq=0;sq<no_of_loops;sq++) {
		if(flag!=1)
 {
        write(LedG,"1",1);
		write(LedB,"1",1);
		usleep(on_time );
        write(LedG, "0", 1);
		write(LedB, "0", 1);
		usleep(total_cycle_duration-on_time);
	}
}

for (sq=0;sq<no_of_loops;sq++) {
		if(flag!=1)
 {
        write(LedR,"1",1);
		write(LedB,"1",1);
		usleep(on_time);
                write(LedR, "0", 1);
		write(LedB, "0", 1);
		usleep(total_cycle_duration-on_time);
	}
}

for (sq=0;sq<no_of_loops;sq++) {
		if(flag!=1)
 {
        write(LedR,"1",1);
		write(LedG,"1",1);
		write(LedB,"1",1);
		usleep(on_time );
        write(LedR, "0", 1);
		write(LedG, "0", 1);
		write(LedB, "0", 1);
		usleep(total_cycle_duration-on_time);
	}
}


// Now we are attempting to access the mouse device //
filed= open (MOUSEFILE, O_RDONLY);

while (read(filed,&ieve,sizeof(struct input_event)) && (flag!=1))
{
if((ieve.code==272 && ieve.value==1)||(ieve.code==273 && ieve.value==1))   //event id 272 is for the left mouse click and 273 is for the right mouse click //
{
printf("Mouse click is detected. The program will terminate now! Goodbye! \n");

flag=1;
}
}
	return 0;

}
