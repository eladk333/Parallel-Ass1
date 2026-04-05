/* 322587064 Elad Katz */

long decode_c_version(long x, long y, long z)
{
    y = y-z;
    x = y*x;
    
    long ret = 0;
    ret = y;
    ret = ret<<63;
    ret = ret>>63;
    ret = ret^x;
    return ret;
}