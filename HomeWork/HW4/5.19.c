void psum1m4a(float a[], float p[], long n)
{
    long i;
    float val, last_val;
    float buf0, buf1, buf2, buf3;
    last_val = p[0] = a[0];

    for (i = 1; i < n - 4; i++)
    {
        buf0 = last_val + a[i];
        buf1 = buf0 + a[i + 1];
        buf2 = buf1 + a[i + 2];
        buf3 = buf2 + a[i + 3];

        p[i] = buf0;
        p[i + 1] = buf1;
        p[i + 2] = buf2;
        p[i + 3] = buf3;

        last_val = buf3;
    }
    for (; i < n; i++)
    {
        last_val += a[i];
        p[i] = last_val;
    }
