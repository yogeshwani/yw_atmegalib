/* Name: stepper.c
 * Date modified: 2 April 2011
 * Considerations for the Stepper motor sequence
 */

uint16_t interpolate(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

