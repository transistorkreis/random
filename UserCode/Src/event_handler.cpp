//! \brief     Class definition for processing state machine events.
//! \details   -
//! \file      event_handler.cpp
//! \author    André Niederlein
//! \date      2020-07-29
//! \copyright GNU General Public License v3:
//! \copyright https://opensource.org/licenses/GPL-3.0
//
//  compliant to the Google C++ Style Guide:
//  https://google.github.io/styleguide/cppguide.html

// INCLUDES --------------------------------------------------------------------
#include "event_handler.hpp"

// MICS ------------------------------------------------------------------------

namespace tkrandom {

// MEMBER FUNCTIONS ------------------------------------------------------------
EventHandler::EventHandler(PcbStatusLed& pcb_status_led,
                           RngHandler& rng_handler,
                           Animation& animation,
                           DistributionPins& distribution_pins)
    : state_(EventHandlerState::kAnimation),
      pcbStatusLed_(pcb_status_led),
      rng_handler_(rng_handler),
      animation_(animation),
      distribution_pins_(distribution_pins),
      has_timer_elapsed_(false),
      has_error_occurred_(false),
      has_distribution_changed_(false),
      is_gate_1_(false),
      is_gate_2_(false),
      debounce_counter_(0U),
      kDebounceDelay_(1U) {

}
//------------------------------------------------------------------------------
void EventHandler::Run() {
  // processes gate inputs
  if (state_ == EventHandlerState::kWorking) {
    if (is_gate_1_ || is_gate_2_) {
      const RngHandlerStatus rng_handler_status =
          rng_handler_.SetOutputsLeds(is_gate_1_, is_gate_2_);
      if (rng_handler_status != RngHandlerStatus::kSuccess) {
        HandleError();
      }
      is_gate_1_ = false;
      is_gate_2_ = false;
    }
  }
  // timer based event processing
  if (has_timer_elapsed_) {
    pcbStatusLed_.ProcessTick();
    rng_handler_.ProcessTick();
    if (has_distribution_changed_) {
      ProcessDistribution();  // timer based processing for switch debouncing
    }
    if (state_ == EventHandlerState::kAnimation) {
      const AnimationStatus animation_status = animation_.ClockAnimation();
      if (animation_status == AnimationStatus::kCompleted) {
        state_ = EventHandlerState::kWorking;
      }
      if (animation_status == AnimationStatus::kError) {
        HandleError();
      }
    }
    has_timer_elapsed_ = false;
  }
  // error handling
  if (has_error_occurred_) {
    HandleError();
    has_error_occurred_ = false;
  }
}
//------------------------------------------------------------------------------
void EventHandler::Init() {
  rng_handler_.Init();
  has_distribution_changed_ = true;  // triggers ProcessDistribution()
}
//------------------------------------------------------------------------------
void EventHandler::ProcessDistribution() {
  // debounces distribution switches
  if (debounce_counter_ > kDebounceDelay_) {
    has_distribution_changed_ = false;
    debounce_counter_ = 0U;
    Distribution distribution_1 = Distribution::kUniform;
    Distribution distribution_2 = Distribution::kUniform;
    Distribution distribution_3 = Distribution::kUniform;
    Distribution distribution_4 = Distribution::kUniform;
    const GPIO_PinState pin_distr_1 =
        HAL_GPIO_ReadPin(distribution_pins_.gpio_port_distribution_1,
                         distribution_pins_.pin_distribution_1);
    const GPIO_PinState pin_distr_2 =
        HAL_GPIO_ReadPin(distribution_pins_.gpio_port_distribution_2,
                         distribution_pins_.pin_distribution_2);
    const GPIO_PinState pin_distr_3 =
        HAL_GPIO_ReadPin(distribution_pins_.gpio_port_distribution_3,
                         distribution_pins_.pin_distribution_3);
    const GPIO_PinState pin_distr_4 =
        HAL_GPIO_ReadPin(distribution_pins_.gpio_port_distribution_4,
                         distribution_pins_.pin_distribution_4);
    if (pin_distr_1 == GPIO_PIN_SET) {
      distribution_1 = Distribution::kNormal;
    }
    if (pin_distr_2 == GPIO_PIN_SET) {
          distribution_2 = Distribution::kNormal;
        }
    if (pin_distr_3 == GPIO_PIN_SET) {
          distribution_3 = Distribution::kNormal;
        }
    if (pin_distr_4 == GPIO_PIN_SET) {
          distribution_4 = Distribution::kNormal;
        }
    rng_handler_.SetDistribution(Output::kOutput1, distribution_1);
    rng_handler_.SetDistribution(Output::kOutput2, distribution_2);
    rng_handler_.SetDistribution(Output::kOutput3, distribution_3);
    rng_handler_.SetDistribution(Output::kOutput4, distribution_4);
  }
  debounce_counter_++;
}
//------------------------------------------------------------------------------
void EventHandler::SignalEvent(const Event event) {
  switch (event) {
    case Event::kDistributionChanged:
      has_distribution_changed_ = true;
      break;
    case Event::kErrorOccurred:
      has_error_occurred_ = true;
      break;
    case Event::kGate1Triggered:
      is_gate_1_ = true;
      break;
    case Event::kGate2Triggered:
      is_gate_2_ = true;
      break;
    case Event::kTimerElapsed:
      has_timer_elapsed_ = true;
      break;
    default:
      break;
  }
}
//------------------------------------------------------------------------------
void EventHandler::HandleError() {
  pcbStatusLed_.SetLedMode(PcbStatusLedMode::kErrorMode);
  // TODO: any proper error handling
}

}  // namespace tkrandom
