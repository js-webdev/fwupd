/*
 * Copyright (C) 2020 Richard Hughes <richard@hughsie.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#pragma once

#include <glib-object.h>
#include <json-glib/json-glib.h>

#include "fwupd-hsi-attr.h"

G_BEGIN_DECLS

GVariant	*fwupd_hsi_attr_to_variant		(FwupdHsiAttr	*self);
void		 fwupd_hsi_attr_to_json			(FwupdHsiAttr	*self,
							 JsonBuilder	*builder);

G_END_DECLS

