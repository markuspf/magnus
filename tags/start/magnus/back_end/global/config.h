/*
 *   $Id: config.h 4 2005-01-07 00:28:20Z daly $ 
 */

// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Configuration variables set up by the config script
//
// Principal Author: Jon Lennox
//
// Status: Useable.
//
// Revision History:
//

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define MAGNUS_HOME "/root/magnus"
// We keep this compiled-in value to ease debugging.
// Use MagnusHome::magnusHome() in back_end/general/ for
// the Magnus root directory.

#define LINUX

#define MAGNUS_TMP "/tmp"

#endif /* _CONFIG_H_ */