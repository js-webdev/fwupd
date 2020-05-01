/*
 * Copyright (C) 2020 Richard Hughes <richard@hughsie.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#include "config.h"

//#include <glib-object.h>
#include <gio/gio.h>
//#include <string.h>

#include "fwupd-common-private.h"
#include "fwupd-enums-private.h"
//#include "fwupd-error.h"
#include "fwupd-hsi-attr-private.h"

/**
 * SECTION:fwupd-hsi-attr
 *
 * An object that represents an Host Security ID attribute.
 */

static void fwupd_hsi_attr_finalize	 (GObject *object);

typedef struct {
	gchar				*appstream_id;
	GPtrArray			*obsoletes;
	gchar				*name;
	gchar				*summary;
	gchar				*uri;
	guint32				 number;
	FwupdHsiAttrFlags		 flags;
} FwupdHsiAttrPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (FwupdHsiAttr, fwupd_hsi_attr, G_TYPE_OBJECT)
#define GET_PRIVATE(o) (fwupd_hsi_attr_get_instance_private (o))

static const gchar *
fwupd_hsi_attr_flag_to_string (FwupdHsiAttrFlags flag)
{
	if (flag == FWUPD_HSI_ATTR_FLAG_NONE)
		return "none";
	if (flag == FWUPD_HSI_ATTR_FLAG_SUCCESS)
		return "success";
	if (flag == FWUPD_HSI_ATTR_FLAG_RUNTIME_UPDATES)
		return "runtime-updates";
	if (flag == FWUPD_HSI_ATTR_FLAG_RUNTIME_ATTESTATION)
		return "runtime-attestation";
	if (flag == FWUPD_HSI_ATTR_FLAG_RUNTIME_ISSUE)
		return "runtime-issue";
	if (flag == FWUPD_HSI_ATTR_FLAG_RUNTIME_UNTRUSTED)
		return "runtime-untrusted";
	return NULL;
}

/**
 * fwupd_hsi_attr_get_obsoletes:
 * @self: A #FwupdHsiAttr
 *
 * Gets the self obsoletes.
 *
 * Returns: (element-type utf8) (transfer none): the obsoletes, which may be empty
 *
 * Since: 1.4.2
 **/
GPtrArray *
fwupd_hsi_attr_get_obsoletes (FwupdHsiAttr *self)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), NULL);
	return priv->obsoletes;
}

/**
 * fwupd_hsi_attr_add_obsolete:
 * @self: A #FwupdHsiAttr
 * @appstream_id: the attribute appstream_id
 *
 * Sets the attribute appstream_id.
 *
 * Since: 1.4.2
 **/
void
fwupd_hsi_attr_add_obsolete (FwupdHsiAttr *self, const gchar *appstream_id)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_if_fail (FWUPD_IS_HSI_ATTR (self));
	g_return_if_fail (appstream_id != NULL);
	for (guint i = 0; i < priv->obsoletes->len; i++) {
		const gchar *obsolete_tmp = g_ptr_array_index (priv->obsoletes, i);
		if (g_strcmp0 (obsolete_tmp, appstream_id) == 0)
			return;
	}
	g_ptr_array_add (priv->obsoletes, g_strdup (appstream_id));
}

/**
 * fwupd_hsi_attr_has_obsolete:
 * @self: A #FwupdHsiAttr
 * @appstream_id: the attribute appstream_id
 *
 * Finds out if the self has the attribute appstream_id.
 *
 * Returns: %TRUE if the self matches
 *
 * Since: 1.4.2
 **/
gboolean
fwupd_hsi_attr_has_obsolete (FwupdHsiAttr *self, const gchar *appstream_id)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), FALSE);
	g_return_val_if_fail (appstream_id != NULL, FALSE);
	for (guint i = 0; i < priv->obsoletes->len; i++) {
		const gchar *obsolete_tmp = g_ptr_array_index (priv->obsoletes, i);
		if (g_strcmp0 (obsolete_tmp, appstream_id) == 0)
			return TRUE;
	}
	return FALSE;
}

