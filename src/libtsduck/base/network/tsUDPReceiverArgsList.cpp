//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2024, Thierry Lelegard
// BSD-2-Clause license, see LICENSE.txt file or https://tsduck.io/license
//
//----------------------------------------------------------------------------

#include "tsUDPReceiverArgsList.h"
#include "tsArgs.h"


//----------------------------------------------------------------------------
// Load arguments from command line.
//----------------------------------------------------------------------------

bool ts::UDPReceiverArgsList::loadArgs(DuckContext& duck, Args& args, cn::milliseconds receive_timeout)
{
    bool ok = true;

    // Resize the list of arguments based on the number of destination address parameters.
    clear();
    resize(args.count(UDPReceiverArgs::DestName(_dest_is_parameter)));

    IPv4Address previous_local_address;
    IPv4SocketAddress previous_source;

    // Get all addresses.
    for (size_t index = 0; index < size(); ++index) {
        UDPReceiverArgs& rec(at(index));
        ok = rec.loadArgs(duck, args, _dest_is_parameter, index, receive_timeout, previous_local_address, previous_source) && ok;
        previous_local_address = rec.local_address;
        previous_source = rec.source;
    }

    return ok;
}
