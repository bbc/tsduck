//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2024, Thierry Lelegard
// BSD-2-Clause license, see LICENSE.txt file or https://tsduck.io/license
//
//----------------------------------------------------------------------------
//
//  TSDuck Python bindings: encapsulates Report objects for Python.
//
//----------------------------------------------------------------------------

#include "tspy.h"
#include "tspyAsyncReport.h"
#include "tspySyncReport.h"
#include "tsCerrReport.h"
#include "tsNullReport.h"
#include "tsMemory.h"

//-----------------------------------------------------------------------------
// Build a report header from a severity.
//-----------------------------------------------------------------------------

TSDUCKPY void tspyReportHeader(int severity, uint8_t* buffer, size_t* buffer_size)
{
    if (buffer != nullptr && buffer_size != nullptr) {
        const ts::UString str(ts::Severity::Header(severity));
        *buffer_size = 2 * std::min(*buffer_size / 2, str.size());
        ts::MemCopy(buffer, str.data(), *buffer_size);
    }
}

//-----------------------------------------------------------------------------
// Get static report instances.
//-----------------------------------------------------------------------------

TSDUCKPY void* tspyStdErrReport()
{
    return &CERR;
}

TSDUCKPY void* tspyNullReport()
{
    return &NULLREP;
}

//-----------------------------------------------------------------------------
// Interface to ts::AsyncReport.
//-----------------------------------------------------------------------------

TSDUCKPY void* tspyNewAsyncReport(int severity, bool sync_log, bool timed_log, size_t log_msg_count)
{
    ts::AsyncReportArgs args;
    args.sync_log = sync_log;
    args.timed_log = timed_log;
    args.log_msg_count = log_msg_count > 0 ? log_msg_count : ts::AsyncReportArgs::MAX_LOG_MESSAGES;
    return new ts::AsyncReport(severity, args);
}

TSDUCKPY void tspyTerminateAsyncReport(void* report)
{
    ts::AsyncReport* rep = reinterpret_cast<ts::AsyncReport*>(report);
    if (rep != nullptr) {
        rep->terminate();
    }
}

//-----------------------------------------------------------------------------
// Interface to ts::py::AsyncReport.
//-----------------------------------------------------------------------------

TSDUCKPY void* tspyNewPyAsyncReport(ts::py::AsyncReport::LogCallback log, int severity, bool sync_log, size_t log_msg_count)
{
    ts::AsyncReportArgs args;
    args.sync_log = sync_log;
    args.log_msg_count = log_msg_count > 0 ? log_msg_count : ts::AsyncReportArgs::MAX_LOG_MESSAGES;
    return new ts::py::AsyncReport(log, severity, args);
}

//-----------------------------------------------------------------------------
// Interface to ts::py::SyncReport.
//-----------------------------------------------------------------------------

TSDUCKPY void* tspyNewPySyncReport(ts::py::SyncReport::LogCallback log, int severity)
{
    return new ts::py::SyncReport(log, severity);
}

//-----------------------------------------------------------------------------
// Delete a previously allocated instance of Report.
//-----------------------------------------------------------------------------

TSDUCKPY void tspyDeleteReport(void* report)
{
    delete reinterpret_cast<ts::Report*>(report);
}

//-----------------------------------------------------------------------------
// Set the maximum severity of an instance of Report.
//-----------------------------------------------------------------------------

TSDUCKPY void tspySetMaxSeverity(void* report, int severity)
{
    ts::Report* rep = reinterpret_cast<ts::Report*>(report);
    if (rep != nullptr) {
        rep->setMaxSeverity(severity);
    }
}

//-----------------------------------------------------------------------------
// Log a message on an instance of Report.
//-----------------------------------------------------------------------------

TSDUCKPY void tspyLogReport(void* report, int severity, const uint8_t* buffer, size_t size)
{
    ts::Report* rep = reinterpret_cast<ts::Report*>(report);
    if (rep != nullptr) {
        rep->log(severity, ts::py::ToString(buffer, size));
    }
}
