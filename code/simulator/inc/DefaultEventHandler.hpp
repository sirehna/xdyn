/*
 * DefaultEventHandler.hpp
 *
 *  Created on: 21 mars 2014
 *      Author: maroff
 */

#ifndef DEFAULTEVENTHANDLER_HPP_
#define DEFAULTEVENTHANDLER_HPP_

class DefaultEventHandler
{
    public:
        bool detected_state_events() const
        {
            return false;
        }

        void locate_event()
        {
        }

        void run_event_actions()
        {
        }
};



#endif /* DEFAULTEVENTHANDLER_HPP_ */
