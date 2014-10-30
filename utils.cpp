extern "C" {
#include "sc_memory_headers.h"
#include "utils.h"
#include "sc_helper.h"
}

#include <stdio.h>
#include <iostream>

using namespace std;

void printEl(sc_addr element)
{
    sc_addr idtf;
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
