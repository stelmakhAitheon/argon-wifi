/*
 * Copyright (c) 2016 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SERVICES_SYSTEM_ERROR_H
#define SERVICES_SYSTEM_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum system_error_t {
    // SYSTEM_ERROR_NONE = 0,
    // SYSTEM_ERROR_UNKNOWN = -100,
    // ...
    // SYSTEM_ERROR_ENUM_VALUES(SYSTEM_ERROR_)
    SYSTEM_ERROR_NONE = 0, /* -999 ... 0: Generic result codes */ 
    SYSTEM_ERROR_UNKNOWN = -100, 
    SYSTEM_ERROR_BUSY = -110, 
    SYSTEM_ERROR_NOT_SUPPORTED = -120, 
    SYSTEM_ERROR_NOT_ALLOWED = -130, 
    SYSTEM_ERROR_CANCELLED = -140, 
    SYSTEM_ERROR_ABORTED = -150, 
    SYSTEM_ERROR_TIMEOUT = -160, 
    SYSTEM_ERROR_NOT_FOUND = -170, 
    SYSTEM_ERROR_ALREADY_EXISTS = -180, 
    SYSTEM_ERROR_TOO_LARGE = -190, 
    SYSTEM_ERROR_NOT_ENOUGH_DATA = -191, 
    SYSTEM_ERROR_LIMIT_EXCEEDED = -200, 
    SYSTEM_ERROR_END_OF_STREAM = -201, 
    SYSTEM_ERROR_INVALID_STATE = -210, 
    SYSTEM_ERROR_IO = -220, 
    SYSTEM_ERROR_WOULD_BLOCK = -221, 
    SYSTEM_ERROR_FILE = -225, 
    SYSTEM_ERROR_NETWORK = -230, 
    SYSTEM_ERROR_PROTOCOL = -240, 
    SYSTEM_ERROR_INTERNAL = -250, 
    SYSTEM_ERROR_NO_MEMORY = -260, 
    SYSTEM_ERROR_INVALID_ARGUMENT = -270, 
    SYSTEM_ERROR_BAD_DATA = -280, 
    SYSTEM_ERROR_OUT_OF_RANGE = -290, 
    SYSTEM_ERROR_DEPRECATED = -300, 
    SYSTEM_ERROR_COAP = -1000, /* -1199 ... -1000: CoAP errors */ 
    SYSTEM_ERROR_COAP_4XX = -1100, 
    SYSTEM_ERROR_COAP_5XX = -1132, 
    SYSTEM_ERROR_AT_NOT_OK = -1200, /* -1399 ... -1200: AT command errors */ 
    SYSTEM_ERROR_AT_RESPONSE_UNEXPECTED = -1210, 
    SYSTEM_ERROR_OTA_MODULE_NOT_FOUND = -1300, /* -1499 ... -1300: OTA update error */ 
    SYSTEM_ERROR_OTA_UNSUPPORTED_MODULE = -1310, 
    SYSTEM_ERROR_OTA_VALIDATION_FAILED = -1320, 
    SYSTEM_ERROR_OTA_INTEGRITY_CHECK_FAILED = -1330, 
    SYSTEM_ERROR_OTA_DEPENDENCY_CHECK_FAILED = -1340, 
    SYSTEM_ERROR_OTA_INVALID_ADDRESS = -1350, 
    SYSTEM_ERROR_OTA_INVALID_PLATFORM = -1360, 
    SYSTEM_ERROR_OTA_INVALID_FORMAT = -1370
} system_error_t;

// Returns default error message
// const char* system_error_message(int error, void* reserved);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // SERVICES_SYSTEM_ERROR_H
