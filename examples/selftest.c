/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 Sanpe <sanpeqf@gmail.com>
 */

#include "xml.h"
#include <stdio.h>
#include <stdlib.h>

static const char xml_test[] = {
    "<!-- (C) 1999, 2000 WAP Forum Ltd.  All rights reserved -->\n"
    "<wml>\n"
    "<card id=\"card1\">\n"
    "<onevent type=\"onenterforward\">\n"
    "<go href=\"/vswap/run/result.eml\">\n"
    "        <postfield name=\"var\" value=\"&#36;test\"/>\n"
    "        <postfield name=\"v\" value=\"dark\"/>\n"
    "        <postfield name=\"ts\" value=\"0003\"/>\n"
    "        <postfield name=\"tp\" value=\"wml/state/variables/parsing/1\"/>\n"
    "        <postfield name=\"ti\" value=\"1\"/>\n"
    "        <postfield name=\"expected\" value=\"var:pass\"/>\n"
    "</go>\n"
    "</onevent>\n"
    "<p>If automatic testing failed, select <anchor>Failed<go\n"
    "href='/vswap/run/result.eml'>\n"
    "        <postfield name=\"SUBMIT\" value=\"No\"/><postfield name=\"v\"\n"
    "value=\"dark\"/>\n"
    "        <postfield name=\"ts\" value=\"0003\"/>\n"
    "        <postfield name=\"tp\" value=\"wml/state/variables/parsing/1\"/>\n"
    "        <postfield name=\"ti\" value=\"1\"/>\n"
    "        <postfield name=\"expected\" value=\"var:pass\"/></go></anchor>.</p>\n"
    "</card>\n"
    "</wml>\n"
};

static void xml_dumpinfo(struct xml_node *parent, unsigned int depth)
{
    struct xml_node *child;
    unsigned int count;

    printf("object: %s {\n", parent->name);
    list_for_each_entry(child, &parent->child, sibling) {
        for (count = 0; count < depth; ++count)
            printf("    ");
        if (xml_test_object(child)) {
            xml_dumpinfo(child, depth + 1);
            continue;
        }
        if (xml_test_attribute(child))
            printf("attribute: %s=%s", child->attr_name, child->attr_value);
        else if (xml_test_string(child))
            printf("string: %s", child->attr_name);
        printf("\n");
    }

    for (count = 0; count < depth - 1; ++count)
        printf("    ");
    printf("}\n");
}

int main(int argc, char *argv[])
{
    struct xml_node *xnode;
    int retval;
    char *buff;
    int length;

    retval = xml_parse(xml_test, &xnode);
    if (retval)
        return retval;

    printf("pseudo expression:\n");
    xml_dumpinfo(xnode, 1);

    printf("xml encode:\n");
    length = xml_encode(xnode, NULL, 0);

    buff = malloc(length);
    if (!buff) {
        retval = -ENOMEM;
        goto finish;
    }

    length = xml_encode(xnode, buff, length);
    fwrite(buff, length, 1, stdout);
    free(buff);

finish:
    xml_release(xnode);
    return retval;
}
