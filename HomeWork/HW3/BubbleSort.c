void bubble_a(long *data, long count)
{
    for (long last = count - 1; last > 0; last--)
    {
        for (int i = 0; i < last; i++)
        {
            if (data[i + 1] < data[i])
            {
                long t = data[i + 1];
                data[i + 1] = data[i];
                data[i] = t;
            }
        }
    }
}

void bubble_ptr(long *data, long count)
{
    for (long *last = data + count - 1; last > data; last--)
    {
        for (long *first = data; first < last; first++)
        {
            if (first[1] < *first)
            {
                long t = first[1];
                first[1] = *first;
                *first = t;
            }
        }
    }
}