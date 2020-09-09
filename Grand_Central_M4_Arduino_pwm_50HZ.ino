// Adafruit Metro M4 Only: Set-up digital pin D13 to output 50Hz, single slope PWM with a 50% duty cycle
void setup()
{
  // Set up the generic clock (GCLK7) to clock timer TCC0
  GCLK->GENCTRL[13].reg = GCLK_GENCTRL_DIV(1) |       // Divide the 48MHz clock source by divisor 1: 48MHz/1 = 48MHz
                         GCLK_GENCTRL_IDC |          // Set the duty cycle to 50/50 HIGH/LOW
                         GCLK_GENCTRL_GENEN |        // Enable GCLK7
                         GCLK_GENCTRL_SRC_DFLL;      // Select 48MHz DFLL clock source
                         //GCLK_GENCTRL_SRC_DPLL1;     // Select 100MHz DPLL clock source
                         GCLK_GENCTRL_SRC_DPLL0;     // Select 120MHz DPLL clock source
  while (GCLK->SYNCBUSY.bit.GENCTRL11);               // Wait for synchronization 

  GCLK->PCHCTRL[25].reg = GCLK_PCHCTRL_CHEN |        // Enable the TCC0 peripheral channel
                          GCLK_PCHCTRL_GEN_GCLK11;    // Connect generic clock 13 to TCC0

  // Enable the peripheral multiplexer on pin D7
  PORT->Group[g_APinDescription[11].ulPort].PINCFG[g_APinDescription[11].ulPin].bit.PMUXEN = 1;
 
  // Set the D7 (PORT_PB12) peripheral multiplexer to peripheral (even port number) E(6): TCC0, Channel 0
  PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg |= PORT_PMUX_PMUXE(6);
 
  TCC0->CTRLA.reg = TC_CTRLA_PRESCALER_DIV8 |        // Set prescaler to 8, 48MHz/8 = 6MHz
                    TC_CTRLA_PRESCSYNC_PRESC;        // Set the reset/reload to trigger on prescaler clock                 

  TCC0->WAVE.reg = TC_WAVE_WAVEGEN_NPWM;             // Set-up TCC0 timer for Normal (single slope) PWM mode (NPWM)
  while (TCC0->SYNCBUSY.bit.WAVE)                    // Wait for synchronization

  TCC0->PER.reg = 119999;                            // Set-up the PER (period) register 50Hz PWM
  while (TCC0->SYNCBUSY.bit.PER);                    // Wait for synchronization
 
  TCC0->CC[0].reg = 59999;                           // Set-up the CC (counter compare), channel 0 register for 50% duty-cycle
  while (TCC0->SYNCBUSY.bit.CC0);                    // Wait for synchronization

  TCC0->CTRLA.bit.ENABLE = 1;                        // Enable timer TCC0
  while (TCC0->SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
}

void loop() {
  delay(1000)}
