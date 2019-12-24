;double c_img(double complex x)
c_img:
    movapd xmm0, xmm1
    ret
;double c_real(double complex x)   
c_real:
    ret
;double complex c_sub(double complex x, double complex y)
c_sub:
    subsd xmm0, xmm2
    subsd xmm1, xmm3
    