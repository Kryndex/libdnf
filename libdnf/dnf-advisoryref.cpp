/*
 * Copyright (C) 2014 Red Hat, Inc.
 * Copyright (C) 2015 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


/**
 * SECTION:dnf-advisoryref
 * @short_description: Update advisory reference
 * @include: libdnf.h
 * @stability: Unstable
 *
 * This object represents a reference given to an update.
 *
 * See also: #DnfContext
 */


#include <solv/repo.h>
#include <solv/util.h>

#include "dnf-advisoryref-private.hpp"

typedef struct
{
    Pool    *pool;
    Id       a_id;
    int      index;
} DnfAdvisoryRefPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(DnfAdvisoryRef, dnf_advisoryref, G_TYPE_OBJECT)
#define GET_PRIVATE(o) (static_cast<DnfAdvisoryRefPrivate *>(dnf_advisoryref_get_instance_private (o)))

/**
 * dnf_advisoryref_init:
 **/
static void
dnf_advisoryref_init(DnfAdvisoryRef *advisoryref)
{
}

/**
 * dnf_advisoryref_class_init:
 **/
static void
dnf_advisoryref_class_init(DnfAdvisoryRefClass *klass)
{
}

/**
 * dnf_advisoryref_new:
 *
 * Creates a new #DnfAdvisoryRef.
 *
 * Returns:(transfer full): a #DnfAdvisoryRef
 *
 * Since: 0.7.0
 **/
DnfAdvisoryRef *
dnf_advisoryref_new(Pool *pool, Id a_id, int index)
{
    auto advisoryref = DNF_ADVISORYREF(g_object_new(DNF_TYPE_ADVISORYREF, NULL));
    auto priv = GET_PRIVATE(advisoryref);
    priv->pool = pool;
    priv->a_id = a_id;
    priv->index = index;
    return advisoryref;
}

/**
 * dnf_advisoryref_compare:
 * @left: a #DnfAdvisoryRef instance.
 * @right: another #DnfAdvisoryRef instance.
 *
 * Checks two #DnfAdvisoryRef objects for equality.
 *
 * Returns: TRUE if the #DnfAdvisoryRef objects are the same
 *
 * Since: 0.7.0
 */
int
dnf_advisoryref_compare(DnfAdvisoryRef *left, DnfAdvisoryRef *right)
{
    DnfAdvisoryRefPrivate *lpriv = GET_PRIVATE(left);
    DnfAdvisoryRefPrivate *rpriv = GET_PRIVATE(right);
    return (lpriv->a_id == rpriv->a_id) && (lpriv->index == rpriv->index);
}

static const char *
advisoryref_get_str(DnfAdvisoryRef *advisoryref, Id keyname)
{
    DnfAdvisoryRefPrivate *priv = GET_PRIVATE(advisoryref);
    Dataiterator di;
    const char *str = NULL;
    int count = 0;

    dataiterator_init(&di, priv->pool, 0, priv->a_id, UPDATE_REFERENCE, 0, 0);
    while (dataiterator_step(&di)) {
        dataiterator_setpos(&di);
        if (count++ == priv->index) {
            str = pool_lookup_str(priv->pool, SOLVID_POS, keyname);
            break;
        }
    }
    dataiterator_free(&di);

    return str;
}

/**
 * dnf_advisoryref_get_kind:
 * @advisoryref: a #DnfAdvisoryRef instance.
 *
 * Gets the kind of advisory reference.
 *
 * Returns: a #DnfAdvisoryRef, e.g. %DNF_REFERENCE_KIND_BUGZILLA
 *
 * Since: 0.7.0
 */
DnfAdvisoryRefKind
dnf_advisoryref_get_kind(DnfAdvisoryRef *advisoryref)
{
    const char *type;
    type = advisoryref_get_str(advisoryref, UPDATE_REFERENCE_TYPE);
    if (type == NULL)
        return DNF_REFERENCE_KIND_UNKNOWN;
    if (!g_strcmp0 (type, "bugzilla"))
        return DNF_REFERENCE_KIND_BUGZILLA;
    if (!g_strcmp0 (type, "cve"))
        return DNF_REFERENCE_KIND_CVE;
    if (!g_strcmp0 (type, "vendor"))
        return DNF_REFERENCE_KIND_VENDOR;
    return DNF_REFERENCE_KIND_UNKNOWN;
}

/**
 * dnf_advisoryref_get_id:
 * @advisoryref: a #DnfAdvisoryRef instance.
 *
 * Gets an ID for the advisory.
 *
 * Returns: the advisory ID
 *
 * Since: 0.7.0
 */
const char *
dnf_advisoryref_get_id(DnfAdvisoryRef *advisoryref)
{
    return advisoryref_get_str(advisoryref, UPDATE_REFERENCE_ID);
}

/**
 * dnf_advisoryref_get_title:
 * @advisoryref: a #DnfAdvisoryRef instance.
 *
 * Gets a title to use for the advisory.
 *
 * Returns: the advisory title
 *
 * Since: 0.7.0
 */
const char *
dnf_advisoryref_get_title(DnfAdvisoryRef *advisoryref)
{
    return advisoryref_get_str(advisoryref, UPDATE_REFERENCE_TITLE);
}

/**
 * dnf_advisoryref_get_url:
 * @advisoryref: a #DnfAdvisoryRef instance.
 *
 * Gets the link for the advisory.
 *
 * Returns: the advisory URL
 *
 * Since: 0.7.0
 */
const char *
dnf_advisoryref_get_url(DnfAdvisoryRef *advisoryref)
{
    return advisoryref_get_str(advisoryref, UPDATE_REFERENCE_HREF);
}
