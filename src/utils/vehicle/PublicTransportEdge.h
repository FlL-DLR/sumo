/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    PublicTransportEdge.h
/// @author  Michael Behrisch
/// @date    Mon, 03 March 2014
/// @version $Id: CarEdge.h v0_32_0+0478-2bb98619fa oss@behrisch.de 2018-01-11 12:40:22 +0100 $
///
// The PublicTransportEdge is a special intermodal edge connecting the stop edges with scheduled traffic
/****************************************************************************/
#ifndef PublicTransportEdge_h
#define PublicTransportEdge_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include "IntermodalEdge.h"


// ===========================================================================
// class definitions
// ===========================================================================
/// @brief the public transport edge type connecting the stop edges
template<class E, class L, class N, class V>
class PublicTransportEdge : public IntermodalEdge<E, L, N, V> {
private:
    struct Schedule {
        Schedule(const SUMOTime _begin, const SUMOTime _end, const SUMOTime _period, const double _travelTimeSec)
            : begin(STEPS2TIME(_begin)), end(STEPS2TIME(_end)), period(STEPS2TIME(_period)), travelTimeSec(_travelTimeSec) {}
        const double begin;
        const double end;
        const double period;
        const double travelTimeSec;
    private:
        /// @brief Invalidated assignment operator
        Schedule& operator=(const Schedule& src);
    };

public:
    PublicTransportEdge(const std::string id, int numericalID, const IntermodalEdge<E, L, N, V>* entryStop, const E* endEdge, const std::string& line) :
        IntermodalEdge<E, L, N, V>(line + ":" + (id != "" ? id : endEdge->getID()), numericalID, endEdge, line), myEntryStop(entryStop) { }

    bool includeInRoute(bool /* allEdges */) const {
        return true;
    }

    bool prohibits(const IntermodalTrip<E, N, V>* const trip) const {
        return (trip->modeSet & SVC_BUS) == 0;
    }

    const IntermodalEdge<E, L, N, V>* getEntryStop() const {
        return myEntryStop;
    }

    void addSchedule(const SUMOTime begin, const SUMOTime end, const SUMOTime period, const double travelTimeSec) {
        //std::cout << " edge=" << myEntryStop->getID() << "->" << this->getID() << " beg=" << STEPS2TIME(begin) << " end=" << STEPS2TIME(end)
        //    << " period=" << STEPS2TIME(period)
        //    << " travelTime=" << travelTimeSec << "\n";
        mySchedules.insert(std::make_pair(STEPS2TIME(begin), Schedule(begin, end, period, travelTimeSec)));
    }

    double getTravelTime(const IntermodalTrip<E, N, V>* const /* trip */, double time) const {
        double minArrivalSec = std::numeric_limits<double>::max();
        for (typename std::multimap<double, Schedule>::const_iterator it = mySchedules.begin(); it != mySchedules.end(); ++it) {
            const Schedule& s = it->second;
            if (it->first > minArrivalSec) {
                break;
            }
            if (time < s.end) {
                int running;
                if (s.period <= 0 || time < s.begin) {
                    // single vehicle or flow begin
                    running = 0;
                } else {
                    // subsequent flow
                    running = (int)ceil((time - s.begin) / s.period);
                }
                const double nextDepart = s.begin + running * s.period;
                minArrivalSec = MIN2(nextDepart + s.travelTimeSec, minArrivalSec);
                //std::cout << " edge=" << myEntryStop->getID() << "->" << this->getID() << " beg=" << s.begin << " end=" << s.end
                //    << " atTime=" << time
                //    << " running=" << running << " nextDepart=" << nextDepart
                //    << " minASec=" << minArrivalSec << " travelTime=" << minArrivalSec - time << "\n";
            }
        }
        return minArrivalSec - time;
    }

private:
    std::multimap<double, Schedule> mySchedules;
    const IntermodalEdge<E, L, N, V>* const myEntryStop;

};


#endif

/****************************************************************************/
