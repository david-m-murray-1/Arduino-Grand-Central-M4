// Enable/Disable test on TC0, pin A3 on Metro M4, pin A13 on Metro M4 Grand Central
void setup()
{
  Serial.begin(115200);                                        // Open USB serial comms
 
  MCLK->APBAMASK.reg |= MCLK_APBAMASK_TC0;                     // Activate timer TC0
 
  GCLK->PCHCTRL[TC0_GCLK_ID].reg = GCLK_PCHCTRL_CHEN |         // Enable perhipheral channel
                                   GCLK_PCHCTRL_GEN_GCLK0;     // Connect generic clock 0 to TC0

  // Enable the peripheral multiplexer on pin A13
  PORT->Group[g_APinDescription[A13].ulPort].PINCFG[g_APinDescription[A13].ulPin].bit.PMUXEN = 1;
 
  // Set the A3 peripheral multiplexer to peripheral E(4): TC0, Channel 0
  PORT->Group[g_APinDescription[A13].ulPort].PMUX[g_APinDescription[A13].ulPin >> 1].reg |= PORT_PMUX_PMUXE(4);
 
  TC0->COUNT8.CTRLA.reg = TC_CTRLA_PRESCALER_DIV2 |     // Set prescaler to 1024, 120MHz/1024 = 117.19kHz
                          TC_CTRLA_PRESCSYNC_PRESC  |     // Set the reset/reload to trigger on prescaler clock
                          TC_CTRLA_MODE_COUNT8;           // Set the counter to 8-bit mode

  TC0->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_NPWM;      // Set-up TC0 timer for Normal PWM mode (NPWM)

  TC0->COUNT8.PER.reg = 0xFF;                       // Use PER register as TOP value, set for 457.76Hz PWM
  while (TC0->COUNT8.SYNCBUSY.bit.PER);             // Wait for synchronization

  TC0->COUNT8.CC[0].reg = 127;                      // Set the duty cycle to 50% (CC1 half of PER)
  while (TC0->COUNT8.SYNCBUSY.bit.CC0);             // Wait for synchronization

  TC0->COUNT8.CTRLA.bit.ENABLE = 1;                 // Enable timer TC0
  while (TC0->COUNT8.SYNCBUSY.bit.ENABLE);          // Wait for synchronization
}

void loop()
{
  TC0->COUNT8.CTRLA.bit.ENABLE = 1;                       // Enable timer TC0
  while (TC0->COUNT8.SYNCBUSY.bit.ENABLE);                // Wait for synchronization
  for (uint8_t i = 0; i < 10; i++)
  {
    TC0->COUNT8.CTRLBSET.reg = TC_CTRLBSET_CMD_READSYNC;   // Trigger a read synchronization on the COUNT register
    while (TC0->COUNT8.SYNCBUSY.bit.CTRLB);                // Wait for synchronization
    while (TC0->COUNT8.SYNCBUSY.bit.COUNT);                // Wait for read synchronization
  }
                                             // Wait for 1 second
  TC0->COUNT8.CTRLA.bit.ENABLE = 0;                        // Enable timer TC0
  while (TC0->COUNT8.SYNCBUSY.bit.ENABLE);                 // Wait for synchronization
  for (uint8_t i = 0; i < 10; i++)
  {
    TC0->COUNT8.CTRLBSET.reg = TC_CTRLBSET_CMD_READSYNC;   // Trigger a read synchronization on the COUNT register
    while (TC0->COUNT8.SYNCBUSY.bit.CTRLB);                // Wait for synchronization
    while (TC0->COUNT8.SYNCBUSY.bit.COUNT);                // Wait for read synchronization
  }                                             // Wait for 1 second
}
