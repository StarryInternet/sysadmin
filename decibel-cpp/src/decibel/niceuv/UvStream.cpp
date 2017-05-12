
#include <cstdlib>

#include "decibel/niceuv/UvStream.h"

namespace decibel
{
namespace niceuv
{
void on_alloc(uv_handle_t*, size_t sug_size, uv_buf_t* buf)
{
    *buf = uv_buf_init((char*)malloc(sizeof(char) * sug_size), sug_size);
}

void on_write(uv_write_t* req, int)
{
    char* pBuffer = (char*)req->data;
    free(pBuffer);
    delete req;
}
}
}
