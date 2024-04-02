/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: template file for digital and     */
/*                analog square wave generation     */
/*                via the LabJack U3-LV USB DAQ     */
/*                                                  */
/****************************************************/

#include "u3.h"
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>




#define MAX_VOLTAGE_STR \
	"Provide max voltage between 0 and 5V: "

#define MIN_VOLTAGE_STR \
	"Provide min voltage between 0 and 5V: "

#define INVALID_VOLTAGE_STR      \
	"Invalid voltage value!!!\n" \
	"Provide voltage between 0 and 5V: "

#define FREQUENCY_STR \
	"Provide square wave frequency in Hz(between 0 and 1000Hz): "


u3CalibrationInfo *caliInfo;
HANDLE hDevice;
int analog_flag, digital_flag;
static int signal_value = 0;
double max_voltage, min_voltage = 0;
int voltage_flag = 0;


/* This function should initialize the DAQ and return a device
 * handle. The function takes as a parameter the calibratrion info to
 * be filled up with what obtained from the device. */
HANDLE init_DAQ(u3CalibrationInfo *caliInfo)
{
	HANDLE hDevice;
	int localID;

	/* Invoke openUSBConnection function here */
	localID = -1;

	hDevice = openUSBConnection(localID);

	/* Invoke getCalibrationInfo function here */
	getCalibrationInfo(hDevice, caliInfo);

	return hDevice;
}

void handle_rt_signal(int signo, siginfo_t *info, void *extra)
{
	//printf("rt signal handling\n");
	if(signo==SIGRTMAX){
		//printf("Analog signal\n");
		double voltage = (voltage_flag == 0) ? max_voltage : min_voltage;
		voltage_flag = (voltage_flag == 1) ? 0 : 1;
		eDAC(hDevice, caliInfo, 1, 0, voltage, 0, 0, 0);
	} else if (signo==SIGRTMIN){
		//printf("Digital signal\n");
		eDO(hDevice, 1, 2, signal_value); // output, FIO2, high
		signal_value = (signal_value == 0) ? 1 : 0;
	} else{
		printf("default rt signal\n");
	}
	
}

int main(int argc, char **argv)
{
	// init variables
	struct timespec max_fq;
	double frequency;

	struct sigaction sa;
	sigset_t mask, wait_mask;
	struct sigevent ev1, ev2;
	struct itimerspec timer_spec;
	timer_t timer1, timer2;
	
	
	/* Invoke init_DAQ and handle errors if needed */
	caliInfo = malloc(sizeof(u3CalibrationInfo));
	hDevice = init_DAQ(caliInfo);

	/* Provide prompt to the user for a voltage range between 0
	 * and 5 V. Require a new set of inputs if an invalid range
	 * has been entered. */

	// prompt and acquire max and min voltage	
	printf(MAX_VOLTAGE_STR);
	scanf("%lf", &max_voltage);
	while (max_voltage < 0 || max_voltage > 5)
	{
		printf(INVALID_VOLTAGE_STR);
		scanf("%lf", &max_voltage);
	}
	
	printf(MIN_VOLTAGE_STR);
	scanf("%lf", &min_voltage);
	while (min_voltage < 0 || min_voltage > 5)
	{
		printf(INVALID_VOLTAGE_STR);
		scanf("%lf", &min_voltage);
	}
	

	/* Compute the maximum resolutiuon of the CLOCK_REALTIME
	 * system clock and output the theoretical maximum frequency
	 * for a square wave */
	int err = clock_getres(CLOCK_REALTIME, &max_fq);
	if (err == -1)
	{
		printf("cannot process clock_getres");
	}
	long resolution = max_fq.tv_sec + max_fq.tv_nsec*1000000000;
	printf("CLOCK_REALTIME resolution: %ld  Hz\n\n", resolution);

	/* Provide prompt to the user to input a desired square wave
	 * frequency in Hz. */
	// struct timespec period;
	//acquireFrequency(&it);

	// acquire frequency from user
	printf(FREQUENCY_STR);
	scanf("%lf", &frequency);
	
	long interval_nano = 1.0 / (2 * frequency) * 1000000000;
	timer_spec.it_value.tv_sec = interval_nano / 1000000000;
	timer_spec.it_value.tv_nsec = interval_nano % 1000000000;
	timer_spec.it_interval.tv_sec = interval_nano / 1000000000;
	timer_spec.it_interval.tv_nsec = interval_nano % 1000000000;
	

	// decide whether to generate analog or digital wave
	
	char isAnalog, isDigital;

	printf("Do you want to generate an analog wave? [Y/N]: ");
	scanf(" %c", &isAnalog);
	analog_flag = (isAnalog == 'Y' || isAnalog == 'y') ? 1 : 0;
	
	printf("Do you want to generate an digital wave? [Y/N]: ");
	scanf(" %c", &isDigital);
	digital_flag = (isDigital == 'Y' || isDigital == 'y') ? 1 : 0;
	

	// init signals and timer
	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handle_rt_signal;
	sigemptyset(&sa.sa_mask);

	if (digital_flag)
	{
		sigaddset(&sa.sa_mask, SIGRTMIN);
		sigaction(SIGRTMIN, &sa, NULL);
		
		memset(&ev1, 0, sizeof(ev1));
		ev1.sigev_notify = SIGEV_SIGNAL;
		ev1.sigev_signo = SIGRTMIN;
		ev1.sigev_value.sival_ptr = &timer1;
		timer_create(CLOCK_REALTIME, &ev1, &timer1);
		timer_settime(timer1, 0, &timer_spec, NULL);
	}
	if (analog_flag)
	{
		sigaddset(&sa.sa_mask, SIGRTMAX);
		sigaction(SIGRTMAX, &sa, NULL);
		
		memset(&ev2, 0, sizeof(ev2));
		ev2.sigev_notify = SIGEV_SIGNAL;
		ev2.sigev_signo = SIGRTMAX;
		ev2.sigev_value.sival_ptr = &timer2;
		timer_create(CLOCK_REALTIME, &ev2, &timer2);
		timer_settime(timer2, 0, &timer_spec, NULL);
	}
	

	int isExit = 0;
	char command[5] = "quit";
	/* Display a prompt to the user such that if the "exit"
	* command is typed, the USB DAQ is released and the program
	* is terminated. */
	printf("Enter Command: ");
	while(!isExit) {
		// isExit = (strcmp(command, "quit") == 0) ? 1 : 0;
		if (scanf("%4s", command) > 0) {
			isExit = (strcmp(command, "quit") == 0) ? 1 : 0;
		}
	}
		
	
	//do {
	//	
	//	scanf("%4s", command);
	//	isExit = (strcmp(command, "quit") == 0) ? 1 : 0;
	//} while(!isExit);

	/* Program a timer to deliver a SIGRTMIN signal, and the
		* corresponding signal handler to output a square wave on
		* BOTH digital output pin FIO2 and analog pin DAC0. */

	/* The square wave generated on the DAC0 analog pin should
		* have the voltage range specified by the user in the step
		* above. */
	return EXIT_SUCCESS;
}


