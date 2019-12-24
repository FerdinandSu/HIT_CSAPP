typedef long data_t ;

typedef struct{
    long len;
    data_t *data;
}vec_rec,*vec_ptr;

void inner4(vec_ptr u,vec_ptr v, data_t *dest){
    long length=vec_length(u);
    data_t* udata=get_vec_start(u);
    data_t* vdata=get_vec_start(v);
    data_t sum=(data_t)0;
    for (long i = 0; i < length; i++)
    {
        sum+=udata[i]*vdata[i];
    }
    *dest=sum;
}

void inner4m6(vec_ptr u,vec_ptr v, data_t *dest){
    long i;
    long length=vec_length(u);
    long limit=length-5;
    data_t* udata=get_vec_start(u);
    data_t* vdata=get_vec_start(v);
    data_t sum0=(data_t)0,
        sum1=(data_t)0,
        sum2=(data_t)0,
        sum3=(data_t)0,
        sum4=(data_t)0,
        sum5=(data_t)0;
    for (i = 0; i < limit; i+=6)
    {
        sum0+=udata[i]*vdata[i];
        sum1+=udata[i+1]*vdata[i+1];
        sum2+=udata[i+2]*vdata[i+2];
        sum3+=udata[i+3]*vdata[i+3];
        sum4+=udata[i+4]*vdata[i+4];
        sum5+=udata[i+5]*vdata[i+5];
    }
    for(;i<length;i++){
        sum0+=udata[i]*vdata[i];
    }
    *dest=sum0+sum1+sum2+sum3+sum4+sum5;
}