/**
 * fwupd_hsi_attr_get_uri:
 * @self: A #FwupdHsiAttr
 *
 * Gets the attribute uri.
 *
 * Returns: the attribute uri, or %NULL if unset
 *
 * Since: 1.4.2
 **/
const gchar *
fwupd_hsi_attr_get_uri (FwupdHsiAttr *self)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), NULL);
	return priv->uri;
}

/**
 * fwupd_hsi_attr_set_uri:
 * @self: A #FwupdHsiAttr
 * @uri: the attribute URI
 *
 * Sets the attribute uri, i.e. where you can download the firmware from.
 *
 * Since: 1.4.2
 **/
void
fwupd_hsi_attr_set_uri (FwupdHsiAttr *self, const gchar *uri)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_if_fail (FWUPD_IS_HSI_ATTR (self));
	g_free (priv->uri);
	priv->uri = g_strdup (uri);
}

/**
 * fwupd_hsi_attr_get_appstream_id:
 * @self: A #FwupdHsiAttr
 *
 * Gets the AppStream ID.
 *
 * Returns: the AppStream ID, or %NULL if unset
 *
 * Since: 1.4.2
 **/
const gchar *
fwupd_hsi_attr_get_appstream_id (FwupdHsiAttr *self)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), NULL);
	return priv->appstream_id;
}

/**
 * fwupd_hsi_attr_set_appstream_id:
 * @self: A #FwupdHsiAttr
 * @appstream_id: the AppStream component ID, e.g. `com.intel.BiosGuard`
 *
 * Sets the AppStream ID.
 *
 * Since: 1.4.2
 **/
void
fwupd_hsi_attr_set_appstream_id (FwupdHsiAttr *self, const gchar *appstream_id)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_if_fail (FWUPD_IS_HSI_ATTR (self));
	g_free (priv->appstream_id);
	priv->appstream_id = g_strdup (appstream_id);
}

/**
 * fwupd_hsi_attr_get_summary:
 * @self: A #FwupdHsiAttr
 *
 * Gets the attribute summary.
 *
 * Returns: the attribute summary, or %NULL if unset
 *
 * Since: 1.4.2
 **/
const gchar *
fwupd_hsi_attr_get_summary (FwupdHsiAttr *self)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), NULL);
	return priv->summary;
}

/**
 * fwupd_hsi_attr_set_name:
 * @self: A #FwupdHsiAttr
 * @summary: the attribute one line summary
 *
 * Sets the attribute name.
 *
 * Since: 1.4.2
 **/
void
fwupd_hsi_attr_set_name (FwupdHsiAttr *self, const gchar *name)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_if_fail (FWUPD_IS_HSI_ATTR (self));
	g_free (priv->name);
	priv->name = g_strdup (name);
}

/**
 * fwupd_hsi_attr_set_summary:
 * @self: A #FwupdHsiAttr
 * @summary: the attribute one line summary
 *
 * Sets the attribute summary.
 *
 * Since: 1.4.2
 **/
void
fwupd_hsi_attr_set_summary (FwupdHsiAttr *self, const gchar *summary)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_if_fail (FWUPD_IS_HSI_ATTR (self));
	g_free (priv->summary);
	priv->summary = g_strdup (summary);
}

/**
 * fwupd_hsi_attr_get_name:
 * @self: A #FwupdHsiAttr
 *
 * Gets the attribute name.
 *
 * Returns: the attribute name, or %NULL if unset
 *
 * Since: 1.4.2
 **/
const gchar *
fwupd_hsi_attr_get_name (FwupdHsiAttr *self)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), NULL);
	return priv->name;
}

/**
 * fwupd_hsi_attr_get_flags:
 * @self: A #FwupdHsiAttr
 *
 * Gets the self flags.
 *
 * Returns: the self flags, or 0 if unset
 *
 * Since: 1.4.2
 **/
FwupdHsiAttrFlags
fwupd_hsi_attr_get_flags (FwupdHsiAttr *self)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), 0);
	return priv->flags;
}

