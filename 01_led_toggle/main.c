#include <msp430.h> 
#include <stdint.h>

uint8_t led_flag;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    // Set pin 2.0 to an output
    P2DIR |= BIT0;

    // Set pin 2.0 output to 1
    P2OUT &= ~BIT0;

    // Control register: ACLK, divide by 8, Up mode
    TACTL |= TASSEL_1 + ID_3 + MC_1;

    // Capture/Compare control: Enable interrupt
    TACCTL0 |= CCIE;

    // Count up to 4095 before generating interrupt for 1 second count
    TACCR0 = 4095;

    // Main loop
    while (1)
    {
        // Toggle the LED
        if (led_flag == 1)
        {
            P2OUT ^= BIT0;
            led_flag = 0;
        }

        // Enter LPM0 w/ interrupt
        __bis_SR_register(LPM0_bits + GIE);
    }
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    // Set the LED flag so that the LED is toggled in the main loop
    led_flag = 1;

    // Clear the low power bits to wake the MCU
    __bic_SR_register_on_exit(LPM0_bits + GIE);
}
