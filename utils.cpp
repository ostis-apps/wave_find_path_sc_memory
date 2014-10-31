extern "C" {
#include "sc_memory_headers.h"
#include "utils.h"
#include "sc_helper.h"
}

#include <stdio.h>
#include <iostream>

#define NUMBER_PRECISE 8

using namespace std;

void printEl(sc_addr element)
{
    sc_addr idtf;
    sc_type type;
    sc_memory_get_element_type(element, &type);

    if ((sc_type_node & type) == sc_type_node)
    {

        if (SC_RESULT_OK == sc_helper_get_system_identifier(element, &idtf))
        {
            sc_stream *stream;
            sc_uint32 length = 0, read_length = 0;
            sc_char *data;
            sc_memory_get_link_content(idtf, &stream);
            sc_stream_get_length(stream, &length);
            data = (sc_char *)calloc(length + 1, sizeof(sc_char));
            sc_stream_read_data(stream, data, length, &read_length);
            data[length] = '\0';
            printf("%s", data);
            sc_stream_free(stream);
            free(data);
        }
        else
        {
            printf("%u|%u", element.seg, element.offset);
        }
    }
    else
    {
        sc_addr elem1, elem2;
        sc_memory_get_arc_begin(element, &elem1);
        sc_memory_get_arc_end(element, &elem2);
        printf("(");
        printEl(elem1);
        printf(" -> ");
        printEl(elem2);
        printf(")");
    }
}

void printContent(sc_addr element)
{
    sc_stream *stream;
    sc_uint32 length = 0, read_length = 0;
    sc_char *data;
    if (sc_memory_get_link_content(element, &stream) != SC_RESULT_OK)
    {
        printf("Content reading error\n");
    }
    sc_stream_get_length(stream, &length);
    data = (sc_char *)calloc(length + 1, sizeof(sc_char));
    sc_stream_read_data(stream, data, length, &read_length);
    data[length] = '\0';
    printf("%s", data);
    sc_stream_free(stream);
    free(data);
}

int getInt(sc_addr element)
{
    int res = 0;
    sc_stream *stream;
    sc_uint32 length = 0, read_length = 0;
    sc_char *data;
    if (SC_RESULT_OK != sc_memory_get_link_content(element, &stream))
    {
        printf("Can't read content\n");
        return -1;
    }
    sc_stream_get_length(stream, &length);
    data = (sc_char *)calloc(length + 1, sizeof(sc_char));
    sc_stream_read_data(stream, data, length, &read_length);
    data[length] = '\0';

    res = atoi(data);

    sc_stream_free(stream);
    free(data);
    return res;
}

int getFloat(sc_addr element)
{
    int res = 0;
    sc_stream *stream;
    sc_uint32 length = 0, read_length = 0;
    sc_char *data;
    if (SC_RESULT_OK != sc_memory_get_link_content(element, &stream))
    {
        printf("Can't read content\n");
        return -1;
    }
    sc_stream_get_length(stream, &length);
    data = (sc_char *)calloc(length + 1, sizeof(sc_char));
    sc_stream_read_data(stream, data, length, &read_length);
    data[length] = '\0';

    res = atof(data);

    sc_stream_free(stream);
    free(data);
    return res;
}

sc_addr genIntNode(sc_uint32 data)
{
    sc_stream *stream;
    sc_addr link;
    char *content = (char *)calloc(NUMBER_PRECISE, sizeof(sc_char));
    snprintf(content, NUMBER_PRECISE, "%d", data);

    link = sc_memory_link_new();
    stream = sc_stream_memory_new(content, strlen(content), SC_STREAM_READ, SC_FALSE);
    if (SC_RESULT_OK != sc_memory_set_link_content(link, stream))
    {
        free(content);
        sc_memory_element_free(link);
        SC_ADDR_MAKE_EMPTY(link);
        sc_stream_free(stream);
        return link;
    }
    free(content);
    sc_stream_free(stream);
    return link;
}

sc_addr genFloatNode(float data)
{
    sc_stream *stream;
    sc_addr link;
    char *content = (char *)calloc(NUMBER_PRECISE, sizeof(sc_char));
    snprintf(content, NUMBER_PRECISE, "%f", data);

    link = sc_memory_link_new();
    stream = sc_stream_memory_new(content, strlen(content), SC_STREAM_READ, SC_FALSE);
    if (SC_RESULT_OK != sc_memory_set_link_content(link, stream))
    {
        free(content);
        sc_memory_element_free(link);
        SC_ADDR_MAKE_EMPTY(link);
        sc_stream_free(stream);
        return link;
    }
    free(content);
    sc_stream_free(stream);
    return link;
}