/**
 * fwupd_hsi_attr_set_flags:
 * @self: A #FwupdHsiAttr
 * @flags: the self flags, e.g. %FWUPD_HSI_ATTR_FLAG_TRUSTED_PAYLOAD
 *
 * Sets the self flags.
 *
 * Since: 1.4.2
 **/
void
fwupd_hsi_attr_set_flags (FwupdHsiAttr *self, FwupdHsiAttrFlags flags)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_if_fail (FWUPD_IS_HSI_ATTR (self));
	priv->flags = flags;
}

/**
 * fwupd_hsi_attr_add_flag:
 * @self: A #FwupdHsiAttr
 * @flag: the #FwupdHsiAttrFlags
 *
 * Adds a specific self flag to the self.
 *
 * Since: 1.4.2
 **/
void
fwupd_hsi_attr_add_flag (FwupdHsiAttr *self, FwupdHsiAttrFlags flag)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_if_fail (FWUPD_IS_HSI_ATTR (self));
	priv->flags |= flag;
}

/**
 * fwupd_hsi_attr_has_flag:
 * @self: A #FwupdHsiAttr
 * @flag: the #FwupdHsiAttrFlags
 *
 * Finds if the self has a specific self flag.
 *
 * Returns: %TRUE if the flag is set
 *
 * Since: 1.4.2
 **/
gboolean
fwupd_hsi_attr_has_flag (FwupdHsiAttr *self, FwupdHsiAttrFlags flag)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), FALSE);
	return (priv->flags & flag) > 0;
}

/**
 * fwupd_hsi_attr_get_number:
 * @self: A #FwupdHsiAttr
 *
 * Gets the time estimate for firmware installation (in seconds)
 *
 * Returns: the estimated time to flash this self (or 0 if unset)
 *
 * Since: 1.4.2
 **/
guint32
fwupd_hsi_attr_get_number (FwupdHsiAttr *self)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), 0);
	return priv->number;
}

/**
 * fwupd_hsi_attr_set_number:
 * @self: A #FwupdHsiAttr
 * @number: The amount of time
 *
 * Sets the time estimate for firmware installation (in seconds)
 *
 * Since: 1.4.2
 **/
void
fwupd_hsi_attr_set_number (FwupdHsiAttr *self, guint32 number)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_return_if_fail (FWUPD_IS_HSI_ATTR (self));
	priv->number = number;
}

/**
 * fwupd_hsi_attr_to_variant:
 * @self: A #FwupdHsiAttr
 *
 * Creates a GVariant from the self data.
 *
 * Returns: the GVariant, or %NULL for error
 *
 * Since: 1.4.2
 **/
GVariant *
fwupd_hsi_attr_to_variant (FwupdHsiAttr *self)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	GVariantBuilder builder;

	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), NULL);

	g_variant_builder_init (&builder, G_VARIANT_TYPE_VARDICT);
	if (priv->appstream_id != NULL) {
		g_variant_builder_add (&builder, "{sv}",
				       FWUPD_RESULT_KEY_APPSTREAM_ID,
				       g_variant_new_string (priv->appstream_id));
	}
	if (priv->name != NULL) {
		g_variant_builder_add (&builder, "{sv}",
				       FWUPD_RESULT_KEY_NAME,
				       g_variant_new_string (priv->name));
	}
	if (priv->summary != NULL) {
		g_variant_builder_add (&builder, "{sv}",
				       FWUPD_RESULT_KEY_SUMMARY,
				       g_variant_new_string (priv->summary));
	}
	if (priv->uri != NULL) {
		g_variant_builder_add (&builder, "{sv}",
				       FWUPD_RESULT_KEY_URI,
				       g_variant_new_string (priv->uri));
	}
	if (priv->obsoletes->len > 0) {
		g_autofree const gchar **strv = g_new0 (const gchar *, priv->obsoletes->len + 1);
		for (guint i = 0; i < priv->obsoletes->len; i++)
			strv[i] = (const gchar *) g_ptr_array_index (priv->obsoletes, i);
		g_variant_builder_add (&builder, "{sv}",
				       FWUPD_RESULT_KEY_CATEGORIES,
				       g_variant_new_strv (strv, -1));
	}
	if (priv->flags != 0) {
		g_variant_builder_add (&builder, "{sv}",
				       FWUPD_RESULT_KEY_TRUST_FLAGS,
				       g_variant_new_uint64 (priv->flags));
	}
	if (priv->number > 0) {
		g_variant_builder_add (&builder, "{sv}",
				       FWUPD_RESULT_KEY_HSI_NUMBER,
				       g_variant_new_uint32 (priv->number));
	}
	return g_variant_new ("a{sv}", &builder);
}

