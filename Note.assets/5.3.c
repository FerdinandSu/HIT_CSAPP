#include "combine.h"

void combine1(vec_ptr v, data_t *dest)
{
    *dest = IDENT;
    for (long i = 0; i < vec_length(v); i++)
    {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}

void combine4(vec_ptr v, data_t *dest)
{
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;
    for (long i = 0; i < length; i++)
    {
        acc = acc OP data[i];
    }
    *dest = acc;
}

/* 
 * 2*1 循环展开
 * 2 表示每轮循环运算2次
 * 1 表示每次只有一个缓冲变量。
 */
void combine5(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    long limit = length - 1;
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;
    for (i = 0; i < limit; i += 2)
    {
        acc = (acc OP data[i])OP data[i + 1];
    }
    //剩余项
    for (; i < length; i++)
    {
        acc = acc OP data[i];
    }
    *dest = acc;
}

/* 
 * 2*2 循环展开
 * 2 表示每轮循环运算2次
 * 2 表示每次有2个缓冲变量。
 */
void combine6(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    long limit = length - 1;
    data_t *data = get_vec_start(v);
    data_t acc0 = IDENT;
    data_t acc1 = IDENT;
    for (i = 0; i < limit; i += 2)
    {
        acc0 = acc0 OP data[i];
        acc1 = acc1 OP data[i + 1];
    }
    //剩余项
    for (; i < length; i++)
    {
        acc0 = acc0 OP data[i];
    }
    *dest = acc0 OP acc1;
}



/* 
 * 2*1a 循环展开
 * 2 表示每轮循环运算2次
 * 1 表示每次只有一个缓冲变量。
 * a 表示元素结合方式的改变为右优先结合
 */
void combine7(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    long limit = length - 1;
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;
    for (i = 0; i < limit; i += 2)
    {
        acc = acc OP (data[i] OP data[i + 1]);
    }
    //剩余项
    for (; i < length; i++)
    {
        acc = acc OP data[i];
    }
    *dest = acc;
}