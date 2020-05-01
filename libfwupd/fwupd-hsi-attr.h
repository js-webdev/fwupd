/*
 * Copyright (C) 2020 Richard Hughes <richard@hughsie.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#pragma once

#include <glib-object.h>

#include "fwupd-enums.h"

G_BEGIN_DECLS

#define FWUPD_TYPE_HSI_ATTR (fwupd_hsi_attr_get_type ())
G_DECLARE_DERIVABLE_TYPE (FwupdHsiAttr, fwupd_hsi_attr, FWUPD, HSI_ATTR, GObject)

struct _FwupdHsiAttrClass
{
	GObjectClass			 parent_class;
	/*< private >*/
	void (*_fwupd_reserved1)	(void);
	void (*_fwupd_reserved2)	(void);
	void (*_fwupd_reserved3)	(void);
	void (*_fwupd_reserved4)	(void);
	void (*_fwupd_reserved5)	(void);
	void (*_fwupd_reserved6)	(void);
	void (*_fwupd_reserved7)	(void);
};


/**
 * FwupdHsiAttrFlags:
 * @FWUPD_HSI_ATTR_FLAG_NONE:			No flags set
 * @FWUPD_HSI_ATTR_FLAG_SUCCESS:		Success
 * @FWUPD_HSI_ATTR_FLAG_RUNTIME_UPDATES:	Suffix `U`
 * @FWUPD_HSI_ATTR_FLAG_RUNTIME_ATTESTATION:	Suffix `A`
 * @FWUPD_HSI_ATTR_FLAG_RUNTIME_ISSUE:		Suffix `!`
 * @FWUPD_HSI_ATTR_FLAG_RUNTIME_UNTRUSTED:	Suffix `?`
 *
 * The flags available for HSI attributes.
 **/
typedef enum {
	FWUPD_HSI_ATTR_FLAG_NONE		= 0,
	FWUPD_HSI_ATTR_FLAG_SUCCESS		= 1 << 0,
	FWUPD_HSI_ATTR_FLAG_RUNTIME_UPDATES	= 1 << 8,
	FWUPD_HSI_ATTR_FLAG_RUNTIME_ATTESTATION	= 1 << 9,
	FWUPD_HSI_ATTR_FLAG_RUNTIME_ISSUE	= 1 << 10,
	FWUPD_HSI_ATTR_FLAG_RUNTIME_UNTRUSTED	= 1 << 11,
} FwupdHsiAttrFlags;

/* the maximum value defined, although this might be increased in the future */
#define FWUPD_HSI_ATTR_NUMBER_MAX		5

FwupdHsiAttr	*fwupd_hsi_attr_new			(const gchar	*appstream_id);
gchar		*fwupd_hsi_attr_to_string		(FwupdHsiAttr	*self);

const gchar	*fwupd_hsi_attr_get_appstream_id	(FwupdHsiAttr	*self);
void		 fwupd_hsi_attr_set_appstream_id	(FwupdHsiAttr	*self,
							 const gchar	*appstream_id);
guint32		 fwupd_hsi_attr_get_number		(FwupdHsiAttr	*self);
void		 fwupd_hsi_attr_set_number		(FwupdHsiAttr	*self,
							 guint32	 number);
const gchar	*fwupd_hsi_attr_get_name		(FwupdHsiAttr	*self);
void		 fwupd_hsi_attr_set_name		(FwupdHsiAttr	*self,
							 const gchar	*name);
const gchar	*fwupd_hsi_attr_get_summary		(FwupdHsiAttr	*self);
void		 fwupd_hsi_attr_set_summary		(FwupdHsiAttr	*self,
							 const gchar	*summary);
GPtrArray	*fwupd_hsi_attr_get_obsoletes		(FwupdHsiAttr	*self);
void		 fwupd_hsi_attr_add_obsolete		(FwupdHsiAttr	*self,
							 const gchar	*appstream_id);
gboolean	 fwupd_hsi_attr_has_obsolete		(FwupdHsiAttr	*self,
							 const gchar	*appstream_id);
FwupdHsiAttrFlags fwupd_hsi_attr_get_flags		(FwupdHsiAttr	*self);
void		 fwupd_hsi_attr_set_flags		(FwupdHsiAttr	*self,
							 FwupdHsiAttrFlags flags);
void		 fwupd_hsi_attr_add_flag		(FwupdHsiAttr	*self,
							 FwupdHsiAttrFlags flag);
gboolean	 fwupd_hsi_attr_has_flag		(FwupdHsiAttr	*self,
							 FwupdHsiAttrFlags flag);

FwupdHsiAttr	*fwupd_hsi_attr_from_variant		(GVariant	*value);
GPtrArray	*fwupd_hsi_attr_array_from_variant	(GVariant	*value);

G_END_DECLS
