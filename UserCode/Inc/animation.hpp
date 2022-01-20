//! \brief     Class declaration for frontpanel LED animation.
//! \details   Declaration of function members for LED switching patterns.
//! \file      animation.hpp
//! \author    André Niederlein
//! \date      2020-07-29
//! \copyright GNU General Public License v3, see ../LICENSE
//
//  compliant to the Google C++ Style Guide:
//  https://google.github.io/styleguide/cppguide.html

#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

// INCLUDES --------------------------------------------------------------------
#include "transmitter.hpp"
#include "stm32l4xx_hal.h"

namespace tkrandom {

// TYPE DECLARATIONS -----------------------------------------------------------
//! enum type for Animation member function return values
enum class AnimationStatus {
  kCompleted,  //!< animation completed successfully
  kOngoing,    //!< animation not completed yet
  kError       //!< an error occurred
};

// CLASS DECLARATION -----------------------------------------------------------
//! Animation class declaration
class Animation {
 public:
  //! constructor
  Animation(Transmitter& transmitter);

  //! destructor
  ~Animation(void) {}

  //! no copy constructor allowed since there is only one instance
  Animation(const Animation&) = delete;

  //! no assignment operator allowed since there is only one instance
  Animation& operator=(Animation const&) = delete;

  //! clocks a short LED animation (after module is powered up)
  //! \return kCompleted if animation completed successfully
  AnimationStatus ClockAnimation(void);

 private:
  //! enum type for the states of the internal LED animation state machine
  enum class AnimationState {
    kStateNone,     //!< animation not started or done
    kState1,        //!< front panel LED 1 switched on
    kState2,        //!< front panel LED 2 switched on
    kState3,        //!< front panel LED 3 switched on
    kState4,        //!< front panel LED 4 switched on
    kState5,        //!< front panel LED 3 switched on
    kState6,        //!< front panel LED 2 switched on
    kState7,        //!< front panel LED 1 switched on
    kStateComplete  //!< animation completed
  };

  //! switches one front panel LED on and one front panel LED off
  //! \param[in] led_on LED that is switched on
  //! \param[in] led_off LED that is switched off
  //! \return kSuccess if no error occurs
  TransmitterStatus SwitchLeds(const Led led_on, const Led led_off) const;

  //! state of the internal state machine for the LED animation
  AnimationState state_;

  //! pointer to Transmitter instance for SPI transfers
  Transmitter& transmitter_;
};

}  // namespace tkrandom

#endif  // ANIMATION_HPP_
