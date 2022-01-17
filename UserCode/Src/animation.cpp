//  published under the MIT License
//  [github.com/transistorkreis/tkrandom]
//
//  compliant to the Google C++ Style Guide
//  [https://google.github.io/styleguide/cppguide.html]
//
//! \brief   class definition for frontpanel LED animation
//! \details definition of function members for LED switching patterns
//! \file    animation.cpp
//! \author  transistorkreis
//! \date    2020-07-29

// INCLUDES --------------------------------------------------------------------
#include "animation.hpp"

// MICS ------------------------------------------------------------------------

namespace tkrandom {

// MEMBER FUNCTIONS ------------------------------------------------------------
Animation::Animation(Transmitter& transmitter)
    : state_(AnimationState::kStateNone),
      transmitter_(transmitter) {

}
//------------------------------------------------------------------------------
AnimationStatus Animation::ClockAnimation() {
  AnimationStatus return_value = AnimationStatus::kOngoing;
  TransmitterStatus transmitter_status = TransmitterStatus::kSuccess;

  // switches LEDs if counter threshold has been reached
  switch (state_) {
    case AnimationState::kStateNone:
      transmitter_status = transmitter_.SetLedBrightness(Led::kLed1, 0xffffU);
      state_ = AnimationState::kState1;
      break;
    case AnimationState::kState1:
      transmitter_status = SwitchLeds(Led::kLed2, Led::kLed1);
      state_ = AnimationState::kState2;
      break;
    case AnimationState::kState2:
      transmitter_status = SwitchLeds(Led::kLed3, Led::kLed2);
      state_ = AnimationState::kState3;
      break;
    case AnimationState::kState3:
      transmitter_status = SwitchLeds(Led::kLed4, Led::kLed3);
      state_ = AnimationState::kState4;
      break;
    case AnimationState::kState4:
      transmitter_status = SwitchLeds(Led::kLed3, Led::kLed4);
      state_ = AnimationState::kState5;
      break;
    case AnimationState::kState5:
      transmitter_status = SwitchLeds(Led::kLed2, Led::kLed3);
      state_ = AnimationState::kState6;
      break;
    case AnimationState::kState6:
      transmitter_status = SwitchLeds(Led::kLed1, Led::kLed2);
      state_ = AnimationState::kState7;
      break;
    case AnimationState::kState7:
      transmitter_status = transmitter_.SetLedBrightness(Led::kLed1, 0x0000U);
      state_ = AnimationState::kStateComplete;
      break;
    case AnimationState::kStateComplete:
      return_value = AnimationStatus::kCompleted;
      break;
    default:
      break;
  }

  if (transmitter_status != TransmitterStatus::kSuccess) {
    return_value = AnimationStatus::kError;
  }

  return return_value;
}
//------------------------------------------------------------------------------
TransmitterStatus Animation::SwitchLeds(const Led led_on,
                                        const Led led_off) const {
  TransmitterStatus return_code =
      transmitter_.SetLedBrightness(led_on, 0xffffU);
  if (return_code == TransmitterStatus::kSuccess) {
    return_code = transmitter_.SetLedBrightness(led_off, 0x0000U);
  }

  return return_code;
}

}  // namespace tkrandom
