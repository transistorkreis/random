//  published under the MIT License
//  [github.com/transistorkreis/Random]
//
//  compliant to the Google C++ Style Guide
//  [https://google.github.io/styleguide/cppguide.html]
//
//! \brief   class defintion for setting the operating mode of the PCB LED
//! \details the PCB LED can be switched on or blink with different frequencies
//! \file    pcb_status_led.cpp
//! \author  transistorkreis
//! \date    2020-07-29

// INCLUDES --------------------------------------------------------------------
#include "pcb_status_led.hpp"

// MICS ------------------------------------------------------------------------
namespace tkrandom {

// MEMBER FUNCTIONS ------------------------------------------------------------
PcbStatusLed::PcbStatusLed(GPIO_TypeDef* const gpio_port_led,
                           const uint16_t pin_led)
    : mode_(PcbStatusLedMode::kNormalMode),
      tick_counter_(0U),
      kErrorCount_(5U),
      kNormalCountOn_(48U),
      kNormalCountOff_(50U),
      gpio_port_led_(gpio_port_led),
      kPinLed_(pin_led) {
}
//------------------------------------------------------------------------------
void PcbStatusLed::SetLedMode(const PcbStatusLedMode mode) {
  mode_ = mode;
  if (mode_ == PcbStatusLedMode::kSwitchedOff) {
    HAL_GPIO_WritePin(gpio_port_led_, kPinLed_, GPIO_PIN_RESET);
  }
}
//------------------------------------------------------------------------------
void PcbStatusLed::ProcessTick() {
  switch (mode_) {
    case PcbStatusLedMode::kNormalMode:
      tick_counter_++;
      if (tick_counter_ > kNormalCountOn_) {
        HAL_GPIO_WritePin(gpio_port_led_, kPinLed_, GPIO_PIN_SET);
      }
      if (tick_counter_ > kNormalCountOff_) {
        HAL_GPIO_WritePin(gpio_port_led_, kPinLed_, GPIO_PIN_RESET);
        tick_counter_ = 0U;
      }
      break;
    case PcbStatusLedMode::kErrorMode:
      tick_counter_++;
      if (tick_counter_ > kErrorCount_) {
        HAL_GPIO_TogglePin(gpio_port_led_, kPinLed_);
        tick_counter_ = 0U;
      }
      break;
    case PcbStatusLedMode::kSwitchedOff:
      break;
    default:
      break;
  }
}

}  // namespace tkrandom
