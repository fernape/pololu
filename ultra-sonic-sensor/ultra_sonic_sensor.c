/*
   Uses a ultra-sonic distance sensor connected to one of
   the free I/O pins and prints the readouts on the LCD screen

   Ape, Fri Aug 10 20:52:07 CEST 2012
 */

// F_CPU tells util/delay.h our clock frequency
//#define F_CPU 8000000UL	// Orangutan frequency (8MHz)
#define F_CPU 20000000UL	// Baby Orangutan frequency (20MHz)

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <pololu/3pi.h>
#include <pololu/motors.h>

static int echo_pin = PD0;				/* Arduino's digital pin #2 */
static const int echo_length_us = 15; 			/* 10 us pulse width is the minimun length required */
static const int measurments_delay_ms = 100;		/* 10 ms between echoes at least */

#define	TURN_LEFT_90	do { set_motors(40, 0); _delay_ms(300); } while(0)
#define	TURN_RIGHT_90	do { set_motors(0, -40); _delay_ms(300); } while(0)
#define	TURN_LEFT_180	do { set_motors(40, 0); _delay_ms(600); } while(0)
#define	TURN_RIGHT_180	do { set_motors(0, -40); _delay_ms(600); } while(0)


/* Set pin as input */
void
pin_as_input()
{
	/* 
	   Set echo_in_pin as input
	 */
	DDRD &= ~(_BV(echo_pin));
}

/* Set pin as output */
void
pin_as_output()
{
	/* 
	   Set echo_out_pin as output.
	 */
	DDRD |= _BV(echo_pin);
}
/*
   Set echo_out_pin position in PORTD to 1.
   Leave the rest of the bits untouched
 */
inline void
pin_set_high()
{
	/* Set second pin in the register */
	/*PGM_P str = "Output high...\n";
	printf(str);
	fflush(stdout);*/
	PORTD |= _BV(echo_pin);
}
/*
   Set echo_out_pin position in PORTD to 0.
   Leave the rest of the bits untouched
 */
inline void
pin_set_low()
{
	/* Clear the register */
	/*PGM_P str = "Output low...\n";
	printf(str);
	fflush(stdout);*/
	PORTD &= ~(_BV(echo_pin));
}

/*
   Waits for the answer of the echo and returns
   the length of the pulse in cm.
 */
int
read_value()
{

	int pulse_length_ms = 0;

	/* Wait until we receive the response */
	loop_until_bit_is_set(PIND, echo_pin);

	while (PIND & _BV(echo_pin)) { 
		_delay_us(1); 
		pulse_length_ms++;
	}

	/* 
	   The length of the pulse is the distance in inches,
	   so divide it by 58 to obtain cm.
	 */
	return (pulse_length_ms / 58);
}

/*
   Sends an echo 
 */
void
send_echo()
{

	/* Ensure we start from low impedance */
	pin_set_low();
	pin_set_high();
	_delay_us(echo_length_us);
	pin_set_low();
}

/*
  Sends an echo an reads the response.
  It returns the distance in cms.
 */
int
measure_distance()
{
	int pulse_length = 0;

	pin_as_output();

	/* Send echo */
	send_echo();

	pin_as_input();

	/* Read the echo response */
	pulse_length = read_value();

	return (pulse_length); /* Divide by 74 to get the result in inches */
}

/*
   Program entry point
 */

int main(int argc, char **argv)
{
	int distance = 0;
	unsigned int turn = 0;
	int count_stuck = 0;

	_delay_ms(10000);

	for (;;) {
		/* Measure distance by sending a pulse and reading the response */
		distance = measure_distance();

		if (count_stuck > 2) {
			/* Go back */
			set_motors(-75, 75);
			_delay_ms(500);
			/* And turn */
			TURN_LEFT_180;
			TURN_LEFT_90;
			count_stuck = 0;
		}

		if (distance > 20) {
			red_led(LOW);
			green_led(HIGH);
			set_motors(75, -75);
			count_stuck = 0;
		} else {
			red_led(HIGH);
			green_led(LOW);

			if (turn > 5 && !(turn % 5)) {
				TURN_RIGHT_180;
			} else if (!(turn % 4)) {
				TURN_RIGHT_90;
			} else {
				TURN_LEFT_90;
			}

			count_stuck++;
		}

		turn++;

		/* Send data through RS232 */
		print_long((long)distance);
		print(" cm");

		/* Wait at least 10 ms */
		_delay_ms(measurments_delay_ms);
		clear();
	}


	/* Make the compiler happy */
	return (0);
}




