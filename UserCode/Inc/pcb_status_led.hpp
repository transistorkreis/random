//  published under the MIT License
//  [github.com/transistorkreis/Random]
//
//  compliant to the Google C++ Style Guide
//  [https://google.github.io/styleguide/cppguide.html]
//
//! \brief   class declaration for setting the operating mode of the PCB LED
//! \details the PCB LED can be switched on or blink with different frequencies
//! \file    pcb_status_led.hpp
//! \author  transistorkreis
//! \date    2020-07-29

#ifndef PCB_STATUS_LED_HPP_
#define PCB_STATUS_LED_HPP_

// INCLUDES --------------------------------------------------------------------
#include "stm32l4xx_hal.h"

namespace tkrandom {

// TYPE DECLARATIONS -----------------------------------------------------------
//! enum type for the operation modes and states of the internal state machine
enum class PcbStatusLedMode {
  kNormalMode,  //!< PCB LED signals normal operation mode
  kErrorMode,   //!< PCB LED signals error mode
  kSwitchedOff  //!< PCB LED is switched off
};

// CLASS DECLARATION -----------------------------------------------------------
//! PcbStatusLed class declaration
class PcbStatusLed {
 public:
  //! constructor
  //! \param[in] gpio_port_led pionter to GPIO port for PCB LED
  //! \param[in] pin_led pin number of PCB LED
  PcbStatusLed(GPIO_TypeDef* const gpio_port_led, const uint16_t pin_led);

  //! deconstructor
  ~PcbStatusLed(void) {}

  //! no copy constructor allowed since there is only one instance
  PcbStatusLed(const PcbStatusLed&) = delete;

  //! no assignment operator allowed since there is only one instance
  PcbStatusLed& operator=(PcbStatusLed const&) = delete;

  //! switches operation mode of PCB status LED
  //! /param[in] mode PCB status LED operation mode that is switched to
  void SetLedMode(const PcbStatusLedMode mode);

  //! uses timer interrupt to switch internal state machine
  void ProcessTick(void);

 private:
  //! current state of the internal state machine
  PcbStatusLedMode mode_;

  //! incremented and processed by ProcessTick()
  uint32_t tick_counter_;

  //! number of processed ticks when PCB LED is toggled in error mode
  const uint32_t kErrorCount_;

  //! number of processed ticks when PCB LED is switched on in normal mode
  const uint32_t kNormalCountOn_;

  //! number of processed ticks when PCB LED is switched off in normal mode
  const uint32_t kNormalCountOff_;

  //! pionter to GPIO port for PCB LED
  GPIO_TypeDef* const gpio_port_led_;

  //! pin_dac pin number of DAC PCB LED
  const uint16_t kPinLed_;

};

}  // namespace tkrandom
#endif  // PCB_STATUS_LED_HPP_
