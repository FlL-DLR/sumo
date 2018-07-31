/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2012-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    Junction.h
/// @author  Daniel Krajzewicz
/// @author  Mario Krumnow
/// @author  Michael Behrisch
/// @date    30.05.2012
/// @version $Id$
///
// C++ TraCI client API implementation
/****************************************************************************/
#ifndef Junction_h
#define Junction_h


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <vector>
#include <libsumo/TraCIDefs.h>


// ===========================================================================
// class declarations
// ===========================================================================
class NamedRTree;
class MSJunction;
class PositionVector;
namespace libsumo {
    class VariableWrapper;
}


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class Junction
 * @brief C++ TraCI client API implementation
 */
namespace libsumo {
class Junction {
public:
    static std::vector<std::string> getIDList();
    static int getIDCount();
    static TraCIPosition getPosition(const std::string& junctionID);
    static TraCIPositionVector getShape(const std::string& junctionID);

    static void subscribe(const std::string& objID, const std::vector<int>& vars = std::vector<int>(), SUMOTime beginTime = 0, SUMOTime endTime = ((2 ^ 31) - 1));
    static void subscribeContext(const std::string& objID, int domain, double range, const std::vector<int>& vars = std::vector<int>(), SUMOTime beginTime = 0, SUMOTime endTime = ((2 ^ 31) - 1));
    static const SubscriptionResults getSubscriptionResults();
    static const TraCIResults getSubscriptionResults(const std::string& objID);
    static const ContextSubscriptionResults getContextSubscriptionResults();
    static const SubscriptionResults getContextSubscriptionResults(const std::string& objID);

    /** @brief Returns a tree filled with junction instances
     * @return The rtree of junctions
     */
    static NamedRTree* getTree();

    /** @brief Saves the shape of the requested object in the given container
    *  @param id The id of the poi to retrieve
    *  @param shape The container to fill
    */
    static void storeShape(const std::string& id, PositionVector& shape);

    static std::shared_ptr<VariableWrapper> makeWrapper();

    static bool handleVariable(const std::string& objID, const int variable, VariableWrapper* wrapper);

private:
    static MSJunction* getJunction(const std::string& id);

private:
    static SubscriptionResults mySubscriptionResults;
    static ContextSubscriptionResults myContextSubscriptionResults;

private:
    /// @brief invalidated standard constructor
    Junction() = delete;
};
}


#endif

/****************************************************************************/
