//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2024, Thierry Lelegard
// BSD-2-Clause license, see LICENSE.txt file or https://tsduck.io/license
//
//----------------------------------------------------------------------------
//!
//!  @file
//!  Command line arguments for the class UDPReceiver.
//!
//----------------------------------------------------------------------------

#pragma once
#include "tsIPv4SocketAddress.h"

namespace ts {

    class Args;
    class DuckContext;

    //!
    //! Command line arguments for the class UDPReceiver.
    //! @ingroup net
    //!
    class TSDUCKDLL UDPReceiverArgs
    {
    public:
        //!
        //! Constructor.
        //!
        UDPReceiverArgs() = default;

        bool             reuse_port = true;          //!< Reuse port socket option (-\-no-reuse-port not present).
        bool             default_interface = false;  //!< Use default local interface only for multicast (-\-default-interface).
        bool             use_first_source = false;   //!< Use socket address of first received packet to filter subsequent packets (-\-first-source).
        bool             mc_loopback = true;         //!< Multicast loopback option (-\-disable-multicast-loop not present).
        bool             use_ssm = false;            //!< Use source-specific multicast (-\-ssm or SSM syntax used in destination).
        bool             receive_timestamps = true;  //!< Get receive timestamps, currently hardcoded, is there a reason to disable it?
        size_t           receive_bufsize = 0;        //!< Socket receive buffer size in bytes (-\-buffer-size).
        cn::milliseconds receive_timeout = cn::milliseconds(-1);  //!< Receive timeout (-\-receive-timeout).
        IPv4Address      local_address {};           //!< Optional local addresses on which to listen (-\-local-address).

        //!
        //! Destination of packets to read ([address:]port).
        //! The UDP port is mandatory. The IP address is optional. When present, it must be a multicast address.
        //! When the IP address is not present, receive unicast packets on the specified UDP port.
        //!
        IPv4SocketAddress destination {};

        //!
        //! Optional source (-\-source address[:port]).
        //! The packets are accepted only is they come from the corresponding source.
        //! If the source IP address or source UDP port is unspecified, any address or port is accepted.
        //! If @a use_ssm is true, the source is used in source-specific multicast.
        //!
        IPv4SocketAddress source {};

        //!
        //! Set application-specified parameters to receive unicast traffic.
        //! This method is used when command line parameters are not used.
        //! @param [in] local_address Optional local address and required UDP port.
        //! @param [in] reuse_port Optional reuse-port option.
        //! @param [in] buffer_size Optional socket receive buffer size.
        //!
        void setUnicast(const IPv4SocketAddress& local_address, bool reuse_port = true, size_t buffer_size = 0);

        //!
        //! Add command line option definitions in an Args.
        //! @param [in,out] args Command line arguments to update.
        //! @param [in] with_short_options When true, define one-letter short options.
        //! @param [in] destination_is_parameter When true, the destination [address:]port is defined as a mandatory parameter.
        //! When false, it is defined as option --ip--udp (optional, can be omitted)
        //!
        void defineArgs(Args& args, bool with_short_options, bool destination_is_parameter)
        {
            _dest_is_parameter = destination_is_parameter;
            DefineArgs(args, with_short_options, destination_is_parameter, false);
        }

        //!
        //! Load arguments from command line.
        //! Args error indicator is set in case of incorrect arguments.
        //! @param [in,out] duck TSDuck execution context.
        //! @param [in,out] args Command line arguments from which to load the parameters.
        //! @param [in] default_receive_timeout Default receive timeout in milliseconds. No timeout if zero or negative.
        //! @return True on success, false on error in argument line.
        //!
        bool loadArgs(DuckContext& duck, Args& args, cn::milliseconds default_receive_timeout = cn::milliseconds(-1))
        {
            return loadArgs(duck, args, _dest_is_parameter, 0, default_receive_timeout, IPv4Address(), IPv4SocketAddress());
        }

    private:
        bool _dest_is_parameter = true;

        // Option name for destination address.
        static const UChar* DestName(bool destination_is_parameter) { return destination_is_parameter ? u"" : u"ip-udp"; }

        // Support for multiple destination addresses.
        friend class UDPReceiverArgsList;
        static void DefineArgs(Args& args, bool with_short_options, bool destination_is_parameter, bool multiple_receivers);
        bool loadArgs(DuckContext& duck,
                      Args& args,
                      bool destination_is_parameter,
                      size_t dest_index,
                      cn::milliseconds default_receive_timeout,
                      const IPv4Address& default_local_address,
                      const IPv4SocketAddress& default_source);
    };
}