static void
fwupd_hsi_attr_from_key_value (FwupdHsiAttr *self, const gchar *key, GVariant *value)
{
	if (g_strcmp0 (key, FWUPD_RESULT_KEY_APPSTREAM_ID) == 0) {
		fwupd_hsi_attr_set_appstream_id (self, g_variant_get_string (value, NULL));
		return;
	}
	if (g_strcmp0 (key, FWUPD_RESULT_KEY_NAME) == 0) {
		fwupd_hsi_attr_set_name (self, g_variant_get_string (value, NULL));
		return;
	}
	if (g_strcmp0 (key, FWUPD_RESULT_KEY_SUMMARY) == 0) {
		fwupd_hsi_attr_set_summary (self, g_variant_get_string (value, NULL));
		return;
	}
	if (g_strcmp0 (key, FWUPD_RESULT_KEY_CHECKSUM) == 0) {
		g_autofree const gchar **strv = g_variant_get_strv (value, NULL);
		for (guint i = 0; strv[i] != NULL; i++)
			fwupd_hsi_attr_add_obsolete (self, strv[i]);
		return;
	}
	if (g_strcmp0 (key, FWUPD_RESULT_KEY_URI) == 0) {
		fwupd_hsi_attr_set_uri (self, g_variant_get_string (value, NULL));
		return;
	}
	if (g_strcmp0 (key, FWUPD_RESULT_KEY_TRUST_FLAGS) == 0) {
		fwupd_hsi_attr_set_flags (self, g_variant_get_uint64 (value));
		return;
	}
	if (g_strcmp0 (key, FWUPD_RESULT_KEY_HSI_NUMBER) == 0) {
		fwupd_hsi_attr_set_number (self, g_variant_get_uint32 (value));
		return;
	}
}

static void
fwupd_pad_kv_str (GString *str, const gchar *key, const gchar *value)
{
	/* ignore */
	if (key == NULL || value == NULL)
		return;
	g_string_append_printf (str, "  %s: ", key);
	for (gsize i = strlen (key); i < 20; i++)
		g_string_append (str, " ");
	g_string_append_printf (str, "%s\n", value);
}

static void
fwupd_pad_kv_tfl (GString *str, const gchar *key, FwupdHsiAttrFlags hsi_attr_flags)
{
	g_autoptr(GString) tmp = g_string_new ("");
	for (guint i = 0; i < 64; i++) {
		if ((hsi_attr_flags & ((guint64) 1 << i)) == 0)
			continue;
		g_string_append_printf (tmp, "%s|",
					fwupd_hsi_attr_flag_to_string ((guint64) 1 << i));
	}
	if (tmp->len == 0) {
		g_string_append (tmp, fwupd_hsi_attr_flag_to_string (0));
	} else {
		g_string_truncate (tmp, tmp->len - 1);
	}
	fwupd_pad_kv_str (str, key, tmp->str);
}

static void
fwupd_pad_kv_int (GString *str, const gchar *key, guint32 value)
{
	g_autofree gchar *tmp = NULL;

	/* ignore */
	if (value == 0)
		return;
	tmp = g_strdup_printf("%" G_GUINT32_FORMAT, value);
	fwupd_pad_kv_str (str, key, tmp);
}

static void
fwupd_hsi_attr_json_add_string (JsonBuilder *builder, const gchar *key, const gchar *str)
{
	if (str == NULL)
		return;
	json_builder_set_member_name (builder, key);
	json_builder_add_string_value (builder, str);
}

