/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#ifndef _JSON_H_
#define _JSON_H_

#include "list.h"
#include "macro.h"
#include <errno.h>

enum xml_flags {
    __XML_IS_OBJECT     = 0,
    __XML_IS_ATTRIBUTE  = 1,
    __XML_IS_STRING     = 2,
};

#define XML_IS_OBJECT       (1ULL << __XML_IS_OBJECT)
#define XML_IS_ATTRIBUTE    (1ULL << __XML_IS_ATTRIBUTE)
#define XML_IS_STRING       (1ULL << __XML_IS_STRING)

struct xml_node {
    struct xml_node *parent;
    struct list_head sibling;
    unsigned long flags;
    union {
        struct { /* object */
            char *name;
            struct list_head child;
        };
        struct { /* attribyte */
            char *attr_name;
            char *attr_value;
        }; /* string */
        char *string;
    };
};

#define GENERIC_XML_BITOPS(name, value)                     \
static inline void xml_clr_##name(struct xml_node *xml)     \
{                                                           \
    xml->flags &= ~value;                                   \
}                                                           \
                                                            \
static inline void xml_set_##name(struct xml_node *xml)     \
{                                                           \
    xml->flags |= value;                                    \
}                                                           \
                                                            \
static inline bool xml_test_##name(struct xml_node *xml)    \
{                                                           \
    return !!(xml->flags & value);                          \
}

GENERIC_XML_BITOPS(object, XML_IS_OBJECT)
GENERIC_XML_BITOPS(attribute, XML_IS_ATTRIBUTE)
GENERIC_XML_BITOPS(string, XML_IS_STRING)

extern int xml_parse(const char *buff, struct xml_node **root);
extern int xml_encode(struct xml_node *root, char *buff, int size);
extern void xml_release(struct xml_node *root);

#endif  /* _JSON_H_ */
