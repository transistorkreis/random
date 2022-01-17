//  published under the MIT License
//  [github.com/transistorkreis/Random]
//
//  compliant to the Google C++ Style Guide
//  [https://google.github.io/styleguide/cppguide.html]
//
//! \brief   class declaration for processing state machine events
//! \details implemented as singleton
//! \file    event_handler.hpp
//! \author  transistorkreis
//! \date    2020-07-29

#ifndef EVENT_HANDLER_HPP_
#define EVENT_HANDLER_HPP_

// INCLUDES --------------------------------------------------------------------
#include "pcb_status_led.hpp"
#include "animation.hpp"
#include "rng_handler.hpp"

namespace tkrandom {

// TYPE DECLARATIONS -----------------------------------------------------------
//! struct with all pins required by EventHandler
struct DistributionPins {
  GPIO_TypeDef* gpio_port_distribution_1;  //!< GPIO port for distribution 1
  uint16_t pin_distribution_1;             //!< pin number of distribution 1
  GPIO_TypeDef* gpio_port_distribution_2;  //!< GPIO port for distribution 2
  uint16_t pin_distribution_2;             //!< pin number of distribution 2
  GPIO_TypeDef* gpio_port_distribution_3;  //!< GPIO port for distribution 3
  uint16_t pin_distribution_3;             //!< pin number of distribution 3
  GPIO_TypeDef* gpio_port_distribution_4;  //!< GPIO port for distribution 4
  uint16_t pin_distribution_4;             //!< pin number of distribution 4
};

enum class Event {
  kTimerElapsed,         // timer tick
  kDistributionChanged,  // EXTI line interrupt of the distribution switches
  kGate1Triggered,       // EXTI line interrupt of IN_1
  kGate2Triggered,       // EXTI line interrupt of IN_2
  kErrorOccurred         // an error occurred
};

// CLASS DECLARATION -----------------------------------------------------------
//! \brief class declaration of EventHandler Singleton
class EventHandler {
 public:
  //! constructor
  //! \param[in] pcb_status_led reference to control the PCB status LED
  //! \param[in] rng_handler RngHandler reference to set output values
  //! \param[in] animation Animation reference to trigger LED start animation
  //! \param[in] distribution_pins struct with all required pins
  EventHandler(PcbStatusLed& pcb_status_led, RngHandler& rng_handler,
               Animation& animation, DistributionPins& distribution_pins);

  //! destructor
  ~EventHandler(void) {}

  //! no copy constructor allowed since there is only one instance
  EventHandler(const EventHandler&) = delete;

  //! no assignment operator allowed since there is only one instance
  EventHandler& operator=(EventHandler const&) = delete;

  //! signals events to EventHandler instance
  //! \details signaled events are processed by Run()
  //! \param[in] event event to be signaled to the EventHandler instance
  void SignalEvent(const Event event);

  //! reiteratively called in main() to process signaled events
  void Run(void);

  //! initializes dependencies: Transmitter instance
  void Init(void);

 private:
  //! enum type for the states of the internal event handling state machine
  enum class EventHandlerState {
    kAnimation,  //!< animation not completed yet
    kWorking,    //!< normal working mode
    kError       //!< an error occurred
  };

  //! reads state of distribution switches, calls Transmitter::SetDistribution()
  void ProcessDistribution(void);

  //! switches mode of PCB status LED to kErrorMode
  void HandleError(void);

  //! state of the internal state machine for the event handling
  EventHandlerState state_;

  //! reference to PcbStatusLed instance in order to control the PCB status LED
  PcbStatusLed& pcbStatusLed_;

  //! RngBuffers reference to handle buffers with random numbers
  RngHandler& rng_handler_;

  //! Animation reference to trigger and control LED start animation
  Animation& animation_;

  //! struct with all pins required by EventHandler
  DistributionPins& distribution_pins_;

  //! set to true if timer tick occurred
  bool has_timer_elapsed_;

  //! set to true if an error occured somewhere
  bool has_error_occurred_;

  //! set to true if one of the distribution was switched
  bool has_distribution_changed_;

  //! set to true if gate 1 was triggered (IN_1)
  bool is_gate_1_;

  //! set to true if gate 2 was triggered (IN_2)
  bool is_gate_2_;

  //! number of timer ticks already waiting to debounce distribution switches
  uint32_t debounce_counter_;

  //! number of timer ticks that is waited to debounce distribution switches
  const uint32_t kDebounceDelay_;
};

}  // namespace tkrandom

#endif  // EVENT_HANDLER_HPP_
