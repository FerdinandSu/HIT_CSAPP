long switch_prob(long x, long n){
    long result = x;
    switch (n){
    case 60:
    case 62:
        result = 8 * x;
        break;
    case 61:
    default:
        result = x - 0x4b;
        break;
    case 63:
        result = x >> 3;
        break;
    case 64:
        x = ((x << 4) - x);
    case 65:
        result = x * x + 0x4b;
        break;
    }     
    return result;  
}