static void
fwupd_hsi_attr_json_add_int (JsonBuilder *builder, const gchar *key, guint64 num)
{
	if (num == 0)
		return;
	json_builder_set_member_name (builder, key);
	json_builder_add_int_value (builder, num);
}

/**
 * fwupd_hsi_attr_to_json:
 * @self: A #FwupdHsiAttr
 * @builder: A #JsonBuilder
 *
 * Adds a fwupd self to a JSON builder
 *
 * Since: 1.4.2
 **/
void
fwupd_hsi_attr_to_json (FwupdHsiAttr *self, JsonBuilder *builder)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	g_autoptr(GList) keys = NULL;

	g_return_if_fail (FWUPD_IS_HSI_ATTR (self));
	g_return_if_fail (builder != NULL);

	fwupd_hsi_attr_json_add_string (builder, FWUPD_RESULT_KEY_APPSTREAM_ID, priv->appstream_id);
	fwupd_hsi_attr_json_add_int (builder, FWUPD_RESULT_KEY_HSI_NUMBER, priv->number);
	fwupd_hsi_attr_json_add_string (builder, FWUPD_RESULT_KEY_NAME, priv->name);
	fwupd_hsi_attr_json_add_string (builder, FWUPD_RESULT_KEY_SUMMARY, priv->summary);
	if (priv->obsoletes->len > 0) {
		json_builder_set_member_name (builder, FWUPD_RESULT_KEY_CHECKSUM);
		json_builder_begin_array (builder);
		for (guint i = 0; i < priv->obsoletes->len; i++) {
			const gchar *appstream_id = g_ptr_array_index (priv->obsoletes, i);
			json_builder_add_string_value (builder, appstream_id);
		}
		json_builder_end_array (builder);
	}
	fwupd_hsi_attr_json_add_string (builder, FWUPD_RESULT_KEY_URI, priv->uri);
	if (priv->flags != FWUPD_HSI_ATTR_FLAG_NONE) {
		json_builder_set_member_name (builder, FWUPD_RESULT_KEY_FLAGS);
		json_builder_begin_array (builder);
		for (guint i = 0; i < 64; i++) {
			const gchar *tmp;
			if ((priv->flags & ((guint64) 1 << i)) == 0)
				continue;
			tmp = fwupd_hsi_attr_flag_to_string ((guint64) 1 << i);
			json_builder_add_string_value (builder, tmp);
		}
		json_builder_end_array (builder);
	}
}

/**
 * fwupd_hsi_attr_to_string:
 * @self: A #FwupdHsiAttr
 *
 * Builds a text representation of the object.
 *
 * Returns: text, or %NULL for invalid
 *
 * Since: 1.4.2
 **/
gchar *
fwupd_hsi_attr_to_string (FwupdHsiAttr *self)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	GString *str;
	g_autoptr(GList) keys = NULL;

	g_return_val_if_fail (FWUPD_IS_HSI_ATTR (self), NULL);

	str = g_string_new ("");
	fwupd_pad_kv_str (str, FWUPD_RESULT_KEY_APPSTREAM_ID, priv->appstream_id);
	fwupd_pad_kv_int (str, FWUPD_RESULT_KEY_HSI_NUMBER, priv->number);
	if (priv->flags != FWUPD_HSI_ATTR_FLAG_NONE)
		fwupd_pad_kv_tfl (str, FWUPD_RESULT_KEY_FLAGS, priv->flags);
	fwupd_pad_kv_str (str, FWUPD_RESULT_KEY_NAME, priv->name);
	fwupd_pad_kv_str (str, FWUPD_RESULT_KEY_SUMMARY, priv->summary);
	for (guint i = 0; i < priv->obsoletes->len; i++) {
		const gchar *appstream_id = g_ptr_array_index (priv->obsoletes, i);
		fwupd_pad_kv_str (str, FWUPD_RESULT_KEY_CHECKSUM, appstream_id);
	}

	return g_string_free (str, FALSE);
}

static void
fwupd_hsi_attr_class_init (FwupdHsiAttrClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	object_class->finalize = fwupd_hsi_attr_finalize;
}

static void
fwupd_hsi_attr_init (FwupdHsiAttr *self)
{
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);
	priv->obsoletes = g_ptr_array_new_with_free_func (g_free);
}

static void
fwupd_hsi_attr_finalize (GObject *object)
{
	FwupdHsiAttr *self = FWUPD_HSI_ATTR (object);
	FwupdHsiAttrPrivate *priv = GET_PRIVATE (self);

	g_free (priv->appstream_id);
	g_free (priv->name);
	g_free (priv->summary);
	g_free (priv->uri);
	g_ptr_array_unref (priv->obsoletes);

	G_OBJECT_CLASS (fwupd_hsi_attr_parent_class)->finalize (object);
}

static void
fwupd_hsi_attr_set_from_variant_iter (FwupdHsiAttr *self, GVariantIter *iter)
{
	GVariant *value;
	const gchar *key;
	while (g_variant_iter_next (iter, "{&sv}", &key, &value)) {
		fwupd_hsi_attr_from_key_value (self, key, value);
		g_variant_unref (value);
	}
}

/**
 * fwupd_hsi_attr_from_variant:
 * @value: a #GVariant
 *
 * Creates a new self using packed data.
 *
 * Returns: (transfer full): a new #FwupdHsiAttr, or %NULL if @value was invalid
 *
 * Since: 1.4.2
 **/
FwupdHsiAttr *
fwupd_hsi_attr_from_variant (GVariant *value)
{
	FwupdHsiAttr *rel = NULL;
	const gchar *type_string;
	g_autoptr(GVariantIter) iter = NULL;

	type_string = g_variant_get_type_string (value);
	if (g_strcmp0 (type_string, "(a{sv})") == 0) {
		rel = fwupd_hsi_attr_new (NULL);
		g_variant_get (value, "(a{sv})", &iter);
		fwupd_hsi_attr_set_from_variant_iter (rel, iter);
	} else if (g_strcmp0 (type_string, "a{sv}") == 0) {
		rel = fwupd_hsi_attr_new (NULL);
		g_variant_get (value, "a{sv}", &iter);
		fwupd_hsi_attr_set_from_variant_iter (rel, iter);
	} else {
		g_warning ("type %s not known", type_string);
	}
	return rel;
}

/**
 * fwupd_hsi_attr_array_from_variant:
 * @value: a #GVariant
 *
 * Creates an array of new hsi_attrs using packed data.
 *
 * Returns: (transfer container) (element-type FwupdHsiAttr): hsi_attrs, or %NULL if @value was invalid
 *
 * Since: 1.4.2
 **/
GPtrArray *
fwupd_hsi_attr_array_from_variant (GVariant *value)
{
	GPtrArray *array = NULL;
	gsize sz;
	g_autoptr(GVariant) untuple = NULL;

	array = g_ptr_array_new_with_free_func ((GDestroyNotify) g_object_unref);
	untuple = g_variant_get_child_value (value, 0);
	sz = g_variant_n_children (untuple);
	for (guint i = 0; i < sz; i++) {
		FwupdHsiAttr *rel;
		g_autoptr(GVariant) data = NULL;
		data = g_variant_get_child_value (untuple, i);
		rel = fwupd_hsi_attr_from_variant (data);
		if (rel == NULL)
			continue;
		g_ptr_array_add (array, rel);
	}
	return array;
}

/**
 * fwupd_hsi_attr_new:
 * @appstream_id: (allow-none): the AppStream component ID, e.g. `com.intel.BiosGuard`
 *
 * Creates a new self.
 *
 * Returns: a new #FwupdHsiAttr
 *
 * Since: 1.4.2
 **/
FwupdHsiAttr *
fwupd_hsi_attr_new (const gchar *appstream_id)
{
	FwupdHsiAttr *self;
	self = g_object_new (FWUPD_TYPE_HSI_ATTR, NULL);
	if (appstream_id != NULL)
		fwupd_hsi_attr_set_appstream_id (self, appstream_id);
	return FWUPD_HSI_ATTR (self);
}
